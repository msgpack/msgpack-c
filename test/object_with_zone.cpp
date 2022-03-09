#include <msgpack.hpp>

#define BOOST_TEST_MODULE object_with_zone
#include <boost/test/unit_test.hpp>

#include <cmath>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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


const unsigned int kLoop = 1000;
const unsigned int kElements = 100;
const double kEPS = 1e-10;

// bool
BOOST_AUTO_TEST_CASE(bool_)
{
    bool v = true;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK_EQUAL(obj.as<bool>(), v);
    v = false;
    BOOST_CHECK(obj.as<bool>());
}

// char
BOOST_AUTO_TEST_CASE(char_)
{
    char v = 1;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK_EQUAL(obj.as<char>(), v);
    v = 2;
    BOOST_CHECK_EQUAL(obj.as<char>(), 1);
}

// signed integer family
BOOST_AUTO_TEST_CASE(signed_char)
{
    signed char v = -1;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK_EQUAL(obj.as<signed char>(), v);
    v = -2;
    BOOST_CHECK_EQUAL(obj.as<signed char>(), -1);
}

BOOST_AUTO_TEST_CASE(signed_short)
{
    signed short v = -1;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK_EQUAL(obj.as<signed short>(), v);
    v = -2;
    BOOST_CHECK_EQUAL(obj.as<signed short>(), -1);
}

BOOST_AUTO_TEST_CASE(signed_int)
{
    signed int v = -1;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK_EQUAL(obj.as<signed int>(), v);
    v = -2;
    BOOST_CHECK_EQUAL(obj.as<signed int>(), -1);
}

BOOST_AUTO_TEST_CASE(signed_long)
{
    signed long v = -1;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK_EQUAL(obj.as<signed long>(), v);
    v = -2;
    BOOST_CHECK_EQUAL(obj.as<signed long>(), -1);
}

BOOST_AUTO_TEST_CASE(signed_long_long)
{
    signed long long v = -1;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK_EQUAL(obj.as<signed long long>(), v);
    v = -2;
    BOOST_CHECK_EQUAL(obj.as<signed long long>(), -1);
}

// unsigned integer family
BOOST_AUTO_TEST_CASE(unsigned_char)
{
    unsigned char v = 1;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK_EQUAL(obj.as<unsigned char>(), v);
    v = 2;
    BOOST_CHECK_EQUAL(obj.as<unsigned char>(), 1);
}

BOOST_AUTO_TEST_CASE(unsigned_short)
{
    unsigned short v = 1;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK_EQUAL(obj.as<unsigned short>(), v);
    v = 2;
    BOOST_CHECK_EQUAL(obj.as<unsigned short>(), 1);
}

BOOST_AUTO_TEST_CASE(unsigned_int)
{
    unsigned int v = 1;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK_EQUAL(obj.as<unsigned int>(), v);
    v = 2;
    BOOST_CHECK_EQUAL(obj.as<unsigned int>(), 1u);
}

BOOST_AUTO_TEST_CASE(unsigned_long)
{
    unsigned long v = 1;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK_EQUAL(obj.as<unsigned long>(), v);
    v = 2;
    BOOST_CHECK_EQUAL(obj.as<unsigned long>(), 1u);
}

BOOST_AUTO_TEST_CASE(unsigned_long_long)
{
    unsigned long long v = 1;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK_EQUAL(obj.as<unsigned long long>(), v);
    v = 2;
    BOOST_CHECK_EQUAL(obj.as<unsigned long long>(), 1u);
}

// float
BOOST_AUTO_TEST_CASE(float_)
{
    float v = 1.23f;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK_EQUAL(obj.type, msgpack::type::FLOAT32);
    BOOST_CHECK(fabs(obj.as<float>() - v) <= kEPS);
    v = 4.56f;
    BOOST_CHECK(fabs(obj.as<float>() - static_cast<float>(1.23)) <= kEPS);
}

// double
BOOST_AUTO_TEST_CASE(double_)
{
    double v = 1.23;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK_EQUAL(obj.type, msgpack::type::FLOAT64);
    BOOST_CHECK(fabs(obj.as<double>() - v) <= kEPS);
    v = 4.56;
    BOOST_CHECK(fabs(obj.as<double>() - 1.23) <= kEPS);
}

// vector

BOOST_AUTO_TEST_CASE(vector)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        std::vector<int> v1;
        v1.push_back(1);
        for (unsigned int i = 1; i < kElements; i++)
            v1.push_back(static_cast<int>(i));
        msgpack::zone z;
        msgpack::object obj(v1, z);
        BOOST_CHECK(obj.as<std::vector<int> >() == v1);
        v1.front() = 42;
        BOOST_CHECK_EQUAL(obj.as<std::vector<int> >().front(), 1);
    }
}

// vector_char
BOOST_AUTO_TEST_CASE(vector_char)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        std::vector<char> v1;
        v1.push_back(1);
        for (unsigned int i = 1; i < kElements; i++)
            v1.push_back(static_cast<char>(i));
        msgpack::zone z;
        msgpack::object obj(v1, z);
        BOOST_CHECK(obj.as<std::vector<char> >() == v1);
        v1.front() = 42;
        BOOST_CHECK_EQUAL(obj.as<std::vector<char> >().front(), 1);
    }
}



// vector_unsgined_char
BOOST_AUTO_TEST_CASE(vector_unsigned_char)
{
    if (!msgpack::is_same<uint8_t, unsigned char>::value) return;
    for (unsigned int k = 0; k < kLoop; k++) {
        std::vector<unsigned char> v1;
        v1.push_back(1);
        for (unsigned int i = 1; i < kElements; i++)
            v1.push_back(static_cast<unsigned char>(i));
        msgpack::zone z;
        msgpack::object obj(v1, z);
        BOOST_CHECK(obj.as<std::vector<unsigned char> >() == v1);
        v1.front() = 42;
        BOOST_CHECK_EQUAL(obj.as<std::vector<unsigned char> >().front(), 1);
    }
}

// list
BOOST_AUTO_TEST_CASE(list)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        std::list<int> v1;
        v1.push_back(1);
        for (unsigned int i = 1; i < kElements; i++)
            v1.push_back(static_cast<int>(i));
        msgpack::zone z;
        msgpack::object obj(v1, z);
        BOOST_CHECK(obj.as<std::list<int> >() == v1);
        v1.front() = 42;
        BOOST_CHECK_EQUAL(obj.as<std::list<int> >().front(), 1);
    }
}

// deque
BOOST_AUTO_TEST_CASE(deque)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        std::deque<int> v1;
        v1.push_back(1);
        for (unsigned int i = 1; i < kElements; i++)
            v1.push_back(static_cast<int>(i));
        msgpack::zone z;
        msgpack::object obj(v1, z);
        BOOST_CHECK(obj.as<std::deque<int> >() == v1);
        v1.front() = 42;
        BOOST_CHECK_EQUAL(obj.as<std::deque<int> >().front(), 1);
    }
}

// string
BOOST_AUTO_TEST_CASE(string)
{
    std::string v = "abc";
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK_EQUAL(obj.as<std::string>(), v);
    v[0] = 'd';
    BOOST_CHECK_EQUAL(obj.as<std::string>()[0], 'a');
}

#if MSGPACK_DEFAULT_API_VERSION == 1

BOOST_AUTO_TEST_CASE(without_zone_string)
{
    std::string v = "abc";
    msgpack::zone z;
    msgpack::object obj(v);
    BOOST_CHECK_EQUAL(obj.as<std::string>(), v);
    v[0] = 'd';
    BOOST_CHECK_EQUAL(obj.as<std::string>()[0], 'd');
}

#endif // MSGPACK_DEFAULT_API_VERSION == 1

// wstring
BOOST_AUTO_TEST_CASE(wstring)
{
    std::wstring v = L"abc";
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK(obj.as<std::wstring>() == v);
    v[0] = 'd';
    BOOST_CHECK(obj.as<std::wstring>()[0] == L'a');
}

// char*
BOOST_AUTO_TEST_CASE(char_ptr)
{
    char v[] = "abc";
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK_EQUAL(obj.as<std::string>(), std::string(v));
    v[0] = 'd';
    BOOST_CHECK_EQUAL(obj.as<std::string>()[0], 'a');
}

// complex
BOOST_AUTO_TEST_CASE(complex_float)
{
    typedef std::complex<float> test_t;
    test_t v(1.23F, 4.56F);
    msgpack::zone z;
    msgpack::object obj(v, z);
    float r = v.real();
    BOOST_CHECK(fabs(obj.as<test_t>().real() - v.real()) <= kEPS);
    BOOST_CHECK(fabs(obj.as<test_t>().imag() - v.imag()) <= kEPS);
    v.real(7.89F);
    BOOST_CHECK(fabs(obj.as<test_t>().real() - r) <= kEPS);
}

BOOST_AUTO_TEST_CASE(complex_double)
{
    typedef std::complex<double> test_t;
    test_t v(1.23, 4.56);
    msgpack::zone z;
    msgpack::object obj(v, z);
    double r = v.real();
    BOOST_CHECK(fabs(obj.as<test_t>().real() - v.real()) <= kEPS);
    BOOST_CHECK(fabs(obj.as<test_t>().imag() - v.imag()) <= kEPS);
    v.real(7.89);
    BOOST_CHECK(fabs(obj.as<test_t>().real() - r) <= kEPS);
}

#if MSGPACK_DEFAULT_API_VERSION == 1

BOOST_AUTO_TEST_CASE(without_zone_char_ptr)
{
    char v[] = "abc";
    msgpack::zone z;
    msgpack::object obj(v);
    BOOST_CHECK_EQUAL(obj.as<std::string>(), std::string(v));
    v[0] = 'd';
    BOOST_CHECK_EQUAL(obj.as<std::string>()[0], 'd');
}

#endif // MSGPACK_DEFAULT_API_VERSION == 1

// raw_ref
BOOST_AUTO_TEST_CASE(raw_ref)
{
    std::string s = "abc";
    msgpack::type::raw_ref v(s.data(), static_cast<uint32_t>(s.size()));
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK(obj.as<msgpack::type::raw_ref>() == v);
    s[0] = 'd';
    // even if with_zone, not copied due to raw_ref
    // Basically, the combination raw_ref and object::wit_zone
    // is meaningless.
    BOOST_CHECK(obj.as<msgpack::type::raw_ref>() == v);
}

// pair
BOOST_AUTO_TEST_CASE(pair)
{
    typedef std::pair<int, std::string> test_t;
    test_t v(1, "abc");
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK(obj.as<test_t>() == v);
    v.first = 42;
    BOOST_CHECK_EQUAL(obj.as<test_t>().first, 1);
}

// set
BOOST_AUTO_TEST_CASE(set)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        std::set<int> v1;
        for (unsigned int i = 0; i < kElements; i++)
            v1.insert(static_cast<int>(i));
        msgpack::zone z;
        msgpack::object obj(v1, z);
        BOOST_CHECK(obj.as<std::set<int> >() == v1);
    }
}

// multiset
BOOST_AUTO_TEST_CASE(multiset)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        std::multiset<int> v1;
        for (unsigned int i = 0; i < kElements; i++)
            v1.insert(i % (kElements / 2));
        msgpack::zone z;
        msgpack::object obj(v1, z);
        BOOST_CHECK(obj.as<std::multiset<int> >() == v1);
    }
}

// map
BOOST_AUTO_TEST_CASE(map)
{
    typedef std::map<int, int> test_t;
    for (unsigned int k = 0; k < kLoop; k++) {
        test_t v1;
        for (unsigned int i = 0; i < kElements; i++)
            v1.insert(std::make_pair(i, i*2));
        msgpack::zone z;
        msgpack::object obj(v1, z);
        BOOST_CHECK(obj.as<test_t >() == v1);
    }
}

// multimap
BOOST_AUTO_TEST_CASE(multimap)
{
    typedef std::multimap<int, int> test_t;
    for (unsigned int k = 0; k < kLoop; k++) {
        test_t v1;
        for (unsigned int i = 0; i < kElements; i++)
            v1.insert(std::make_pair(i % (kElements / 2), i*2));
        msgpack::zone z;
        msgpack::object obj(v1, z);
        BOOST_CHECK(obj.as<test_t >() == v1);
    }
}

// msgpack_tuple
BOOST_AUTO_TEST_CASE(msgpack_tuple)
{
    typedef msgpack::type::tuple<int, std::string, bool> test_t;
    test_t v(1, "abc", true);
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK_EQUAL(msgpack::type::get<0>(obj.as<test_t>()), 1);
    BOOST_CHECK_EQUAL(msgpack::type::get<1>(obj.as<test_t>()), "abc");
    BOOST_CHECK_EQUAL(msgpack::type::get<2>(obj.as<test_t>()), true);
    msgpack::type::get<0>(v) = 42;
    BOOST_CHECK_EQUAL(msgpack::type::get<0>(obj.as<test_t>()), 1);
}

BOOST_AUTO_TEST_CASE(msgpack_tuple_empty)
{
    typedef msgpack::type::tuple<> test_t;
    test_t v;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK_EQUAL(obj.via.array.size, 0u);
}

// TR1

#ifdef MSGPACK_HAS_STD_TR1_UNORDERED_MAP
#include <tr1/unordered_map>
#include "msgpack/adaptor/tr1/unordered_map.hpp"
BOOST_AUTO_TEST_CASE(tr1_unordered_map)
{
    typedef std::tr1::unordered_map<int, int> test_t;
    for (unsigned int k = 0; k < kLoop; k++) {
        test_t v1;
        for (unsigned int i = 0; i < kElements; i++)
            v1[rand()] = rand();
        msgpack::zone z;
        msgpack::object obj(v1, z);
        test_t v2 = obj.as<test_t>();
        BOOST_CHECK_EQUAL(v1.size(), v2.size());
        test_t::const_iterator it;
        for (it = v1.begin(); it != v1.end(); ++it) {
            BOOST_CHECK(v2.find(it->first) != v2.end());
            BOOST_CHECK_EQUAL(it->second, v2.find(it->first)->second);
        }
    }
}


BOOST_AUTO_TEST_CASE(tr1_unordered_multimap)
{
    typedef std::tr1::unordered_multimap<int, int> test_t;
    for (unsigned int k = 0; k < kLoop; k++) {
        test_t v1;
        for (unsigned int i = 0; i < kElements; i++) {
            int i1 = rand();
            v1.insert(std::make_pair(i1, rand()));
            v1.insert(std::make_pair(i1, rand()));
        }
        msgpack::zone z;
        msgpack::object obj(v1, z);
        test_t v2 = obj.as<test_t>();
        std::vector<std::pair<int, int> > vec1, vec2;
        std::tr1::unordered_multimap<int, int>::const_iterator it;
        for (it = v1.begin(); it != v1.end(); ++it)
            vec1.push_back(std::make_pair(it->first, it->second));
        for (it = v2.begin(); it != v2.end(); ++it)
            vec2.push_back(std::make_pair(it->first, it->second));
        BOOST_CHECK_EQUAL(v1.size(), v2.size());
        BOOST_CHECK_EQUAL(vec1.size(), vec2.size());
        std::sort(vec1.begin(), vec1.end());
        std::sort(vec2.begin(), vec2.end());
        BOOST_CHECK(vec1 == vec2);
    }
}
#endif

#ifdef MSGPACK_HAS_STD_TR1_UNORDERED_SET
#include <tr1/unordered_set>
#include "msgpack/adaptor/tr1/unordered_set.hpp"
BOOST_AUTO_TEST_CASE(tr1_unordered_set)
{
    typedef std::tr1::unordered_set<int> test_t;
    for (unsigned int k = 0; k < kLoop; k++) {
        test_t v1;
        for (unsigned int i = 0; i < kElements; i++)
            v1.insert(rand());
        msgpack::zone z;
        msgpack::object obj(v1, z);
        test_t v2 = obj.as<test_t>();
        BOOST_CHECK_EQUAL(v1.size(), v2.size());
        std::tr1::unordered_set<int>::const_iterator it;
        for (it = v1.begin(); it != v1.end(); ++it)
            BOOST_CHECK(v2.find(*it) != v2.end());
    }
}

BOOST_AUTO_TEST_CASE(tr1_unordered_multiset)
{
    typedef std::tr1::unordered_set<int> test_t;
    for (unsigned int k = 0; k < kLoop; k++) {
        test_t v1;
        for (unsigned int i = 0; i < kElements; i++) {
            int i1 = rand();
            v1.insert(i1);
            v1.insert(i1);
        }
        msgpack::zone z;
        msgpack::object obj(v1, z);
        test_t v2 = obj.as<test_t>();
        std::vector<int> vec1, vec2;
        std::tr1::unordered_multiset<int>::const_iterator it;
        for (it = v1.begin(); it != v1.end(); ++it)
            vec1.push_back(*it);
        for (it = v2.begin(); it != v2.end(); ++it)
            vec2.push_back(*it);
        BOOST_CHECK_EQUAL(v1.size(), v2.size());
        BOOST_CHECK_EQUAL(vec1.size(), vec2.size());
        std::sort(vec1.begin(), vec1.end());
        std::sort(vec2.begin(), vec2.end());
        BOOST_CHECK(vec1 == vec2);
    }
}
#endif

#ifdef MSGPACK_HAS_STD_UNORDERED_MAP
#include <unordered_map>
#include "msgpack/adaptor/tr1/unordered_map.hpp"
BOOST_AUTO_TEST_CASE(unordered_map)
{
    typedef std::unordered_map<int, int> test_t;
    for (unsigned int k = 0; k < kLoop; k++) {
        test_t v1;
        for (unsigned int i = 0; i < kElements; i++)
            v1[rand()] = rand();
        msgpack::zone z;
        msgpack::object obj(v1, z);
        test_t v2 = obj.as<test_t>();
        BOOST_CHECK_EQUAL(v1.size(), v2.size());
        test_t::const_iterator it;
        for (it = v1.begin(); it != v1.end(); ++it) {
            BOOST_CHECK(v2.find(it->first) != v2.end());
            BOOST_CHECK_EQUAL(it->second, v2.find(it->first)->second);
        }
    }
}

BOOST_AUTO_TEST_CASE(unordered_multimap)
{
   typedef std::unordered_multimap<int, int> test_t;
    for (unsigned int k = 0; k < kLoop; k++) {
        test_t v1;
        for (unsigned int i = 0; i < kElements; i++) {
            int i1 = rand();
            v1.insert(std::make_pair(i1, rand()));
            v1.insert(std::make_pair(i1, rand()));
        }
        msgpack::zone z;
        msgpack::object obj(v1, z);
        test_t v2 = obj.as<test_t>();
        std::vector<std::pair<int, int> > vec1, vec2;
        std::unordered_multimap<int, int>::const_iterator it;
        for (it = v1.begin(); it != v1.end(); ++it)
            vec1.push_back(std::make_pair(it->first, it->second));
        for (it = v2.begin(); it != v2.end(); ++it)
            vec2.push_back(std::make_pair(it->first, it->second));
        BOOST_CHECK_EQUAL(v1.size(), v2.size());
        BOOST_CHECK_EQUAL(vec1.size(), vec2.size());
        sort(vec1.begin(), vec1.end());
        sort(vec2.begin(), vec2.end());
        BOOST_CHECK(vec1 == vec2);
    }
}
#endif

#ifdef MSGPACK_HAS_STD_UNORDERED_SET
#include <unordered_set>
#include "msgpack/adaptor/tr1/unordered_set.hpp"
BOOST_AUTO_TEST_CASE(unordered_set)
{
    typedef std::unordered_set<int> test_t;
    for (unsigned int k = 0; k < kLoop; k++) {
        test_t v1;
        for (unsigned int i = 0; i < kElements; i++)
            v1.insert(rand());
        msgpack::zone z;
        msgpack::object obj(v1, z);
        test_t v2 = obj.as<test_t>();
        BOOST_CHECK_EQUAL(v1.size(), v2.size());
        std::unordered_set<int>::const_iterator it;
        for (it = v1.begin(); it != v1.end(); ++it)
            BOOST_CHECK(v2.find(*it) != v2.end());
    }
}

BOOST_AUTO_TEST_CASE(unordered_multiset)
{
    typedef std::unordered_set<int> test_t;
    for (unsigned int k = 0; k < kLoop; k++) {
        test_t v1;
        for (unsigned int i = 0; i < kElements; i++) {
            int i1 = rand();
            v1.insert(i1);
            v1.insert(i1);
        }
        msgpack::zone z;
        msgpack::object obj(v1, z);
        test_t v2 = obj.as<test_t>();
        std::vector<int> vec1, vec2;
        std::unordered_multiset<int>::const_iterator it;
        for (it = v1.begin(); it != v1.end(); ++it)
            vec1.push_back(*it);
        for (it = v2.begin(); it != v2.end(); ++it)
            vec2.push_back(*it);
        BOOST_CHECK_EQUAL(v1.size(), v2.size());
        BOOST_CHECK_EQUAL(vec1.size(), vec2.size());
        std::sort(vec1.begin(), vec1.end());
        std::sort(vec2.begin(), vec2.end());
        BOOST_CHECK(vec1 == vec2);
    }
}
#endif

// User defined class
class TestClass
{
public:
    TestClass() : i(0), s("kzk") {}
    int i;
    std::string s;
    MSGPACK_DEFINE(i, s);
};

BOOST_AUTO_TEST_CASE(user_defined)
{
    TestClass v1;
    msgpack::zone z;
    msgpack::object obj(v1, z);
    TestClass v2 = obj.as<TestClass>();
    BOOST_CHECK_EQUAL(v1.i, v2.i);
    BOOST_CHECK_EQUAL(v1.s, v2.s);
}

BOOST_AUTO_TEST_CASE(construct_enum)
{
    msgpack::zone z;
    msgpack::object obj(elem, z);
    BOOST_CHECK_EQUAL(msgpack::type::POSITIVE_INTEGER, obj.type);
    BOOST_CHECK_EQUAL(static_cast<uint64_t>(elem), obj.via.u64);
}

#if !defined(MSGPACK_USE_CPP03)

BOOST_AUTO_TEST_CASE(construct_enum_newstyle)
{
    msgpack::zone z;
    msgpack::object obj(enum_test::elem, z);
    BOOST_CHECK_EQUAL(msgpack::type::POSITIVE_INTEGER, obj.type);
    BOOST_CHECK_EQUAL(elem, obj.via.u64);
}

#endif // !defined(MSGPACK_USE_CPP03)

BOOST_AUTO_TEST_CASE(construct_enum_outer)
{
    msgpack::zone z;
    msgpack::object obj(outer_enum::elem, z);
    BOOST_CHECK_EQUAL(msgpack::type::POSITIVE_INTEGER, obj.type);
    BOOST_CHECK_EQUAL(static_cast<uint64_t>(elem), obj.via.u64);
}

// User defined inheriting classes
struct top {
    int t;
    MSGPACK_DEFINE(t);
};

struct mid1 : top {
    int m1;
    MSGPACK_DEFINE(MSGPACK_BASE(top), m1);
};

struct mid2 : top {
    int m2;
    MSGPACK_DEFINE(m2, MSGPACK_BASE(top));
};

struct bottom : mid1, mid2 {
    int b;
    MSGPACK_DEFINE(MSGPACK_BASE(mid1), MSGPACK_BASE(mid2), b);
};

BOOST_AUTO_TEST_CASE(user_defined_non_virtual)
{
    bottom b;
    b.b = 1;
    b.m1 = 2;
    b.m2 = 3;
    b.mid1::t = 4;
    b.mid2::t = 5;

    msgpack::zone z;
    msgpack::object obj(b, z);
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
    bottom br = obj.as<bottom>();
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
    BOOST_CHECK_EQUAL(b.b, br.b);
    BOOST_CHECK_EQUAL(b.m1, br.m1);
    BOOST_CHECK_EQUAL(b.m2, br.m2);
    BOOST_CHECK_EQUAL(b.mid1::t, br.mid1::t);
    BOOST_CHECK_EQUAL(b.mid2::t, br.mid2::t);
}

struct v_top {
    int t;
    MSGPACK_DEFINE(t);
};

struct v_mid1 : virtual v_top {
    int m1;
    MSGPACK_DEFINE(m1);
};

struct v_mid2 : virtual v_top {
    int m2;
    MSGPACK_DEFINE(m2);
};

struct v_bottom : v_mid1, v_mid2 {
    int b;
    MSGPACK_DEFINE(MSGPACK_BASE(v_mid1), MSGPACK_BASE(v_mid2), MSGPACK_BASE(v_top), b);
};

BOOST_AUTO_TEST_CASE(user_defined_virtual)
{
    v_bottom b;
    b.b = 1;
    b.m1 = 2;
    b.m2 = 3;
    b.t = 4;

    msgpack::zone z;
    msgpack::object obj(b, z);
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
    v_bottom br = obj.as<v_bottom>();
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
    BOOST_CHECK_EQUAL(b.b, br.b);
    BOOST_CHECK_EQUAL(b.m1, br.m1);
    BOOST_CHECK_EQUAL(b.m2, br.m2);
    BOOST_CHECK_EQUAL(b.t, br.t);
}

#if !defined(MSGPACK_USE_CPP03)

BOOST_AUTO_TEST_CASE(construct_enum_outer_newstyle)
{
    msgpack::zone z;
    msgpack::object obj(outer_enum::enum_test::elem, z);
    BOOST_CHECK_EQUAL(msgpack::type::POSITIVE_INTEGER, obj.type);
    BOOST_CHECK_EQUAL(elem, obj.via.u64);
}

BOOST_AUTO_TEST_CASE(construct_class_enum)
{
    msgpack::zone z;
    msgpack::object obj(enum_class_test::elem, z);
    BOOST_CHECK_EQUAL(msgpack::type::POSITIVE_INTEGER, obj.type);
    BOOST_CHECK_EQUAL(elem, obj.via.u64);
}


BOOST_AUTO_TEST_CASE(construct_class_enum_outer)
{
    msgpack::zone z;
    msgpack::object obj(outer_enum_class::enum_class_test::elem, z);
    BOOST_CHECK_EQUAL(msgpack::type::POSITIVE_INTEGER, obj.type);
    BOOST_CHECK_EQUAL(elem, obj.via.u64);
}


BOOST_AUTO_TEST_CASE(array)
{
    typedef std::array<int, kElements> test_t;
    for (unsigned int k = 0; k < kLoop; k++) {
        test_t v1;
        v1[0] = 1;
        for (unsigned int i = 1; i < kElements; i++)
            v1[i] = rand();
        msgpack::zone z;
        msgpack::object obj(v1, z);
        BOOST_CHECK(obj.as<test_t>() == v1);
        v1.front() = 42;
        BOOST_CHECK_EQUAL(obj.as<test_t>().front(), 1);
    }
}

BOOST_AUTO_TEST_CASE(array_char)
{
    typedef std::array<char, kElements> test_t;
    for (unsigned int k = 0; k < kLoop; k++) {
        test_t v1;
        v1[0] = 1;
        for (unsigned int i = 1; i < kElements; i++)
            v1[i] = static_cast<char>(rand());
        msgpack::zone z;
        msgpack::object obj(v1, z);
        BOOST_CHECK(obj.as<test_t>() == v1);
        v1.front() = 42;
        BOOST_CHECK_EQUAL(obj.as<test_t>().front(), 1);
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
        msgpack::zone z;
        msgpack::object obj(v1, z);
        BOOST_CHECK(obj.as<test_t>() == v1);
        v1.front() = 42;
        BOOST_CHECK_EQUAL(obj.as<test_t>().front(), 1);
    }
}

BOOST_AUTO_TEST_CASE(forward_list)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        std::forward_list<int> v1;
        for (unsigned int i = 0; i < kElements; i++)
            v1.push_front(static_cast<int>(i));
        msgpack::zone z;
        msgpack::object obj(v1, z);
        BOOST_CHECK(obj.as<std::forward_list<int> >() == v1);
        v1.front() = 42;
        BOOST_CHECK_EQUAL(obj.as<std::forward_list<int> >().front(), static_cast<int>(kElements - 1));
    }
}

BOOST_AUTO_TEST_CASE(tuple)
{
    typedef std::tuple<int, std::string, bool> test_t;
    test_t v(1, "abc", true);
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK(obj.as<test_t>() == v);
}

BOOST_AUTO_TEST_CASE(tuple_empty)
{
    typedef std::tuple<> test_t;
    test_t v;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK(obj.as<test_t>() == v);
}

#if !defined(MSGPACK_NO_BOOST)

BOOST_AUTO_TEST_CASE(system_clock)
{
    std::chrono::system_clock::time_point v;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK(obj.as<std::chrono::system_clock::time_point>() == v);
}

BOOST_AUTO_TEST_CASE(system_clock_32)
{
    std::chrono::system_clock::time_point v(std::chrono::seconds(0x12345678L));
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK(obj.as<std::chrono::system_clock::time_point>() == v);
}

BOOST_AUTO_TEST_CASE(system_clock_32_max)
{
    std::chrono::system_clock::time_point v(std::chrono::seconds(0xffffffffL));
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK(obj.as<std::chrono::system_clock::time_point>() == v);
}

BOOST_AUTO_TEST_CASE(system_clock_64)
{
    std::chrono::system_clock::time_point v(std::chrono::seconds(0x31234567L));
    v +=
        std::chrono::duration_cast<std::chrono::system_clock::duration>(
            std::chrono::nanoseconds(0x312345678L)
        );
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK(obj.as<std::chrono::system_clock::time_point>() == v);
}

BOOST_AUTO_TEST_CASE(system_clock_64_max)
{
    std::chrono::system_clock::time_point v(std::chrono::seconds(0xffffffffL));
    v +=
        std::chrono::duration_cast<std::chrono::system_clock::duration>(
            std::chrono::nanoseconds(0x3b9ac9ffL) // 999,999,999
        );
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK(obj.as<std::chrono::system_clock::time_point>() == v);
}

BOOST_AUTO_TEST_CASE(system_clock_impl_min)
{
    std::chrono::system_clock::time_point v(std::chrono::system_clock::time_point::min());
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK(obj.as<std::chrono::system_clock::time_point>() == v);
}

BOOST_AUTO_TEST_CASE(system_clock_impl_max)
{
    std::chrono::system_clock::time_point v(std::chrono::system_clock::time_point::max());
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK(obj.as<std::chrono::system_clock::time_point>() == v);
}

#endif // !defined(MSGPACK_NO_BOOST)

#endif // !defined(MSGPACK_USE_CPP03)

BOOST_AUTO_TEST_CASE(ext_empty)
{
    msgpack::type::ext v;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK(obj.as<msgpack::type::ext>() == v);
    BOOST_CHECK(obj.as<msgpack::type::ext_ref>() == v);
}

BOOST_AUTO_TEST_CASE(ext)
{
    msgpack::type::ext v(42, 10);
    for (int i = 0; i < 10; ++i) v.data()[i] = static_cast<char>(i);
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK(obj.as<msgpack::type::ext>() == v);
    BOOST_CHECK(obj.as<msgpack::type::ext_ref>() == v);
}

BOOST_AUTO_TEST_CASE(ext_from_buf)
{
    char const buf[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    msgpack::type::ext v(42, buf, sizeof(buf));
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK(obj.as<msgpack::type::ext>() == v);
    BOOST_CHECK(obj.as<msgpack::type::ext_ref>() == v);
}

BOOST_AUTO_TEST_CASE(ext_ref_empty)
{
    msgpack::type::ext_ref v;
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK(obj.as<msgpack::type::ext>() == msgpack::type::ext(v));
    BOOST_CHECK(obj.as<msgpack::type::ext_ref>() == v);
}

BOOST_AUTO_TEST_CASE(ext_ref_from_buf)
{
    char const buf[] = { 77, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    msgpack::type::ext_ref v(buf, sizeof(buf));
    msgpack::zone z;
    msgpack::object obj(v, z);
    BOOST_CHECK(obj.as<msgpack::type::ext>() == msgpack::type::ext(v));
    BOOST_CHECK(obj.as<msgpack::type::ext_ref>() == v);
}
