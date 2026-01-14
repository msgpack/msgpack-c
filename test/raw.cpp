#include <msgpack.hpp>

#include <string>
#include <sstream>

#define BOOST_TEST_MODULE MSGPACK_RAW_REF
#include <boost/test/unit_test.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

BOOST_AUTO_TEST_CASE(pack_unpack)
{
    std::string s = "ABC";

    msgpack::type::raw_ref rr1(s.data(), static_cast<uint32_t>(s.size()));
    std::stringstream ss;
    msgpack::pack(ss, rr1);
    std::string packed_str = ss.str();
    BOOST_CHECK_EQUAL(packed_str[0], static_cast<char>(0xc4u));
    BOOST_CHECK_EQUAL(packed_str[1], static_cast<char>(0x03u));
    BOOST_CHECK_EQUAL(packed_str[2], 'A');
    BOOST_CHECK_EQUAL(packed_str[3], 'B');
    BOOST_CHECK_EQUAL(packed_str[4], 'C');

    msgpack::object_handle oh =
        msgpack::unpack(packed_str.data(), packed_str.size());
    msgpack::type::raw_ref rr2 = oh.get().as<msgpack::type::raw_ref>();
    BOOST_CHECK(rr1 == rr2);
}

BOOST_AUTO_TEST_CASE(pack_unpack_8_l)
{
    std::string s;

    msgpack::type::raw_ref rr1(s.data(), static_cast<uint32_t>(s.size()));
    std::stringstream ss;
    msgpack::pack(ss, rr1);
    std::string packed_str = ss.str();
    BOOST_CHECK_EQUAL(packed_str[0], static_cast<char>(0xc4u));
    BOOST_CHECK_EQUAL(packed_str[1], static_cast<char>(0x00u));

    msgpack::object_handle oh =
        msgpack::unpack(packed_str.data(), packed_str.size());
    msgpack::type::raw_ref rr2 = oh.get().as<msgpack::type::raw_ref>();
    BOOST_CHECK(rr1 == rr2);
}

BOOST_AUTO_TEST_CASE(pack_unpack_8_h)
{
    std::string s(0xff, 'A');

    msgpack::type::raw_ref rr1(s.data(), static_cast<uint32_t>(s.size()));
    std::stringstream ss;
    msgpack::pack(ss, rr1);
    std::string packed_str = ss.str();
    BOOST_CHECK_EQUAL(packed_str[0], static_cast<char>(0xc4u));
    BOOST_CHECK_EQUAL(packed_str[1], static_cast<char>(0xffu));
    BOOST_CHECK_EQUAL(packed_str[2], 'A');

    msgpack::object_handle oh =
        msgpack::unpack(packed_str.data(), packed_str.size());
    msgpack::type::raw_ref rr2 = oh.get().as<msgpack::type::raw_ref>();
    BOOST_CHECK(rr1 == rr2);
}

BOOST_AUTO_TEST_CASE(pack_unpack_16_l)
{
    std::string s(0xff+1, 'A');

    msgpack::type::raw_ref rr1(s.data(), static_cast<uint32_t>(s.size()));
    std::stringstream ss;
    msgpack::pack(ss, rr1);
    std::string packed_str = ss.str();
    BOOST_CHECK_EQUAL(packed_str[0], static_cast<char>(0xc5u));
    BOOST_CHECK_EQUAL(packed_str[1], static_cast<char>(0x01));
    BOOST_CHECK_EQUAL(packed_str[2], static_cast<char>(0x00));
    BOOST_CHECK_EQUAL(packed_str[3], 'A');

    msgpack::object_handle oh =
        msgpack::unpack(packed_str.data(), packed_str.size());
    msgpack::type::raw_ref rr2 = oh.get().as<msgpack::type::raw_ref>();
    BOOST_CHECK(rr1 == rr2);
}

BOOST_AUTO_TEST_CASE(pack_unpack_16_h)
{
    std::string s(0xffff, 'A');

    msgpack::type::raw_ref rr1(s.data(), static_cast<uint32_t>(s.size()));
    std::stringstream ss;
    msgpack::pack(ss, rr1);
    std::string packed_str = ss.str();
    BOOST_CHECK_EQUAL(packed_str[0], static_cast<char>(0xc5u));
    BOOST_CHECK_EQUAL(packed_str[1], static_cast<char>(0xffu));
    BOOST_CHECK_EQUAL(packed_str[2], static_cast<char>(0xffu));
    BOOST_CHECK_EQUAL(packed_str[3], 'A');

    msgpack::object_handle oh =
        msgpack::unpack(packed_str.data(), packed_str.size());
    msgpack::type::raw_ref rr2 = oh.get().as<msgpack::type::raw_ref>();
    BOOST_CHECK(rr1 == rr2);
}

BOOST_AUTO_TEST_CASE(pack_unpack_32_l)
{
    std::string s(0xffff+1, 'A');

    msgpack::type::raw_ref rr1(s.data(), static_cast<uint32_t>(s.size()));
    std::stringstream ss;
    msgpack::pack(ss, rr1);
    std::string packed_str = ss.str();
    BOOST_CHECK_EQUAL(packed_str[0], static_cast<char>(0xc6u));
    BOOST_CHECK_EQUAL(packed_str[1], static_cast<char>(0x00));
    BOOST_CHECK_EQUAL(packed_str[2], static_cast<char>(0x01));
    BOOST_CHECK_EQUAL(packed_str[3], static_cast<char>(0x00));
    BOOST_CHECK_EQUAL(packed_str[4], static_cast<char>(0x00));
    BOOST_CHECK_EQUAL(packed_str[5], 'A');

    msgpack::object_handle oh =
        msgpack::unpack(packed_str.data(), packed_str.size());
    msgpack::type::raw_ref rr2 = oh.get().as<msgpack::type::raw_ref>();
    BOOST_CHECK(rr1 == rr2);
}

BOOST_AUTO_TEST_CASE(v4_pack_unpack)
{
    std::string s = "ABC";

    msgpack::type::v4raw_ref rr1(s.data(), static_cast<uint32_t>(s.size()));
    std::stringstream ss;
    msgpack::pack(ss, rr1);
    std::string packed_str = ss.str();
    BOOST_CHECK_EQUAL(packed_str[0], static_cast<char>(0xa3u));
    BOOST_CHECK_EQUAL(packed_str[1], 'A');
    BOOST_CHECK_EQUAL(packed_str[2], 'B');
    BOOST_CHECK_EQUAL(packed_str[3], 'C');

    msgpack::object_handle oh =
        msgpack::unpack(packed_str.data(), packed_str.size());
    msgpack::type::v4raw_ref rr2 = oh.get().as<msgpack::type::v4raw_ref>();
    BOOST_CHECK(rr1 == rr2);
}

BOOST_AUTO_TEST_CASE(v4_pack_unpack_fix_l)
{
    std::string s;

    msgpack::type::v4raw_ref rr1(s.data(), static_cast<uint32_t>(s.size()));
    std::stringstream ss;
    msgpack::pack(ss, rr1);
    std::string packed_str = ss.str();
    BOOST_CHECK_EQUAL(packed_str[0], static_cast<char>(0xa0u));

    msgpack::object_handle oh =
        msgpack::unpack(packed_str.data(), packed_str.size());
    msgpack::type::v4raw_ref rr2 = oh.get().as<msgpack::type::v4raw_ref>();
    BOOST_CHECK(rr1 == rr2);
}

BOOST_AUTO_TEST_CASE(v4_pack_unpack_fix_h)
{
    std::string s(0x1f, 'A');

    msgpack::type::v4raw_ref rr1(s.data(), static_cast<uint32_t>(s.size()));
    std::stringstream ss;
    msgpack::pack(ss, rr1);
    std::string packed_str = ss.str();
    BOOST_CHECK_EQUAL(packed_str[0], static_cast<char>(0xbfu));
    BOOST_CHECK_EQUAL(packed_str[1], 'A');

    msgpack::object_handle oh =
        msgpack::unpack(packed_str.data(), packed_str.size());
    msgpack::type::v4raw_ref rr2 = oh.get().as<msgpack::type::v4raw_ref>();
    BOOST_CHECK(rr1 == rr2);
}

BOOST_AUTO_TEST_CASE(v4_pack_unpack_16_l)
{
    std::string s(0x1f+1, 'A');

    msgpack::type::v4raw_ref rr1(s.data(), static_cast<uint32_t>(s.size()));
    std::stringstream ss;
    msgpack::pack(ss, rr1);
    std::string packed_str = ss.str();
    BOOST_CHECK_EQUAL(packed_str[0], static_cast<char>(0xdau));
    BOOST_CHECK_EQUAL(packed_str[1], static_cast<char>(0x00u));
    BOOST_CHECK_EQUAL(packed_str[2], static_cast<char>(0x20u));
    BOOST_CHECK_EQUAL(packed_str[3], 'A');

    msgpack::object_handle oh =
        msgpack::unpack(packed_str.data(), packed_str.size());
    msgpack::type::v4raw_ref rr2 = oh.get().as<msgpack::type::v4raw_ref>();
    BOOST_CHECK(rr1 == rr2);
}

BOOST_AUTO_TEST_CASE(v4_pack_unpack_16_h)
{
    std::string s(0xffff, 'A');

    msgpack::type::v4raw_ref rr1(s.data(), static_cast<uint32_t>(s.size()));
    std::stringstream ss;
    msgpack::pack(ss, rr1);
    std::string packed_str = ss.str();
    BOOST_CHECK_EQUAL(packed_str[0], static_cast<char>(0xdau));
    BOOST_CHECK_EQUAL(packed_str[1], static_cast<char>(0xffu));
    BOOST_CHECK_EQUAL(packed_str[2], static_cast<char>(0xffu));
    BOOST_CHECK_EQUAL(packed_str[3], 'A');

    msgpack::object_handle oh =
        msgpack::unpack(packed_str.data(), packed_str.size());
    msgpack::type::v4raw_ref rr2 = oh.get().as<msgpack::type::v4raw_ref>();
    BOOST_CHECK(rr1 == rr2);
}

BOOST_AUTO_TEST_CASE(v4_pack_unpack_32_l)
{
    std::string s(0xffff+1, 'A');

    msgpack::type::v4raw_ref rr1(s.data(), static_cast<uint32_t>(s.size()));
    std::stringstream ss;
    msgpack::pack(ss, rr1);
    std::string packed_str = ss.str();
    BOOST_CHECK_EQUAL(packed_str[0], static_cast<char>(0xdbu));
    BOOST_CHECK_EQUAL(packed_str[1], static_cast<char>(0x00));
    BOOST_CHECK_EQUAL(packed_str[2], static_cast<char>(0x01));
    BOOST_CHECK_EQUAL(packed_str[3], static_cast<char>(0x00));
    BOOST_CHECK_EQUAL(packed_str[4], static_cast<char>(0x00));
    BOOST_CHECK_EQUAL(packed_str[5], 'A');

    msgpack::object_handle oh =
        msgpack::unpack(packed_str.data(), packed_str.size());
    msgpack::type::v4raw_ref rr2 = oh.get().as<msgpack::type::v4raw_ref>();
    BOOST_CHECK(rr1 == rr2);
}
