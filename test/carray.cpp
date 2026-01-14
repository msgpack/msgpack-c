#include <msgpack.hpp>

#define BOOST_TEST_MODULE carray
#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_CASE(pack_unpack_int)
{
    std::stringstream ss;
    int v1[] = { 0, 1 };

    msgpack::pack(ss, v1);
    std::string const& str = ss.str();
    msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);

    int v2[2];
    oh.get().convert(v2);
    BOOST_CHECK_EQUAL(v1[0], v2[0]);
    BOOST_CHECK_EQUAL(v1[1], v2[1]);
}

BOOST_AUTO_TEST_CASE(obj_with_zone_int)
{
    int v1[] = { 0, 1 };
    msgpack::zone z;

    msgpack::object o(v1, z);
    BOOST_CHECK_EQUAL(o.type, msgpack::type::ARRAY);

    int v2[2];
    o.convert(v2);
    BOOST_CHECK_EQUAL(v1[0], v2[0]);
    BOOST_CHECK_EQUAL(v1[1], v2[1]);
}

BOOST_AUTO_TEST_CASE(pack_unpack_const_int)
{
    std::stringstream ss;
    const int v1[] = { 0, 1 };

    msgpack::pack(ss, v1);
    std::string const& str = ss.str();
    msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);

    int v2[2];
    oh.get().convert(v2);
    BOOST_CHECK_EQUAL(v1[0], v2[0]);
    BOOST_CHECK_EQUAL(v1[1], v2[1]);
}

BOOST_AUTO_TEST_CASE(obj_with_zone_const_int)
{
    const int v1[] = { 0, 1 };
    msgpack::zone z;

    msgpack::object o(v1, z);
    BOOST_CHECK_EQUAL(o.type, msgpack::type::ARRAY);

    int v2[2];
    o.convert(v2);
    BOOST_CHECK_EQUAL(v1[0], v2[0]);
    BOOST_CHECK_EQUAL(v1[1], v2[1]);
}

BOOST_AUTO_TEST_CASE(pack_unpack_string)
{
    std::stringstream ss;
    std::string v1[] = { "ABC", "DEFG" };

    msgpack::pack(ss, v1);
    std::string const& str = ss.str();
    msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);

    std::string v2[2];
    oh.get().convert(v2);
    BOOST_CHECK_EQUAL(v1[0], v2[0]);
    BOOST_CHECK_EQUAL(v1[1], v2[1]);
}

BOOST_AUTO_TEST_CASE(obj_with_zone_string)
{
    std::string v1[] = { "ABC", "DEFG" };
    msgpack::zone z;

    msgpack::object o(v1, z);
    BOOST_CHECK_EQUAL(o.type, msgpack::type::ARRAY);

    std::string v2[2];
    o.convert(v2);
    BOOST_CHECK_EQUAL(v1[0], v2[0]);
    BOOST_CHECK_EQUAL(v1[1], v2[1]);
}

BOOST_AUTO_TEST_CASE(pack_unpack_char)
{
    std::stringstream ss;
    char v1[] = { 0, 1 };

    msgpack::pack(ss, msgpack::type::make_array_ref(v1));
    std::string const& str = ss.str();
    msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);

    char v2[2];
    msgpack::type::array_ref<char[2]> ar = msgpack::type::make_array_ref(v2);
    oh.get().convert(ar);
    BOOST_CHECK_EQUAL(v1[0], v2[0]);
    BOOST_CHECK_EQUAL(v1[1], v2[1]);
}

BOOST_AUTO_TEST_CASE(pack_unpack_char_as_str)
{
    std::stringstream ss;
    char v1[2] = { 1, '\0' };

    msgpack::pack(ss, v1);
    std::string const& str = ss.str();
    msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::STR);
    BOOST_CHECK_EQUAL(oh.get().via.str.size, static_cast<size_t>(1));

    char v2[2] = { 2, 3 };
    oh.get().convert(v2);
    BOOST_CHECK_EQUAL(v1[0], v2[0]);
    BOOST_CHECK_EQUAL(v1[1], v2[1]);
}

BOOST_AUTO_TEST_CASE(obj_with_zone_char)
{
    char v1[] = { 0, 1 };
    msgpack::zone z;

    msgpack::object o(msgpack::type::make_array_ref(v1), z);
    BOOST_CHECK_EQUAL(o.type, msgpack::type::ARRAY);

    char v2[2];
    msgpack::type::array_ref<char[2]> ar = msgpack::type::make_array_ref(v2);
    o.convert(ar);
    BOOST_CHECK_EQUAL(v1[0], v2[0]);
    BOOST_CHECK_EQUAL(v1[1], v2[1]);
}

BOOST_AUTO_TEST_CASE(obj_with_zone_char_as_str)
{
    char v1[] = { 1, '\0' };
    msgpack::zone z;

    msgpack::object o(v1, z);
    BOOST_CHECK_EQUAL(o.type, msgpack::type::STR);
    BOOST_CHECK_EQUAL(o.via.str.size, static_cast<size_t>(1));

    char v2[2];
    o.convert(v2);
    BOOST_CHECK_EQUAL(v1[0], v2[0]);
    BOOST_CHECK_EQUAL(v1[1], v2[1]);
}

BOOST_AUTO_TEST_CASE(pack_unpack_unsigned_char)
{
    std::stringstream ss;
    unsigned char v1[] = { 0, 1 };

    msgpack::pack(ss, msgpack::type::make_array_ref(v1));
    std::string const& str = ss.str();
    msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);

    unsigned char v2[2];
    msgpack::type::array_ref<unsigned char[2]> ar = msgpack::type::make_array_ref(v2);
    oh.get().convert(ar);
    BOOST_CHECK_EQUAL(v1[0], v2[0]);
    BOOST_CHECK_EQUAL(v1[1], v2[1]);
}

BOOST_AUTO_TEST_CASE(pack_unpack_unsigned_char_as_bin)
{
    std::stringstream ss;
    unsigned char v1[] = { 0, 1 };

    msgpack::pack(ss, v1);
    std::string const& str = ss.str();
    msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::BIN);
    BOOST_CHECK_EQUAL(oh.get().via.bin.size, static_cast<size_t>(2));

    unsigned char v2[2];
    oh.get().convert(v2);
    BOOST_CHECK_EQUAL(v1[0], v2[0]);
    BOOST_CHECK_EQUAL(v1[1], v2[1]);
}

BOOST_AUTO_TEST_CASE(obj_with_zone_unsigned_char)
{
    unsigned char v1[] = { 0, 1 };
    msgpack::zone z;

    msgpack::object o(msgpack::type::make_array_ref(v1), z);
    BOOST_CHECK_EQUAL(o.type, msgpack::type::ARRAY);

    unsigned char v2[2];
    msgpack::type::array_ref<unsigned char[2]> ar = msgpack::type::make_array_ref(v2);
    o.convert(ar);
    BOOST_CHECK_EQUAL(v1[0], v2[0]);
    BOOST_CHECK_EQUAL(v1[1], v2[1]);
}

BOOST_AUTO_TEST_CASE(obj_with_zone_unsigned_char_as_bin)
{
    unsigned char v1[] = { 0, 1 };
    msgpack::zone z;

    msgpack::object o(v1, z);
    BOOST_CHECK_EQUAL(o.type, msgpack::type::BIN);
    BOOST_CHECK_EQUAL(o.via.bin.size, static_cast<size_t>(2));

    unsigned char v2[2];
    o.convert(v2);
    BOOST_CHECK_EQUAL(v1[0], v2[0]);
    BOOST_CHECK_EQUAL(v1[1], v2[1]);
}

BOOST_AUTO_TEST_CASE(pack_unpack_signed_char)
{
    std::stringstream ss;
    signed char v1[] = { 0, 1 };

    msgpack::pack(ss, v1);
    std::string const& str = ss.str();
    msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);

    signed char v2[2];
    oh.get().convert(v2);
    BOOST_CHECK_EQUAL(v1[0], v2[0]);
    BOOST_CHECK_EQUAL(v1[1], v2[1]);
}

BOOST_AUTO_TEST_CASE(obj_with_zone_signed_char)
{
    signed char v1[] = { 0, 1 };
    msgpack::zone z;

    msgpack::object o(v1, z);
    BOOST_CHECK_EQUAL(o.type, msgpack::type::ARRAY);

    signed char v2[2];
    o.convert(v2);
    BOOST_CHECK_EQUAL(v1[0], v2[0]);
    BOOST_CHECK_EQUAL(v1[1], v2[1]);
}

#if MSGPACK_DEFAULT_API_VERSION == 1

BOOST_AUTO_TEST_CASE(object_as_str)
{
    char v1[] = { 1, '\0' };

    msgpack::zone z;
    msgpack::object o(v1);
    BOOST_CHECK_EQUAL(o.type, msgpack::type::STR);
}
BOOST_AUTO_TEST_CASE(object_as_str_const)
{
    const char v1[] = { 1, '\0' };

    msgpack::zone z;
    msgpack::object o(v1);
    BOOST_CHECK_EQUAL(o.type, msgpack::type::STR);
}

#endif // MSGPACK_DEFAULT_API_VERSION == 1
