#include <msgpack.hpp>

#define BOOST_TEST_MODULE object
#include <boost/test/unit_test.hpp>

enum enum_test {
    elem
};

MSGPACK_ADD_ENUM(enum_test);

struct outer_enum {
    enum enum_test {
        elem
    };
};

MSGPACK_ADD_ENUM(outer_enum::enum_test);

#if !defined(MSGPACK_USE_CPP03)

enum class enum_class_test {
    elem
};

MSGPACK_ADD_ENUM(enum_class_test);

struct outer_enum_class {
    enum class enum_class_test {
        elem
    };
};

MSGPACK_ADD_ENUM(outer_enum_class::enum_class_test);

#endif // !defined(MSGPACK_USE_CPP03)

struct myclass {
    myclass() : num(0), str("default") { }

    myclass(int num, const std::string& str) :
        num(num), str(str) { }

    ~myclass() { }

    int num;
    std::string str;
    std::vector<double> vec;
    std::map<std::string, std::vector<char> > map;

    MSGPACK_DEFINE(num, str, vec, map);

    bool operator==(const myclass& o) const
    {
        return num == o.num && str == o.str && vec == o.vec && map == o.map;
    }
};

std::ostream& operator<<(std::ostream& o, const myclass& m)
{
    return o << "myclass("<<m.num<<",\""<<m.str<<"\")";
}


BOOST_AUTO_TEST_CASE(convert)
{
    myclass m1(1, "custom");

    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, m1);

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());

    myclass m2;
    oh.get().convert(m2);

    BOOST_CHECK_EQUAL(m1, m2);
}


BOOST_AUTO_TEST_CASE(as)
{
    myclass m1(1, "custom");

    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, m1);

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());

    BOOST_CHECK_EQUAL(m1, oh.get().as<myclass>());
}

BOOST_AUTO_TEST_CASE(cross_zone_copy)
{
    myclass m1(1, "custom");
    m1.vec.push_back(1.0);
    m1.vec.push_back(0.1);
    std::vector<char> vc;
    vc.push_back('t');
    vc.push_back('w');
    vc.push_back('o');
    m1.map["one"] = vc;

    msgpack::zone z1;
    msgpack::object::with_zone obj1(z1);

    {
        msgpack::zone z2;
        msgpack::object::with_zone obj2(z2);
        obj2 << m1;

        obj1 << obj2;

        BOOST_CHECK_EQUAL(obj1.via.array.ptr[2].via.array.ptr[0].via.f64, 1.0);
#if defined(MSGPACK_USE_LEGACY_NAME_AS_FLOAT)
        BOOST_CHECK_EQUAL(obj1.via.array.ptr[2].via.array.ptr[0].via.dec, 1.0);
#endif // MSGPACK_USE_LEGACY_NAME_AS_FLOAT
        BOOST_CHECK_EQUAL(obj1.via.array.ptr[3].via.map.ptr[0].key.via.str.ptr[0], 'o');
        BOOST_CHECK_EQUAL(obj1.via.array.ptr[3].via.map.ptr[0].val.via.bin.ptr[0], 't');
        BOOST_CHECK_NE(
            obj1.via.array.ptr[2].via.array.ptr,
            obj2.via.array.ptr[2].via.array.ptr);
        BOOST_CHECK_NE(
            obj1.via.array.ptr[3].via.map.ptr,
            obj2.via.array.ptr[3].via.map.ptr);
        BOOST_CHECK(
            obj1.via.array.ptr[3].via.map.ptr[0].key.via.str.ptr !=
            obj2.via.array.ptr[3].via.map.ptr[0].key.via.str.ptr);
        BOOST_CHECK(
            obj1.via.array.ptr[3].via.map.ptr[0].val.via.bin.ptr !=
            obj2.via.array.ptr[3].via.map.ptr[0].val.via.bin.ptr);
    }

    BOOST_CHECK_EQUAL(m1, obj1.as<myclass>());
}

BOOST_AUTO_TEST_CASE(cross_zone_copy_construct)
{
    myclass m1(1, "custom");
    m1.vec.push_back(1.0);
    m1.vec.push_back(0.1);
    std::vector<char> vc;
    vc.push_back('t');
    vc.push_back('w');
    vc.push_back('o');
    m1.map["one"] = vc;

    msgpack::zone z1;
    msgpack::zone z2;
    msgpack::object::with_zone obj2(z2);
    obj2 << m1;

    msgpack::object obj1(obj2, z1);

    BOOST_CHECK_EQUAL(obj1.via.array.ptr[2].via.array.ptr[0].via.f64, 1.0);
#if defined(MSGPACK_USE_LEGACY_NAME_AS_FLOAT)
    BOOST_CHECK_EQUAL(obj1.via.array.ptr[2].via.array.ptr[0].via.dec, 1.0);
#endif // MSGPACK_USE_LEGACY_NAME_AS_FLOAT
    BOOST_CHECK_EQUAL(obj1.via.array.ptr[3].via.map.ptr[0].key.via.str.ptr[0], 'o');
    BOOST_CHECK_EQUAL(obj1.via.array.ptr[3].via.map.ptr[0].val.via.bin.ptr[0], 't');
    BOOST_CHECK_NE(
        obj1.via.array.ptr[2].via.array.ptr,
        obj2.via.array.ptr[2].via.array.ptr);
    BOOST_CHECK_NE(
        obj1.via.array.ptr[3].via.map.ptr,
        obj2.via.array.ptr[3].via.map.ptr);
    BOOST_CHECK(
        obj1.via.array.ptr[3].via.map.ptr[0].key.via.str.ptr !=
        obj2.via.array.ptr[3].via.map.ptr[0].key.via.str.ptr);
    BOOST_CHECK(
        obj1.via.array.ptr[3].via.map.ptr[0].val.via.bin.ptr !=
        obj2.via.array.ptr[3].via.map.ptr[0].val.via.bin.ptr);
    BOOST_CHECK_EQUAL(m1, obj1.as<myclass>());
}

BOOST_AUTO_TEST_CASE(cross_zone_copy_ext)
{
    msgpack::zone z1;
    msgpack::zone z2;
    msgpack::object::with_zone obj1(z1);

    obj1.type = msgpack::type::EXT;
    char* ptr = static_cast<char*>(obj1.zone.allocate_align(2, MSGPACK_ZONE_ALIGNOF(char)));
    ptr[0] = 1;
    ptr[1] = 2;
    obj1.via.ext.ptr = ptr;
    obj1.via.ext.size = 1;

    msgpack::object::with_zone obj2(z2);
    obj2 << obj1;
    BOOST_CHECK_EQUAL(obj2.via.ext.size, 1u);
    BOOST_CHECK_EQUAL(obj2.via.ext.ptr[0], 1);
    BOOST_CHECK_EQUAL(obj2.via.ext.ptr[1], 2);
    BOOST_CHECK(
        obj1.via.ext.ptr !=
        obj2.via.ext.ptr);
}

BOOST_AUTO_TEST_CASE(cross_zone_copy_construct_ext)
{
    msgpack::zone z1;
    msgpack::zone z2;
    msgpack::object::with_zone obj1(z1);

    obj1.type = msgpack::type::EXT;
    char* ptr = static_cast<char*>(obj1.zone.allocate_align(2, MSGPACK_ZONE_ALIGNOF(char)));
    ptr[0] = 1;
    ptr[1] = 2;
    obj1.via.ext.ptr = ptr;
    obj1.via.ext.size = 1;

    msgpack::object obj2(obj1, z2);
    BOOST_CHECK_EQUAL(obj2.via.ext.size, 1u);
    BOOST_CHECK_EQUAL(obj2.via.ext.ptr[0], 1);
    BOOST_CHECK_EQUAL(obj2.via.ext.ptr[1], 2);
    BOOST_CHECK(
        obj1.via.ext.ptr !=
        obj2.via.ext.ptr);
}

BOOST_AUTO_TEST_CASE(print)
{
    msgpack::object obj;
    std::stringstream output;
    output << obj;
    BOOST_CHECK_EQUAL(output.str(), "null");
}

BOOST_AUTO_TEST_CASE(is_nil)
{
    msgpack::object obj;
    BOOST_CHECK(obj.is_nil());
}

BOOST_AUTO_TEST_CASE(type_error)
{
    msgpack::object obj(1);
    BOOST_CHECK_THROW(obj.as<std::string>(), msgpack::type_error);
    BOOST_CHECK_THROW(obj.as<std::vector<int> >(), msgpack::type_error);
    BOOST_CHECK_EQUAL(1, obj.as<int>());
    BOOST_CHECK_EQUAL(1, obj.as<short>());
    BOOST_CHECK_EQUAL(1u, obj.as<unsigned int>());
    BOOST_CHECK_EQUAL(1u, obj.as<unsigned long>());
}

BOOST_AUTO_TEST_CASE(equal_primitive)
{
    msgpack::object obj_nil;
    BOOST_CHECK_EQUAL(obj_nil, msgpack::object());

    msgpack::object obj_int(1);
    BOOST_CHECK_EQUAL(obj_int, msgpack::object(1));
    BOOST_CHECK_EQUAL(obj_int, 1);

    msgpack::object obj_float(1.2);
    BOOST_CHECK_EQUAL(obj_float, msgpack::object(1.2));
    BOOST_CHECK_EQUAL(obj_float, 1.2);

    msgpack::object obj_bool(true);
    BOOST_CHECK_EQUAL(obj_bool, msgpack::object(true));
    BOOST_CHECK_EQUAL(obj_bool, true);
}

BOOST_AUTO_TEST_CASE(construct_primitive)
{
    msgpack::object obj_nil;
    BOOST_CHECK_EQUAL(msgpack::type::NIL, obj_nil.type);

    msgpack::object obj_uint(1);
    BOOST_CHECK_EQUAL(msgpack::type::POSITIVE_INTEGER, obj_uint.type);
    BOOST_CHECK_EQUAL(1u, obj_uint.via.u64);

    msgpack::object obj_int(-1);
    BOOST_CHECK_EQUAL(msgpack::type::NEGATIVE_INTEGER, obj_int.type);
    BOOST_CHECK_EQUAL(-1, obj_int.via.i64);

    msgpack::object obj_float(1.2F);
    BOOST_CHECK_EQUAL(msgpack::type::FLOAT32, obj_float.type);
    BOOST_CHECK_EQUAL(1.2F, obj_float.via.f64);

    msgpack::object obj_double(1.2);
    BOOST_CHECK_EQUAL(msgpack::type::FLOAT64, obj_double.type);
    BOOST_CHECK_EQUAL(msgpack::type::FLOAT, obj_double.type);
    BOOST_CHECK_EQUAL(1.2, obj_double.via.f64);
#if defined(MSGPACK_USE_LEGACY_NAME_AS_FLOAT)
    BOOST_CHECK_EQUAL(msgpack::type::DOUBLE, obj_double.type);
    BOOST_CHECK_EQUAL(1.2, obj_double.via.dec);
#endif // MSGPACK_USE_LEGACY_NAME_AS_FLOAT

    msgpack::object obj_bool(true);
    BOOST_CHECK_EQUAL(msgpack::type::BOOLEAN, obj_bool.type);
    BOOST_CHECK_EQUAL(true, obj_bool.via.boolean);
}

BOOST_AUTO_TEST_CASE(construct_enum)
{
    msgpack::object obj(elem);
    BOOST_CHECK_EQUAL(msgpack::type::POSITIVE_INTEGER, obj.type);
    BOOST_CHECK_EQUAL(static_cast<uint64_t>(elem), obj.via.u64);
}

#if !defined(MSGPACK_USE_CPP03)

BOOST_AUTO_TEST_CASE(construct_enum_newstyle)
{
    msgpack::object obj(enum_test::elem);
    BOOST_CHECK_EQUAL(msgpack::type::POSITIVE_INTEGER, obj.type);
    BOOST_CHECK_EQUAL(elem, obj.via.u64);
}

#endif // !defined(MSGPACK_USE_CPP03)

BOOST_AUTO_TEST_CASE(construct_enum_outer)
{
    msgpack::object obj(outer_enum::elem);
    BOOST_CHECK_EQUAL(msgpack::type::POSITIVE_INTEGER, obj.type);
    BOOST_CHECK_EQUAL(static_cast<uint64_t>(elem), obj.via.u64);
}

#if !defined(MSGPACK_USE_CPP03)

BOOST_AUTO_TEST_CASE(construct_enum_outer_newstyle)
{
    msgpack::object obj(outer_enum::enum_test::elem);
    BOOST_CHECK_EQUAL(msgpack::type::POSITIVE_INTEGER, obj.type);
    BOOST_CHECK_EQUAL(elem, obj.via.u64);
}

BOOST_AUTO_TEST_CASE(construct_class_enum)
{
    msgpack::object obj(enum_class_test::elem);
    BOOST_CHECK_EQUAL(msgpack::type::POSITIVE_INTEGER, obj.type);
    BOOST_CHECK_EQUAL(elem, obj.via.u64);
}


BOOST_AUTO_TEST_CASE(construct_class_enum_outer)
{
    msgpack::object obj(outer_enum_class::enum_class_test::elem);
    BOOST_CHECK_EQUAL(msgpack::type::POSITIVE_INTEGER, obj.type);
    BOOST_CHECK_EQUAL(elem, obj.via.u64);
}

#endif // !defined(MSGPACK_USE_CPP03)

BOOST_AUTO_TEST_CASE(clone_int)
{
    int v = 0;
    msgpack::object obj(v);
    std::size_t sz1 = msgpack::aligned_zone_size(obj);
    msgpack::object_handle h = msgpack::clone(obj);
    BOOST_CHECK_EQUAL(h.get(), obj);
    BOOST_CHECK_EQUAL(sz1, msgpack::aligned_zone_size(h.get()));
    h = msgpack::clone(obj);
    BOOST_CHECK_EQUAL(h.get(), obj);
    BOOST_CHECK_EQUAL(sz1, msgpack::aligned_zone_size(h.get()));
}

BOOST_AUTO_TEST_CASE(clone_str)
{
    msgpack::object_handle oh;
    std::string v = "123456789";
    {
        msgpack::zone z;
        msgpack::object obj(v, z);
        std::size_t sz1 = msgpack::aligned_zone_size(obj);
        msgpack::object_handle h = msgpack::clone(obj);
        BOOST_CHECK_EQUAL(h.get(), obj);
        BOOST_CHECK_EQUAL(sz1, msgpack::aligned_zone_size(h.get()));
        h = msgpack::clone(obj);
        BOOST_CHECK_EQUAL(h.get(), obj);
        BOOST_CHECK_EQUAL(sz1, msgpack::aligned_zone_size(h.get()));
        oh = msgpack::move(h);
    }
    BOOST_CHECK_EQUAL(v, oh.get().as<std::string>());
}

BOOST_AUTO_TEST_CASE(clone_bin)
{
    msgpack::object_handle oh;
    std::vector<char> v;
    {
        msgpack::zone z;
        v.push_back('A');
        v.push_back('B');
        v.push_back('C');
        msgpack::object obj(v, z);
        std::size_t sz1 = msgpack::aligned_zone_size(obj);
        msgpack::object_handle h = msgpack::clone(obj);
        BOOST_CHECK_EQUAL(h.get(), obj);
        BOOST_CHECK_EQUAL(sz1, msgpack::aligned_zone_size(h.get()));
        h = msgpack::clone(obj);
        BOOST_CHECK_EQUAL(h.get(), obj);
        BOOST_CHECK_EQUAL(sz1, msgpack::aligned_zone_size(h.get()));
        oh = msgpack::move(h);
    }
    std::vector<char> v2 = oh.get().as<std::vector<char> >();
    BOOST_CHECK_EQUAL(v.size(), v2.size());
    BOOST_CHECK(equal(v.begin(), v.end(), v2.begin()));
}

BOOST_AUTO_TEST_CASE(clone_array)
{
    msgpack::object_handle oh;
    std::vector<int> v;
    {
        msgpack::zone z;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        msgpack::object obj(v, z);
        std::size_t sz1 = msgpack::aligned_zone_size(obj);
        msgpack::object_handle h = msgpack::clone(obj);
        BOOST_CHECK_EQUAL(h.get(), obj);
        BOOST_CHECK_EQUAL(sz1, msgpack::aligned_zone_size(h.get()));
        h = msgpack::clone(obj);
        BOOST_CHECK_EQUAL(h.get(), obj);
        BOOST_CHECK_EQUAL(sz1, msgpack::aligned_zone_size(h.get()));
        oh = msgpack::move(h);
    }
    std::vector<int> v2 = oh.get().as<std::vector<int> >();
    BOOST_CHECK_EQUAL(v.size(), v2.size());
    BOOST_CHECK(equal(v.begin(), v.end(), v2.begin()));
}

BOOST_AUTO_TEST_CASE(clone_map)
{
    msgpack::object_handle oh;
    std::map<int, std::string> v;
    {
        msgpack::zone z;
        v.insert(std::map<int, std::string>::value_type(1, "ABC"));
        v.insert(std::map<int, std::string>::value_type(2, "DEF"));
        v.insert(std::map<int, std::string>::value_type(3, "GHI"));
        msgpack::object obj(v, z);
        std::size_t sz1 = msgpack::aligned_zone_size(obj);
        msgpack::object_handle h = msgpack::clone(obj);
        BOOST_CHECK_EQUAL(h.get(), obj);
        BOOST_CHECK_EQUAL(sz1, msgpack::aligned_zone_size(h.get()));
        h = msgpack::clone(obj);
        BOOST_CHECK_EQUAL(h.get(), obj);
        BOOST_CHECK_EQUAL(sz1, msgpack::aligned_zone_size(h.get()));
        oh = msgpack::move(h);
    }
    std::map<int, std::string> v2 = oh.get().as<std::map<int, std::string> >();
    BOOST_CHECK_EQUAL(v.size(), v2.size());
    BOOST_CHECK(equal(v.begin(), v.end(), v2.begin()));
}

BOOST_AUTO_TEST_CASE(pack_float)
{
    msgpack::object obj(1.2F);
    std::stringstream ss1;
    msgpack::pack(ss1, obj);
    std::stringstream ss2;
    msgpack::pack(ss2, 1.2F);
    BOOST_CHECK_EQUAL(static_cast<size_t>(5), ss1.str().size());
    BOOST_CHECK_EQUAL(ss1.str(), ss2.str());
}

BOOST_AUTO_TEST_CASE(pack_double)
{
    msgpack::object obj(1.2);
    std::stringstream ss1;
    msgpack::pack(ss1, obj);
    std::stringstream ss2;
    msgpack::pack(ss2, 1.2);
    BOOST_CHECK_EQUAL(static_cast<size_t>(9), ss1.str().size());
    BOOST_CHECK_EQUAL(ss1.str(), ss2.str());
}

BOOST_AUTO_TEST_CASE(handle_operators)
{
    int i = 1;
    msgpack::object obj(i);
    msgpack::object_handle oh = msgpack::clone(obj);
    BOOST_CHECK_EQUAL(oh.get(), *oh);
    BOOST_CHECK_EQUAL(oh->as<int>(), oh.get().as<int>());
}

const unsigned int kLoop = 1000;
const unsigned int kElements = 100;

BOOST_AUTO_TEST_CASE(vector_char)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        std::vector<char> v1;
        v1.push_back(1);
        for (unsigned int i = 1; i < kElements; i++)
            v1.push_back(static_cast<char>(i));
        msgpack::object obj(v1);
        BOOST_CHECK(obj.as<std::vector<char> >() == v1);
        v1.front() = 42;
        // obj refer to v1
        BOOST_CHECK_EQUAL(obj.as<std::vector<char> >().front(), 42);
    }
}

BOOST_AUTO_TEST_CASE(vector_unsigned_char)
{
    if (!msgpack::is_same<uint8_t, unsigned char>::value) return;
    for (unsigned int k = 0; k < kLoop; k++) {
        std::vector<unsigned char> v1;
        v1.push_back(1);
        for (unsigned int i = 1; i < kElements; i++)
            v1.push_back(static_cast<unsigned char>(i));
        msgpack::object obj(v1);
        BOOST_CHECK(obj.as<std::vector<unsigned char> >() == v1);
        v1.front() = 42;
        // obj refer to v1
        BOOST_CHECK_EQUAL(obj.as<std::vector<unsigned char> >().front(), 42);
    }
}

BOOST_AUTO_TEST_CASE(raw_ref)
{
    std::string s = "abc";
    msgpack::type::raw_ref v(s.data(), static_cast<uint32_t>(s.size()));
    msgpack::zone z;
    msgpack::object obj(v);
    BOOST_CHECK(obj.as<msgpack::type::raw_ref>() == v);
    s[0] = 'd';
    BOOST_CHECK(obj.as<msgpack::type::raw_ref>() == v);
}

#if MSGPACK_CPP_VERSION >= 201703

BOOST_AUTO_TEST_CASE(array_char)
{
    typedef std::array<char, kElements> test_t;
    for (unsigned int k = 0; k < kLoop; k++) {
        test_t v1;
        v1[0] = 1;
        for (unsigned int i = 1; i < kElements; i++)
            v1[i] = static_cast<char>(rand());
        msgpack::object obj(v1);
        BOOST_CHECK(obj.as<test_t>() == v1);
        v1.front() = 42;
        // obj refer to v1
        BOOST_CHECK_EQUAL(obj.as<test_t>().front(), 42);
    }
}


BOOST_AUTO_TEST_CASE(array_unsigned_char)
{
    if (!msgpack::is_same<uint8_t, unsigned char>::value) return;
    typedef std::array<unsigned char, kElements> test_t;
    for (unsigned int k = 0; k < kLoop; k++) {
        test_t v1;
        v1[0] = 1;
        for (unsigned int i = 1; i < kElements; i++)
            v1[i] = static_cast<unsigned char>(rand());
        msgpack::object obj(v1);
        BOOST_CHECK(obj.as<test_t>() == v1);
        v1.front() = 42;
        // obj refer to v1
        BOOST_CHECK_EQUAL(obj.as<test_t>().front(), 42);
    }
}

#endif // MSGPACK_CPP_VERSION >= 201703
