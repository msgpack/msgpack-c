#include <msgpack.hpp>
#include <string>
#include <cmath>

#define BOOST_TEST_MODULE MSGPACK_USER_DEFINED
#include <boost/test/unit_test.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

const unsigned int kLoop = 1000;
const unsigned int kElements = 100;
const double kEPS = 1e-10;

// User-Defined Structures

class TestEnumMemberClass
{
public:
    TestEnumMemberClass()
        : t1(STATE_A), t2(STATE_B), t3(STATE_C) {}

    enum TestEnumType {
        STATE_INVALID = 0,
        STATE_A = 1,
        STATE_B = 2,
        STATE_C = 3
    };
    TestEnumType t1;
    TestEnumType t2;
    TestEnumType t3;

    MSGPACK_DEFINE(t1, t2, t3);
};

MSGPACK_ADD_ENUM(TestEnumMemberClass::TestEnumType);

class TestClass
{
public:
    TestClass() : i(0), s("kzk") {}
    int i;
    string s;
    MSGPACK_DEFINE(i, s);
};

BOOST_AUTO_TEST_CASE(simple_buffer_class)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        TestClass val1;
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        TestClass val2 = oh.get().as<TestClass>();
        BOOST_CHECK_EQUAL(val1.i, val2.i);
        BOOST_CHECK_EQUAL(val1.s, val2.s);
    }
}

class TestClass2
{
public:
    TestClass2() : i(0), s("kzk") {
        for (unsigned int i = 0; i < kElements; i++)
            v.push_back(rand());
    }
    int i;
    string s;
    vector<int> v;
    MSGPACK_DEFINE(i, s, v);
};

BOOST_AUTO_TEST_CASE(simple_buffer_class_old_to_new)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        TestClass val1;
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        TestClass2 val2 = oh.get().as<TestClass2>();
        BOOST_CHECK_EQUAL(val1.i, val2.i);
        BOOST_CHECK_EQUAL(val1.s, val2.s);
        BOOST_CHECK(!val2.s.empty());
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_class_new_to_old)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        TestClass2 val1;
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        TestClass val2 = oh.get().as<TestClass>();
        BOOST_CHECK_EQUAL(val1.i, val2.i);
        BOOST_CHECK_EQUAL(val1.s, val2.s);
        BOOST_CHECK(!val2.s.empty());
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_enum_member)
{
    TestEnumMemberClass val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    TestEnumMemberClass val2 = oh.get().as<TestEnumMemberClass>();
    BOOST_CHECK_EQUAL(val1.t1, val2.t1);
    BOOST_CHECK_EQUAL(val1.t2, val2.t2);
    BOOST_CHECK_EQUAL(val1.t3, val2.t3);
}

class TestUnionMemberClass
{
public:
    TestUnionMemberClass() {}
    TestUnionMemberClass(double f) {
        is_double = true;
        value.f = f;
    }
    TestUnionMemberClass(int i) {
        is_double = false;
        value.i = i;
    }

    union {
        double f;
        int i;
    } value;
    bool is_double;

    template <typename Packer>
    void msgpack_pack(Packer& pk) const
    {
        if (is_double)
            pk.pack(msgpack::type::tuple<bool, double>(true, value.f));
        else
            pk.pack(msgpack::type::tuple<bool, int>(false, value.i));
    }

    void msgpack_unpack(msgpack::object o)
    {
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
        msgpack::type::tuple<bool, msgpack::object> tuple;
        o.convert(tuple);

        is_double = tuple.get<0>();
        if (is_double)
            tuple.get<1>().convert<double>(value.f);
        else
            tuple.get<1>().convert<int>(value.i);
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
    }
};

BOOST_AUTO_TEST_CASE(simple_buffer_union_member)
{
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
    {
        // double
        TestUnionMemberClass val1(1.0);
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        TestUnionMemberClass val2 = oh.get().as<TestUnionMemberClass>();
        BOOST_CHECK_EQUAL(val1.is_double, val2.is_double);
        BOOST_CHECK(fabs(val1.value.f - val2.value.f) < kEPS);
    }
    {
        // int
        TestUnionMemberClass val1(1);
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        TestUnionMemberClass val2 = oh.get().as<TestUnionMemberClass>();
        BOOST_CHECK_EQUAL(val1.is_double, val2.is_double);
        BOOST_CHECK_EQUAL(val1.value.i, 1);
        BOOST_CHECK_EQUAL(val1.value.i, val2.value.i);
    }
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
}

// inheritance

// define

struct d_top {
    int t;
    MSGPACK_DEFINE(t);
};

struct d_mid1 : d_top {
    int m1;
    MSGPACK_DEFINE(MSGPACK_BASE(d_top), m1);
};

struct d_mid2 : d_top {
    int m2;
    MSGPACK_DEFINE(m2, MSGPACK_BASE(d_top));
};

struct d_bottom : d_mid1, d_mid2 {
    int b;
    MSGPACK_DEFINE(MSGPACK_BASE(d_mid1), MSGPACK_BASE(d_mid2), b);
};

BOOST_AUTO_TEST_CASE(inherit_define_non_virtual)
{
    d_bottom b;
    b.b = 1;
    b.m1 = 2;
    b.m2 = 3;
    b.d_mid1::t = 4;
    b.d_mid2::t = 5;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, b);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    d_bottom br = oh.get().as<d_bottom>();
    BOOST_CHECK_EQUAL(b.b, br.b);
    BOOST_CHECK_EQUAL(b.m1, br.m1);
    BOOST_CHECK_EQUAL(b.m2, br.m2);
    BOOST_CHECK_EQUAL(b.d_mid1::t, br.d_mid1::t);
    BOOST_CHECK_EQUAL(b.d_mid2::t, br.d_mid2::t);
}

struct v_d_top {
    int t;
    MSGPACK_DEFINE(t);
};

struct v_d_mid1 : virtual v_d_top {
    int m1;
    MSGPACK_DEFINE(m1);
};

struct v_d_mid2 : virtual v_d_top {
    int m2;
    MSGPACK_DEFINE(m2);
};

struct v_d_bottom : v_d_mid1, v_d_mid2 {
    int b;
    MSGPACK_DEFINE(MSGPACK_BASE(v_d_mid1), MSGPACK_BASE(v_d_mid2), MSGPACK_BASE(v_d_top), b);
};

BOOST_AUTO_TEST_CASE(inherit_define_virtual)
{
    v_d_bottom b;
    b.b = 1;
    b.m1 = 2;
    b.m2 = 3;
    b.t = 4;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, b);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    v_d_bottom br = oh.get().as<v_d_bottom>();
    BOOST_CHECK_EQUAL(b.b, br.b);
    BOOST_CHECK_EQUAL(b.m1, br.m1);
    BOOST_CHECK_EQUAL(b.m2, br.m2);
    BOOST_CHECK_EQUAL(b.t, br.t);
}

// define_array

struct da_top {
    int t;
    MSGPACK_DEFINE_ARRAY(t);
};

struct da_mid1 : da_top {
    int m1;
    MSGPACK_DEFINE_ARRAY(MSGPACK_BASE_ARRAY(da_top), m1);
};

struct da_mid2 : da_top {
    int m2;
    MSGPACK_DEFINE_ARRAY(m2, MSGPACK_BASE_ARRAY(da_top));
};

struct da_bottom : da_mid1, da_mid2 {
    int b;
    MSGPACK_DEFINE_ARRAY(MSGPACK_BASE_ARRAY(da_mid1), MSGPACK_BASE_ARRAY(da_mid2), b);
};

BOOST_AUTO_TEST_CASE(inherit_define_array_non_virtual)
{
    da_bottom b;
    b.b = 1;
    b.m1 = 2;
    b.m2 = 3;
    b.da_mid1::t = 4;
    b.da_mid2::t = 5;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, b);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    da_bottom br = oh.get().as<da_bottom>();
    BOOST_CHECK_EQUAL(b.b, br.b);
    BOOST_CHECK_EQUAL(b.m1, br.m1);
    BOOST_CHECK_EQUAL(b.m2, br.m2);
    BOOST_CHECK_EQUAL(b.da_mid1::t, br.da_mid1::t);
    BOOST_CHECK_EQUAL(b.da_mid2::t, br.da_mid2::t);
}

struct v_da_top {
    int t;
    MSGPACK_DEFINE_ARRAY(t);
};

struct v_da_mid1 : virtual v_da_top {
    int m1;
    MSGPACK_DEFINE_ARRAY(m1);
};

struct v_da_mid2 : virtual v_da_top {
    int m2;
    MSGPACK_DEFINE_ARRAY(m2);
};

struct v_da_bottom : v_da_mid1, v_da_mid2 {
    int b;
    MSGPACK_DEFINE_ARRAY(MSGPACK_BASE_ARRAY(v_da_mid1), MSGPACK_BASE_ARRAY(v_da_mid2), MSGPACK_BASE_ARRAY(v_da_top), b);
};

BOOST_AUTO_TEST_CASE(inherit_define_array_virtual)
{
    v_da_bottom b;
    b.b = 1;
    b.m1 = 2;
    b.m2 = 3;
    b.t = 4;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, b);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    v_da_bottom br = oh.get().as<v_da_bottom>();
    BOOST_CHECK_EQUAL(b.b, br.b);
    BOOST_CHECK_EQUAL(b.m1, br.m1);
    BOOST_CHECK_EQUAL(b.m2, br.m2);
    BOOST_CHECK_EQUAL(b.t, br.t);
}

// define_map

struct dm_top {
    int t;
    MSGPACK_DEFINE_MAP(t);
};

struct dm_mid1 : dm_top {
    int m1;
    MSGPACK_DEFINE_MAP(MSGPACK_BASE_MAP(dm_top), m1);
};

struct dm_mid2 : dm_top {
    int m2;
    MSGPACK_DEFINE_MAP(m2, MSGPACK_BASE_MAP(dm_top));
};

struct dm_bottom : dm_mid1, dm_mid2 {
    int b;
    MSGPACK_DEFINE_MAP(MSGPACK_BASE_MAP(dm_mid1), MSGPACK_BASE_MAP(dm_mid2), b);
};

BOOST_AUTO_TEST_CASE(inherit_define_map_non_virtual)
{
    dm_bottom b;
    b.b = 1;
    b.m1 = 2;
    b.m2 = 3;
    b.dm_mid1::t = 4;
    b.dm_mid2::t = 5;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, b);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
    dm_bottom br = oh.get().as<dm_bottom>();
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
    BOOST_CHECK_EQUAL(b.b, br.b);
    BOOST_CHECK_EQUAL(b.m1, br.m1);
    BOOST_CHECK_EQUAL(b.m2, br.m2);
    BOOST_CHECK_EQUAL(b.dm_mid1::t, br.dm_mid1::t);
    BOOST_CHECK_EQUAL(b.dm_mid2::t, br.dm_mid2::t);
}

struct v_dm_top {
    int t;
    MSGPACK_DEFINE_MAP(t);
};

struct v_dm_mid1 : virtual v_dm_top {
    int m1;
    MSGPACK_DEFINE_MAP(m1);
};

struct v_dm_mid2 : virtual v_dm_top {
    int m2;
    MSGPACK_DEFINE_MAP(m2);
};

struct v_dm_bottom : v_dm_mid1, v_dm_mid2 {
    int b;
    MSGPACK_DEFINE_MAP(MSGPACK_BASE_MAP(v_dm_mid1), MSGPACK_BASE_MAP(v_dm_mid2), MSGPACK_BASE_MAP(v_dm_top), b);
};

BOOST_AUTO_TEST_CASE(inherit_define_map_virtual)
{
    v_dm_bottom b;
    b.b = 1;
    b.m1 = 2;
    b.m2 = 3;
    b.t = 4;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, b);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    v_dm_bottom br = oh.get().as<v_dm_bottom>();
    BOOST_CHECK_EQUAL(b.b, br.b);
    BOOST_CHECK_EQUAL(b.m1, br.m1);
    BOOST_CHECK_EQUAL(b.m2, br.m2);
    BOOST_CHECK_EQUAL(b.t, br.t);
}

// map migration

struct s_v1 {
    int i;
    std::string s;
    s_v1():i(42), s("foo") {}
    MSGPACK_DEFINE_MAP(i, s);
};

struct s_v2 {
    char c; // new member variable
    std::string s;
    int i;
    s_v2():c('A'), s("bar"), i(77) {}
    MSGPACK_DEFINE_MAP(c, s, i); // variable added, order changed
};

BOOST_AUTO_TEST_CASE(migration_order_number_changed)
{
    s_v1 v1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, v1);

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    s_v2 v2 = oh.get().as<s_v2>();

    BOOST_CHECK_EQUAL(v2.c, 'A');
    BOOST_CHECK_EQUAL(v2.s, "foo"); // from v1
    BOOST_CHECK_EQUAL(v2.i, 42);    // from v1
}

// non intrusive with operator <<

class test_non_intrusive {
public:
    const std::string& name() const { return m_name; }
    void set_name(const std::string& name) { m_name = name; }
private:
    std::string m_name;
};

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
namespace adaptor {

template<>
struct convert<test_non_intrusive>
{
    msgpack::object const& operator()(
        msgpack::object const& o,
        test_non_intrusive& t) const {
        t.set_name(o.as<std::string>());
        return o;
    }
};

template<>
struct pack<test_non_intrusive>
{
    template <typename Stream>
    msgpack::packer<Stream>& operator()(
        msgpack::packer<Stream>& p,
        test_non_intrusive const& t) const {
        p.pack(t.name());
        return p;
    }
};

template <>
struct object_with_zone<test_non_intrusive>
{
    void operator()(
        msgpack::object::with_zone& o,
        const test_non_intrusive& t) const {
        o << t.name();
    }
};

} // namespace adaptor
} // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // namespace msgpack

BOOST_AUTO_TEST_CASE(test_non_intrusive_)
{
    test_non_intrusive t1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, t1);

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    test_non_intrusive t2 = oh.get().as<test_non_intrusive>();

    BOOST_CHECK_EQUAL(t1.name(), t2.name());
}

struct nvp_base {
    int a;
    int b;
    MSGPACK_DEFINE_MAP(MSGPACK_NVP("aaa", a), b);
};

struct nvp_derived : nvp_base {
    int c;
    std::string d;
    MSGPACK_DEFINE_MAP(MSGPACK_NVP("ccc", c), MSGPACK_NVP("base", MSGPACK_BASE(nvp_base)), MSGPACK_NVP("ddd", d));
};

BOOST_AUTO_TEST_CASE(nvp_combination)
{
    msgpack::sbuffer sbuf;
    nvp_derived d1;
    d1.a = 1;
    d1.b = 2;
    d1.c = 3;
    d1.d = "ABC";

    msgpack::pack(sbuf, d1);
    msgpack::object_handle oh = msgpack::unpack(sbuf.data(), sbuf.size());
    msgpack::object obj = oh.get();

    BOOST_CHECK_EQUAL(obj.via.map.size, static_cast<size_t>(3));

    BOOST_CHECK_EQUAL(std::string(obj.via.map.ptr[0].key.via.str.ptr, obj.via.map.ptr[0].key.via.str.size), "ccc");
    BOOST_CHECK_EQUAL(obj.via.map.ptr[0].val.via.i64, 3);

    BOOST_CHECK_EQUAL(std::string(obj.via.map.ptr[1].key.via.str.ptr, obj.via.map.ptr[1].key.via.str.size), "base");
    BOOST_CHECK_EQUAL(obj.via.map.ptr[1].val.via.map.size, static_cast<size_t>(2));
    BOOST_CHECK_EQUAL(
        std::string(
            obj.via.map.ptr[1].val.via.map.ptr[0].key.via.str.ptr,
            obj.via.map.ptr[1].val.via.map.ptr[0].key.via.str.size),
        "aaa"
    );
    BOOST_CHECK_EQUAL(obj.via.map.ptr[1].val.via.map.ptr[0].val.via.i64, 1);
    BOOST_CHECK_EQUAL(
        std::string(
            obj.via.map.ptr[1].val.via.map.ptr[1].key.via.str.ptr,
            obj.via.map.ptr[1].val.via.map.ptr[1].key.via.str.size),
        "b"
    );
    BOOST_CHECK_EQUAL(obj.via.map.ptr[1].val.via.map.ptr[1].val.via.i64, 2);

    BOOST_CHECK_EQUAL(std::string(obj.via.map.ptr[2].key.via.str.ptr, obj.via.map.ptr[2].key.via.str.size), "ddd");
    BOOST_CHECK_EQUAL(std::string(obj.via.map.ptr[2].val.via.str.ptr, obj.via.map.ptr[2].val.via.str.size), "ABC");

    nvp_derived d2 = obj.as<nvp_derived>();
    BOOST_CHECK_EQUAL(d2.a, 1);
    BOOST_CHECK_EQUAL(d2.b, 2);
    BOOST_CHECK_EQUAL(d2.c, 3);
    BOOST_CHECK_EQUAL(d2.d, "ABC");
}

struct invalid_key {
    int val;
    MSGPACK_DEFINE_MAP(val);
};

BOOST_AUTO_TEST_CASE(test_invalid_key_type)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> pk(sbuf);
    pk.pack_map(1);
    pk.pack_int(42);
    pk.pack_int(43);
    msgpack::object_handle oh = msgpack::unpack(sbuf.data(), sbuf.size());
    try {
        oh.get().as<invalid_key>();
        BOOST_CHECK(false);
    }
    catch (msgpack::type_error const&) {
        BOOST_CHECK(true);
    }
    catch (...) {
        BOOST_CHECK(false);
    }
}
