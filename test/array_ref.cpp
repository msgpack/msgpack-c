#include <msgpack.hpp>

#include <string>
#include <sstream>

#define BOOST_TEST_MODULE MSGPACK_ARRAY_REF
#include <boost/test/unit_test.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

BOOST_AUTO_TEST_CASE(pack_unpack_convert_vector_char)
{
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');

    msgpack::type::array_ref<std::vector<char> > ar1 = msgpack::type::make_array_ref(v);
    std::stringstream ss;
    msgpack::pack(ss, ar1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);
    std::vector<char> v2;
    msgpack::type::array_ref<std::vector<char> > ar2(v2);
    oh.get().convert(ar2);
    BOOST_CHECK(ar1 == ar2);
}

BOOST_AUTO_TEST_CASE(pack_unpack_convert_vector_char_const)
{
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');

    std::vector<char> const& cv = v;

    msgpack::type::array_ref<std::vector<char> const> ar1 = msgpack::type::make_array_ref(cv);
    std::stringstream ss;
    msgpack::pack(ss, ar1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);
    std::vector<char> v2;
    msgpack::type::array_ref<std::vector<char> > ar2(v2);
    oh.get().convert(ar2);
    BOOST_CHECK(ar1 == ar2);
}

BOOST_AUTO_TEST_CASE(pack_unpack_convert_vector_unsigned_char)
{
    std::vector<unsigned char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');

    msgpack::type::array_ref<std::vector<unsigned char> > ar1 = msgpack::type::make_array_ref(v);
    std::stringstream ss;
    msgpack::pack(ss, ar1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);
    std::vector<unsigned char> v2;
    msgpack::type::array_ref<std::vector<unsigned char> > ar2(v2);
    oh.get().convert(ar2);
    BOOST_CHECK(ar1 == ar2);
}

BOOST_AUTO_TEST_CASE(pack_unpack_convert_vector_unsigned_char_const)
{
    std::vector<unsigned char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');

    std::vector<unsigned char> const& cv = v;

    msgpack::type::array_ref<std::vector<unsigned char> const> ar1 = msgpack::type::make_array_ref(cv);
    std::stringstream ss;
    msgpack::pack(ss, ar1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);
    std::vector<unsigned char> v2;
    msgpack::type::array_ref<std::vector<unsigned char> > ar2(v2);
    oh.get().convert(ar2);
    BOOST_CHECK(ar1 == ar2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_vector_char)
{
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');

    msgpack::type::array_ref<std::vector<char> > ar1 = msgpack::type::make_array_ref(v);
    msgpack::zone z;
    msgpack::object obj(ar1, z);

    BOOST_CHECK_EQUAL(obj.type, msgpack::type::ARRAY);
    std::vector<char> v2;
    msgpack::type::array_ref<std::vector<char> > ar2(v2);
    obj.convert(ar2);
    BOOST_CHECK(ar1 == ar2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_vector_char_const)
{
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');

    std::vector<char> const& cv = v;

    msgpack::type::array_ref<std::vector<char> const> ar1 = msgpack::type::make_array_ref(cv);
    msgpack::zone z;
    msgpack::object obj(ar1, z);

    BOOST_CHECK_EQUAL(obj.type, msgpack::type::ARRAY);
    std::vector<char> v2;
    msgpack::type::array_ref<std::vector<char> > ar2(v2);
    obj.convert(ar2);
    BOOST_CHECK(ar1 == ar2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_vector_unsigned_char)
{
    std::vector<unsigned char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');

    msgpack::type::array_ref<std::vector<unsigned char> > ar1 = msgpack::type::make_array_ref(v);
    msgpack::zone z;
    msgpack::object obj(ar1, z);

    BOOST_CHECK_EQUAL(obj.type, msgpack::type::ARRAY);
    std::vector<unsigned char> v2;
    msgpack::type::array_ref<std::vector<unsigned char> > ar2(v2);
    obj.convert(ar2);
    BOOST_CHECK(ar1 == ar2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_vector_unsigned_char_const)
{
    std::vector<unsigned char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');

    std::vector<unsigned char> const& cv = v;

    msgpack::type::array_ref<std::vector<unsigned char> const> ar1 = msgpack::type::make_array_ref(cv);
    msgpack::zone z;
    msgpack::object obj(ar1, z);

    BOOST_CHECK_EQUAL(obj.type, msgpack::type::ARRAY);
    std::vector<unsigned char> v2;
    msgpack::type::array_ref<std::vector<unsigned char> > ar2(v2);
    obj.convert(ar2);
    BOOST_CHECK(ar1 == ar2);
}

#if !defined(MSGPACK_USE_CPP03)

BOOST_AUTO_TEST_CASE(pack_unpack_convert_array_char)
{
    std::array<char, 3> v { { 'a', 'b', 'c' } };

    msgpack::type::array_ref<std::array<char, 3> > ar1 = msgpack::type::make_array_ref(v);
    std::stringstream ss;
    msgpack::pack(ss, ar1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);
    std::array<char, 3> v2;
    msgpack::type::array_ref<std::array<char, 3> > ar2(v2);
    oh.get().convert(ar2);
    BOOST_CHECK(ar1 == ar2);
}

BOOST_AUTO_TEST_CASE(pack_unpack_convert_array_char_const)
{
    std::array<char, 3> v { { 'a', 'b', 'c' } };

    std::array<char, 3> const& cv = v;

    msgpack::type::array_ref<std::array<char, 3> const> ar1 = msgpack::type::make_array_ref(cv);
    std::stringstream ss;
    msgpack::pack(ss, ar1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);
    std::array<char, 3> v2;
    msgpack::type::array_ref<std::array<char, 3> > ar2(v2);
    oh.get().convert(ar2);
    BOOST_CHECK(ar1 == ar2);
}

BOOST_AUTO_TEST_CASE(pack_unpack_convert_array_unsigned_char)
{
    std::array<unsigned char, 3> v { { 'a', 'b', 'c' } };

    msgpack::type::array_ref<std::array<unsigned char, 3> > ar1 = msgpack::type::make_array_ref(v);
    std::stringstream ss;
    msgpack::pack(ss, ar1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);
    std::array<unsigned char, 3> v2;
    msgpack::type::array_ref<std::array<unsigned char, 3> > ar2(v2);
    oh.get().convert(ar2);
    BOOST_CHECK(ar1 == ar2);
}

BOOST_AUTO_TEST_CASE(pack_unpack_convert_array_unsigned_char_const)
{
    std::array<unsigned char, 3> v { { 'a', 'b', 'c' } };

    std::array<unsigned char, 3> const& cv = v;

    msgpack::type::array_ref<std::array<unsigned char, 3> const> ar1 = msgpack::type::make_array_ref(cv);
    std::stringstream ss;
    msgpack::pack(ss, ar1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);
    std::array<unsigned char, 3> v2;
    msgpack::type::array_ref<std::array<unsigned char, 3> > ar2(v2);
    oh.get().convert(ar2);
    BOOST_CHECK(ar1 == ar2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_array_char)
{
    std::array<char, 3> v { { 'a', 'b', 'c' } };

    msgpack::type::array_ref<std::array<char, 3> > ar1 = msgpack::type::make_array_ref(v);
    msgpack::zone z;
    msgpack::object obj(ar1, z);

    BOOST_CHECK_EQUAL(obj.type, msgpack::type::ARRAY);
    std::array<char, 3> v2;
    msgpack::type::array_ref<std::array<char, 3> > ar2(v2);
    obj.convert(ar2);
    BOOST_CHECK(ar1 == ar2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_array_char_const)
{
    std::array<char, 3> v { { 'a', 'b', 'c' } };

    std::array<char, 3> const& cv = v;

    msgpack::type::array_ref<std::array<char, 3> const> ar1 = msgpack::type::make_array_ref(cv);
    msgpack::zone z;
    msgpack::object obj(ar1, z);

    BOOST_CHECK_EQUAL(obj.type, msgpack::type::ARRAY);
    std::array<char, 3> v2;
    msgpack::type::array_ref<std::array<char, 3> > ar2(v2);
    obj.convert(ar2);
    BOOST_CHECK(ar1 == ar2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_array_unsigned_char)
{
    std::array<unsigned char, 3> v { { 'a', 'b', 'c' } };

    msgpack::type::array_ref<std::array<unsigned char, 3> > ar1 = msgpack::type::make_array_ref(v);
    msgpack::zone z;
    msgpack::object obj(ar1, z);

    BOOST_CHECK_EQUAL(obj.type, msgpack::type::ARRAY);
    std::array<unsigned char, 3> v2;
    msgpack::type::array_ref<std::array<unsigned char, 3> > ar2(v2);
    obj.convert(ar2);
    BOOST_CHECK(ar1 == ar2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_array_unsigned_char_const)
{
    std::array<unsigned char, 3> v { { 'a', 'b', 'c' } };

    std::array<unsigned char, 3> const& cv = v;

    msgpack::type::array_ref<std::array<unsigned char, 3> const> ar1 = msgpack::type::make_array_ref(cv);
    msgpack::zone z;
    msgpack::object obj(ar1, z);

    BOOST_CHECK_EQUAL(obj.type, msgpack::type::ARRAY);
    std::array<unsigned char, 3> v2;
    msgpack::type::array_ref<std::array<unsigned char, 3> > ar2(v2);
    obj.convert(ar2);
    BOOST_CHECK(ar1 == ar2);
}

#endif // !defined(MSGPACK_USE_CPP03)
