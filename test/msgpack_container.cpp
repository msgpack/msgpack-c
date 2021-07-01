#include <iostream>
#include <msgpack.hpp>

#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <set>
#include <list>
#include <limits>

#include "test_allocator.hpp"

#define BOOST_TEST_MODULE MSGPACK_STL
#include <boost/test/unit_test.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


using namespace std;

const unsigned int kLoop = 1000;
const unsigned int kElements = 100;

// strong typedefs
namespace test {

template <class Key>
struct equal_to : std::equal_to<Key> {
};

template <class Key>
struct less : std::less<Key> {
};

} // namespace test

BOOST_AUTO_TEST_CASE(simple_buffer_cstring)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        string val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1 += static_cast<char>('a' + rand() % 26);
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1.c_str());
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::STR);
        string val2 = oh.get().as<string>();
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        BOOST_CHECK_EQUAL(val1, val2);
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_non_const_cstring)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        string val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1 += static_cast<char>('a' + rand() % 26);
        msgpack::sbuffer sbuf;
        char* s = new char[val1.size() + 1];
        std::memcpy(s, val1.c_str(), val1.size() + 1);
        msgpack::pack(sbuf, s);
        delete [] s;
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::STR);
        string val2 = oh.get().as<string>();
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        BOOST_CHECK_EQUAL(val1, val2);
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_wstring)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        wstring val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1 += L'a' + rand() % 26;
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);
        wstring val2 = oh.get().as<wstring>();
        BOOST_CHECK(val1 == val2);
        wstring val3;
        oh.get().convert(val3);
        BOOST_CHECK(val1 == val3);
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_vector)
{
    typedef vector<int, test::allocator<int> > type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1.push_back(rand());
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);
        type const& val2 = oh.get().as<type>();
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_vector_empty)
{
    typedef vector<int, test::allocator<int> > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);
    type const& val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
    BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
}

BOOST_AUTO_TEST_CASE(simple_buffer_vector_char)
{
    typedef vector<char, test::allocator<char> > type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1.push_back(static_cast<char>(rand()));
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::BIN);
        type const& val2 = oh.get().as<type>();
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_vector_char_empty)
{
    typedef vector<char, test::allocator<char> > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::BIN);
    type const& val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
    BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
}

BOOST_AUTO_TEST_CASE(simple_buffer_vector_unsigned_char)
{
    typedef vector<unsigned char, test::allocator<unsigned char> > type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1.push_back(static_cast<unsigned char>(rand()));
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::BIN);
        type const& val2 = oh.get().as<type>();
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_vector_unsigned_char_empty)
{
    typedef vector<unsigned char, test::allocator<unsigned char> > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::BIN);
    type const& val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
    BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
}

BOOST_AUTO_TEST_CASE(simple_buffer_vector_uint8_t)
{
    if (!msgpack::is_same<uint8_t, unsigned char>::value) return;
    typedef vector<uint8_t, test::allocator<uint8_t> > type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1.push_back(static_cast<uint8_t>(rand()));
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::BIN);
        type const& val2 = oh.get().as<type>();
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_vector_uint8_t_empty)
{
    if (!msgpack::is_same<uint8_t, unsigned char>::value) return;
    typedef vector<uint8_t, test::allocator<uint8_t> > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::BIN);
    type const& val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
    BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
}

BOOST_AUTO_TEST_CASE(simple_buffer_vector_bool)
{
    typedef vector<bool, test::allocator<bool> > type;
    type val1;
    for (unsigned int i = 0; i < kElements; i++)
        val1.push_back(i % 2 ? false : true);
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);
    type const& val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
    BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
}

BOOST_AUTO_TEST_CASE(simple_buffer_vector_bool_empty)
{
    typedef vector<bool, test::allocator<bool> > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);
    type const& val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
    BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
}


BOOST_AUTO_TEST_CASE(simple_buffer_assoc_vector)
{
    typedef msgpack::type::assoc_vector<int, int, test::less<int>, test::allocator<std::pair<int, int> > >type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        val1.push_back(std::make_pair(1, 2));
        val1.push_back(std::make_pair(3, 4));
        val1.push_back(std::make_pair(5, 6));
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        type const& val2 = oh.get().as<type>();
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_assoc_vector_empty)
{
    typedef msgpack::type::assoc_vector<int, int, test::less<int>, test::allocator<std::pair<int, int> > >type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    type const& val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
    BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
}

BOOST_AUTO_TEST_CASE(simple_buffer_map)
{
    typedef map<int, int, test::less<int>, test::allocator<std::pair<const int, int> > > type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1[rand()] = rand();
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        type const& val2 = oh.get().as<type>();
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_map_empty)
{
    typedef map<int, int, test::less<int>, test::allocator<std::pair<const int, int> > > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    type const& val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
    BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
}

BOOST_AUTO_TEST_CASE(simple_buffer_deque)
{
    typedef deque<int, test::allocator<int> > type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1.push_back(rand());
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        type const& val2 = oh.get().as<type>();
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_deque_empty)
{
    typedef deque<int, test::allocator<int> > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    type const& val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
    BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
}

BOOST_AUTO_TEST_CASE(simple_buffer_list)
{
    typedef list<int, test::allocator<int> > type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1.push_back(rand());
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        type const& val2 = oh.get().as<type>();
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_list_empty)
{
    typedef list<int, test::allocator<int> > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    type const& val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
    BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
}

BOOST_AUTO_TEST_CASE(simple_buffer_set)
{
    typedef set<int, test::less<int>, test::allocator<int> > type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1.insert(rand());
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        type val2 = oh.get().as<type>();
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_set_empty)
{
    typedef set<int, test::less<int>, test::allocator<int> > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    type val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
    BOOST_CHECK(equal(val1.begin(), val1.end(), val2.begin()));
}

BOOST_AUTO_TEST_CASE(simple_buffer_pair)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        pair<int, int> val1 = make_pair(rand(), rand());
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        pair<int, int> val2 = oh.get().as<pair<int, int> >();
        BOOST_CHECK_EQUAL(val1.first, val2.first);
        BOOST_CHECK_EQUAL(val1.second, val2.second);
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_complex_float)
{
    complex<float> val1 = complex<float>(1.23F, 4.56F);
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    complex<float> val2 = oh.get().as<complex<float> >();
    BOOST_CHECK_EQUAL(val1, val2);
    complex<float> val3;
    oh.get().convert(val3);
    BOOST_CHECK_EQUAL(val1, val3);

}

BOOST_AUTO_TEST_CASE(simple_buffer_complex_double)
{
    complex<double> val1 = complex<double>(1.23, 4.56);
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    complex<double> val2 = oh.get().as<complex<double> >();
    BOOST_CHECK_EQUAL(val1, val2);
    complex<double> val3;
    oh.get().convert(val3);
    BOOST_CHECK_EQUAL(val1, val3);

}

BOOST_AUTO_TEST_CASE(simple_buffer_multimap)
{
    typedef multimap<int, int, test::less<int>, test::allocator<std::pair<const int, int> > > type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        for (unsigned int i = 0; i < kElements; i++) {
            int i1 = rand();
            val1.insert(make_pair(i1, rand()));
            val1.insert(make_pair(i1, rand()));
        }
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        type val2 = oh.get().as<type>();

        vector<pair<int, int> > v1, v2;
        type::const_iterator it;
        for (it = val1.begin(); it != val1.end(); ++it)
            v1.push_back(make_pair(it->first, it->second));
        for (it = val2.begin(); it != val2.end(); ++it)
            v2.push_back(make_pair(it->first, it->second));
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        BOOST_CHECK_EQUAL(v1.size(), v2.size());
        sort(v1.begin(), v1.end());
        sort(v2.begin(), v2.end());
        BOOST_CHECK(v1 == v2);
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_multimap_empty)
{
    typedef multimap<int, int, test::less<int>, test::allocator<std::pair<const int, int> > > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    type val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
}

BOOST_AUTO_TEST_CASE(simple_buffer_multiset)
{
    typedef multiset<int, test::less<int>, test::allocator<int> > type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1.insert(rand());
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        type val2 = oh.get().as<type>();

        vector<int> v1, v2;
        type::const_iterator it;
        for (it = val1.begin(); it != val1.end(); ++it)
            v1.push_back(*it);
        for (it = val2.begin(); it != val2.end(); ++it)
            v2.push_back(*it);
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        BOOST_CHECK_EQUAL(v1.size(), v2.size());
        sort(v1.begin(), v1.end());
        sort(v2.begin(), v2.end());
        BOOST_CHECK(v1 == v2);
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_multiset_empty)
{
    typedef multiset<int, test::less<int>, test::allocator<int> > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    type val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
}

BOOST_AUTO_TEST_CASE(simple_tuple)
{
    msgpack::sbuffer sbuf;
    msgpack::type::tuple<bool, std::string, double> val1(true, "kzk", 12.3);
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    msgpack::type::tuple<bool, std::string, double> val2
        = oh.get().as<msgpack::type::tuple<bool, std::string, double> >();
    BOOST_CHECK_EQUAL(oh.get().via.array.size, 3u);
    BOOST_CHECK_EQUAL(val1.get<0>(), val2.get<0>());
    BOOST_CHECK_EQUAL(val1.get<1>(), val2.get<1>());
    BOOST_CHECK_EQUAL(val1.get<2>(), val2.get<2>());
}

BOOST_AUTO_TEST_CASE(simple_tuple_empty)
{
    msgpack::sbuffer sbuf;
    msgpack::type::tuple<> val1;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    oh.get().as<msgpack::type::tuple<> >();
    BOOST_CHECK_EQUAL(oh.get().via.array.size, 0u);
}

BOOST_AUTO_TEST_CASE(simple_tuple_grater_than_as)
{
    msgpack::sbuffer sbuf;
    msgpack::type::tuple<bool, std::string, int> val1(true, "kzk", 42);
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    msgpack::type::tuple<bool, std::string, int, int> val2
        = oh.get().as<msgpack::type::tuple<bool, std::string, int, int> >();
    BOOST_CHECK_EQUAL(oh.get().via.array.size, 3u);
    BOOST_CHECK_EQUAL(val1.get<0>(), val2.get<0>());
    BOOST_CHECK_EQUAL(val1.get<1>(), val2.get<1>());
    BOOST_CHECK_EQUAL(val1.get<2>(), val2.get<2>());
}

BOOST_AUTO_TEST_CASE(simple_tuple_grater_than_convert)
{
    msgpack::sbuffer sbuf;
    msgpack::type::tuple<bool, std::string, int> val1(true, "kzk", 42);
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    msgpack::type::tuple<bool, std::string, int, int> val2;
    oh.get().convert(val2);
    BOOST_CHECK_EQUAL(oh.get().via.array.size, 3u);
    BOOST_CHECK_EQUAL(val1.get<0>(), val2.get<0>());
    BOOST_CHECK_EQUAL(val1.get<1>(), val2.get<1>());
    BOOST_CHECK_EQUAL(val1.get<2>(), val2.get<2>());
}

BOOST_AUTO_TEST_CASE(simple_tuple_less_than_as)
{
    msgpack::sbuffer sbuf;
    msgpack::type::tuple<bool, std::string, int> val1(true, "kzk", 42);
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    msgpack::type::tuple<bool, std::string> val2
        = oh.get().as<msgpack::type::tuple<bool, std::string> >();
    BOOST_CHECK_EQUAL(oh.get().via.array.size, 3u);
    BOOST_CHECK_EQUAL(val1.get<0>(), val2.get<0>());
    BOOST_CHECK_EQUAL(val1.get<1>(), val2.get<1>());
}

BOOST_AUTO_TEST_CASE(simple_tuple_less_than_convert)
{
    msgpack::sbuffer sbuf;
    msgpack::type::tuple<bool, std::string, int> val1(true, "kzk", 42);
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    msgpack::type::tuple<bool, std::string> val2;
    oh.get().convert(val2);
    BOOST_CHECK_EQUAL(oh.get().via.array.size, 3u);
    BOOST_CHECK_EQUAL(val1.get<0>(), val2.get<0>());
    BOOST_CHECK_EQUAL(val1.get<1>(), val2.get<1>());
}

BOOST_AUTO_TEST_CASE(simple_tuple_nest)
{
    msgpack::sbuffer sbuf;
    msgpack::type::tuple<msgpack::type::tuple<> > val1;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    msgpack::type::tuple<msgpack::type::tuple<> > val2;
    oh.get().convert(val2);
    BOOST_CHECK_EQUAL(oh.get().via.array.size, 1u);
}

// TR1

#if defined(MSGPACK_HAS_STD_TR1_UNORDERED_MAP) || defined(MSGPACK_HAS_STD_TR1_UNORDERED_SET)

#include <tr1/functional>

namespace test {

template <class Key>
struct tr1_hash : std::tr1::hash<Key> {
};

} // namespace test

#endif // defined(MSGPACK_HAS_STD_TR1_UNORDERED_MAP) || defined(MSGPACK_HAS_STD_TR1_UNORDERED_SET)

#ifdef MSGPACK_HAS_STD_TR1_UNORDERED_MAP
#include <tr1/unordered_map>
#include "msgpack/adaptor/tr1/unordered_map.hpp"
BOOST_AUTO_TEST_CASE(simple_buffer_tr1_unordered_map)
{
    typedef tr1::unordered_map<int, int, test::tr1_hash<int>, test::equal_to<int>, test::allocator<std::pair<const int, int> > > type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1[rand()] = rand();
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        type val2 = oh.get().as<type>();
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        type::const_iterator it;
        for (it = val1.begin(); it != val1.end(); ++it) {
            BOOST_CHECK(val2.find(it->first) != val2.end());
            BOOST_CHECK_EQUAL(it->second, val2.find(it->first)->second);
        }
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_tr1_unordered_map_empty)
{
    typedef tr1::unordered_map<int, int, test::tr1_hash<int>, test::equal_to<int>, test::allocator<std::pair<const int, int> > > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    type val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
}

BOOST_AUTO_TEST_CASE(simple_buffer_tr1_unordered_multimap)
{
    typedef tr1::unordered_multimap<int, int, test::tr1_hash<int>, test::equal_to<int>, test::allocator<std::pair<const int, int> > > type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        for (unsigned int i = 0; i < kElements; i++) {
            int i1 = rand();
            val1.insert(make_pair(i1, rand()));
            val1.insert(make_pair(i1, rand()));
        }
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        type val2 = oh.get().as<type>();

        vector<pair<int, int> > v1, v2;
        type::const_iterator it;
        for (it = val1.begin(); it != val1.end(); ++it)
            v1.push_back(make_pair(it->first, it->second));
        for (it = val2.begin(); it != val2.end(); ++it)
            v2.push_back(make_pair(it->first, it->second));
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        BOOST_CHECK_EQUAL(v1.size(), v2.size());
        sort(v1.begin(), v1.end());
        sort(v2.begin(), v2.end());
        BOOST_CHECK(v1 == v2);
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_tr1_unordered_multimap_empty)
{
    typedef tr1::unordered_multimap<int, int, test::tr1_hash<int>, test::equal_to<int>, test::allocator<std::pair<const int, int> > > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    type val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
}

#endif

#ifdef MSGPACK_HAS_STD_TR1_UNORDERED_SET
#include <tr1/unordered_set>
#include "msgpack/adaptor/tr1/unordered_set.hpp"
BOOST_AUTO_TEST_CASE(simple_buffer_tr1_unordered_set)
{
    typedef tr1::unordered_set<int, test::tr1_hash<int>, test::equal_to<int>, test::allocator<int> > type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1.insert(rand());
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        type val2 = oh.get().as<type>();
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        type::const_iterator it;
        for (it = val1.begin(); it != val1.end(); ++it)
            BOOST_CHECK(val2.find(*it) != val2.end());
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_tr1_unordered_set_empty)
{
    typedef tr1::unordered_set<int, test::tr1_hash<int>, test::equal_to<int>, test::allocator<int> > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    type val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
}

BOOST_AUTO_TEST_CASE(simple_buffer_tr1_unordered_multiset)
{
    typedef tr1::unordered_multiset<int, test::tr1_hash<int>, test::equal_to<int>, test::allocator<int> > type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1.insert(rand());
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        type val2 = oh.get().as<type>();

        vector<int> v1, v2;
        type::const_iterator it;
        for (it = val1.begin(); it != val1.end(); ++it)
            v1.push_back(*it);
        for (it = val2.begin(); it != val2.end(); ++it)
            v2.push_back(*it);
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        BOOST_CHECK_EQUAL(v1.size(), v2.size());
        sort(v1.begin(), v1.end());
        sort(v2.begin(), v2.end());
        BOOST_CHECK(v1 == v2);
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_tr1_unordered_multiset_empty)
{
    typedef tr1::unordered_multiset<int, test::tr1_hash<int>, test::equal_to<int>, test::allocator<int> > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    type val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
}

#endif

#if defined (MSGPACK_HAS_STD_UNORDERED_MAP) || defined (MSGPACK_HAS_STD_UNORDERED_SET)

#include <functional>

namespace test {

template <class Key>
struct hash : std::hash<Key> {
};

} // namespace test

#endif // defined (MSGPACK_HAS_STD_UNORDERED_MAP) || defined (MSGPACK_HAS_STD_UNORDERED_SET)

#ifdef MSGPACK_HAS_STD_UNORDERED_MAP
#include <unordered_map>
#include "msgpack/adaptor/tr1/unordered_map.hpp"
BOOST_AUTO_TEST_CASE(simple_buffer_unordered_map)
{
    typedef unordered_map<int, int, test::hash<int>, test::equal_to<int>, test::allocator<std::pair<const int, int> > > type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1[rand()] = rand();
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        type val2 = oh.get().as<type>();
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        type::const_iterator it;
        for (it = val1.begin(); it != val1.end(); ++it) {
            BOOST_CHECK(val2.find(it->first) != val2.end());
            BOOST_CHECK_EQUAL(it->second, val2.find(it->first)->second);
        }
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_unordered_map_empty)
{
    typedef unordered_map<int, int, test::hash<int>, test::equal_to<int>, test::allocator<std::pair<const int, int> > > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    type val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
}

BOOST_AUTO_TEST_CASE(simple_buffer_unordered_multimap)
{
    typedef unordered_multimap<int, int, test::hash<int>, test::equal_to<int>, test::allocator<std::pair<const int, int> > > type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        for (unsigned int i = 0; i < kElements; i++) {
            int i1 = rand();
            val1.insert(make_pair(i1, rand()));
            val1.insert(make_pair(i1, rand()));
        }
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        type val2 = oh.get().as<type>();

        vector<pair<int, int> > v1, v2;
        type::const_iterator it;
        for (it = val1.begin(); it != val1.end(); ++it)
            v1.push_back(make_pair(it->first, it->second));
        for (it = val2.begin(); it != val2.end(); ++it)
            v2.push_back(make_pair(it->first, it->second));
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        BOOST_CHECK_EQUAL(v1.size(), v2.size());
        sort(v1.begin(), v1.end());
        sort(v2.begin(), v2.end());
        BOOST_CHECK(v1 == v2);
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_unordered_multimap_empty)
{
    typedef unordered_multimap<int, int, test::hash<int>, test::equal_to<int>, test::allocator<std::pair<const int, int> > > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    type val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
}

#endif

#ifdef MSGPACK_HAS_STD_UNORDERED_SET

#include <unordered_set>
#include "msgpack/adaptor/tr1/unordered_set.hpp"
BOOST_AUTO_TEST_CASE(simple_buffer_unordered_set)
{
    typedef unordered_set<int, test::hash<int>, test::equal_to<int>, test::allocator<int> > type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1.insert(rand());
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        type val2 = oh.get().as<type>();
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        type::const_iterator it;
        for (it = val1.begin(); it != val1.end(); ++it)
            BOOST_CHECK(val2.find(*it) != val2.end());
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_unordered_set_empty)
{
    typedef unordered_set<int, test::hash<int>, test::equal_to<int>, test::allocator<int> > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    type val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
}

BOOST_AUTO_TEST_CASE(simple_buffer_unordered_multiset)
{
    typedef unordered_multiset<int, test::hash<int>, test::equal_to<int>, test::allocator<int> > type;
    for (unsigned int k = 0; k < kLoop; k++) {
        type val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1.insert(rand());
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        type val2 = oh.get().as<type>();

        vector<int> v1, v2;
        type::const_iterator it;
        for (it = val1.begin(); it != val1.end(); ++it)
            v1.push_back(*it);
        for (it = val2.begin(); it != val2.end(); ++it)
            v2.push_back(*it);
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        BOOST_CHECK_EQUAL(v1.size(), v2.size());
        sort(v1.begin(), v1.end());
        sort(v2.begin(), v2.end());
        BOOST_CHECK(v1 == v2);
    }
}

BOOST_AUTO_TEST_CASE(simple_buffer_unordered_multiset_empty)
{
    typedef unordered_multiset<int, test::hash<int>, test::equal_to<int>, test::allocator<int> > type;
    type val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    type val2 = oh.get().as<type>();
    BOOST_CHECK_EQUAL(val1.size(), val2.size());
}

#endif
