#include <msgpack.hpp>

#define BOOST_TEST_MODULE reference
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(unpack_int)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    bool referenced = false;

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size(), referenced);
    BOOST_CHECK(!referenced);
}

BOOST_AUTO_TEST_CASE(unpack_string)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, std::string("abcdefg"));
    bool referenced = false;

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size(), referenced);
    BOOST_CHECK(!referenced);
}

BOOST_AUTO_TEST_CASE(unpack_bin)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char c[] = { 1, 2, 3, 4, 5, 6 };
    packer.pack_bin(sizeof(c));
    packer.pack_bin_body(c, sizeof(c));

    bool referenced = false;
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size(), referenced);
    BOOST_CHECK(!referenced);
}

BOOST_AUTO_TEST_CASE(unpack_ext)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char const buf [] = { 2 };

    packer.pack_ext(sizeof(buf), 1);
    packer.pack_ext_body(buf, sizeof(buf));
    bool referenced = false;
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size(), referenced);
    BOOST_CHECK(!referenced);
}

bool never_called(msgpack::type::object_type, std::size_t, void*)
{
    BOOST_CHECK(false);
    return false;
}

bool always_reference(msgpack::type::object_type, std::size_t, void*)
{
    return true;
}

BOOST_AUTO_TEST_CASE(unpack_int_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    bool referenced = false;

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size(), referenced, never_called);
    BOOST_CHECK(!referenced);
}

BOOST_AUTO_TEST_CASE(unpack_string_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, std::string("abcdefg"));
    bool referenced = false;

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size(), referenced, always_reference);
    BOOST_CHECK(referenced);
}

BOOST_AUTO_TEST_CASE(unpack_bin_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char c[] = { 1, 2, 3, 4, 5, 6 };
    packer.pack_bin(sizeof(c));
    packer.pack_bin_body(c, sizeof(c));

    bool referenced = false;
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size(), referenced, always_reference);
    BOOST_CHECK(referenced);
}

BOOST_AUTO_TEST_CASE(unpack_ext_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char const buf [] = { 2 };

    packer.pack_ext(sizeof(buf), 1);
    packer.pack_ext_body(buf, sizeof(buf));
    bool referenced = false;

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size(), referenced, always_reference);
    BOOST_CHECK(referenced);
}

static void* s_p;

bool sized_reference(msgpack::type::object_type t, std::size_t s, void* p)
{
    s_p = p;
    switch (t) {
    case msgpack::type::STR:
        if (s >= 5) return true;
        break;
    case msgpack::type::BIN:
        if (s >= 6) return true;
        break;
    case msgpack::type::EXT:
        if (s >= 7) return true;
        break;
    default:
        BOOST_CHECK(false);
    }
    return false;
}

BOOST_AUTO_TEST_CASE(unpack_int_sized_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);

    bool referenced = false;
    s_p = MSGPACK_NULLPTR;

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size(), referenced, never_called, &sbuf);
    BOOST_CHECK(!referenced);
    BOOST_CHECK(MSGPACK_NULLPTR == s_p);
}

BOOST_AUTO_TEST_CASE(unpack_string_sized_ref_4)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, std::string("1234"));

    bool referenced = false;
    s_p = MSGPACK_NULLPTR;
    // the last argument sbuf is any pointer as a user data.
    // That is stored to s_p in sized_reference
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size(), referenced, sized_reference, &sbuf);
    BOOST_CHECK(!referenced);
    // compare the passed argument with stored s_p.
    BOOST_CHECK_EQUAL(&sbuf, s_p);
}

BOOST_AUTO_TEST_CASE(unpack_string_sized_ref_5)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, std::string("12345"));

    bool referenced = false;
    s_p = MSGPACK_NULLPTR;

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size(), referenced, sized_reference, &sbuf);
    BOOST_CHECK(referenced);
    BOOST_CHECK_EQUAL(&sbuf, s_p);
}


BOOST_AUTO_TEST_CASE(unpack_bin_sized_ref_5)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char c[] = { 1, 2, 3, 4, 5 };
    packer.pack_bin(sizeof(c));
    packer.pack_bin_body(c, sizeof(c));

    bool referenced = false;
    s_p = MSGPACK_NULLPTR;

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size(), referenced, sized_reference, &sbuf);
    BOOST_CHECK(!referenced);
    BOOST_CHECK_EQUAL(&sbuf, s_p);
}

BOOST_AUTO_TEST_CASE(unpack_bin_sized_ref_6)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char c[] = { 1, 2, 3, 4, 5, 6 };
    packer.pack_bin(sizeof(c));
    packer.pack_bin_body(c, sizeof(c));

    bool referenced = false;
    s_p = MSGPACK_NULLPTR;
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size(), referenced, sized_reference, &sbuf);
    BOOST_CHECK(referenced);
    BOOST_CHECK_EQUAL(&sbuf, s_p);
}

BOOST_AUTO_TEST_CASE(unpack_ext_sized_ref_6)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char const buf [] = { 1, 2, 3, 4, 5 };

    packer.pack_ext(sizeof(buf), 1); // 5 + 1(type) = 6
    packer.pack_ext_body(buf, sizeof(buf));

    bool referenced = false;
    s_p = MSGPACK_NULLPTR;
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size(), referenced, sized_reference, &sbuf);
    BOOST_CHECK(!referenced);
    BOOST_CHECK_EQUAL(&sbuf, s_p);
}

BOOST_AUTO_TEST_CASE(unpack_ext_sized_ref_7)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char const buf [] = { 1, 2, 3, 4, 5, 6 };

    packer.pack_ext(sizeof(buf), 1); // 6 + 1(type) = 7
    packer.pack_ext_body(buf, sizeof(buf));

    bool referenced = false;
    s_p = MSGPACK_NULLPTR;
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size(), referenced, sized_reference, &sbuf);
    BOOST_CHECK(referenced);
    BOOST_CHECK_EQUAL(&sbuf, s_p);
}

// default reference function
// STR, BIN, and EXT are always referenced, otherwise copied (converted).

BOOST_AUTO_TEST_CASE(unpacker_int)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);

    msgpack::unpacker unp;
    msgpack::object_handle oh;
    bool referenced = false;
    unp.reserve_buffer(sbuf.size());
    std::memcpy(unp.buffer(), sbuf.data(), sbuf.size());
    unp.buffer_consumed(sbuf.size());
    bool b = unp.next(oh, referenced);
    BOOST_CHECK(b);
    BOOST_CHECK(!referenced);
}

BOOST_AUTO_TEST_CASE(unpacker_string)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, std::string("a"));

    msgpack::unpacker unp;
    msgpack::object_handle oh;
    bool referenced = false;
    unp.reserve_buffer(sbuf.size());
    std::memcpy(unp.buffer(), sbuf.data(), sbuf.size());
    unp.buffer_consumed(sbuf.size());
    bool b = unp.next(oh, referenced);
    BOOST_CHECK(b);
    BOOST_CHECK(referenced);
}

BOOST_AUTO_TEST_CASE(unpacker_bin)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char c[] = { 1 };
    packer.pack_bin(sizeof(c));
    packer.pack_bin_body(c, sizeof(c));

    msgpack::unpacker unp;
    msgpack::object_handle oh;
    bool referenced = false;
    unp.reserve_buffer(sbuf.size());
    std::memcpy(unp.buffer(), sbuf.data(), sbuf.size());
    unp.buffer_consumed(sbuf.size());
    bool b = unp.next(oh, referenced);
    BOOST_CHECK(b);
    BOOST_CHECK(referenced);
}

BOOST_AUTO_TEST_CASE(unpacker_ext)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char const buf [] = { 2 };

    packer.pack_ext(sizeof(buf), 1);
    packer.pack_ext_body(buf, sizeof(buf));

    msgpack::unpacker unp;
    msgpack::object_handle oh;
    bool referenced = false;
    unp.reserve_buffer(sbuf.size());
    std::memcpy(unp.buffer(), sbuf.data(), sbuf.size());
    unp.buffer_consumed(sbuf.size());
    bool b = unp.next(oh, referenced);
    BOOST_CHECK(b);
    BOOST_CHECK(referenced);
}

// pass user custom reference function

BOOST_AUTO_TEST_CASE(unpacker_int_sized_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);

    msgpack::unpacker unp(never_called, &sbuf);
    msgpack::object_handle oh;
    bool referenced = false;
    unp.reserve_buffer(sbuf.size());
    std::memcpy(unp.buffer(), sbuf.data(), sbuf.size());
    unp.buffer_consumed(sbuf.size());
    s_p = MSGPACK_NULLPTR;
    bool b = unp.next(oh, referenced);
    BOOST_CHECK(b);
    BOOST_CHECK(!referenced);
    BOOST_CHECK(MSGPACK_NULLPTR == s_p);
}

BOOST_AUTO_TEST_CASE(unpacker_string_sized_ref_4)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, std::string("1234"));

    msgpack::unpacker unp(sized_reference, &sbuf);
    msgpack::object_handle oh;
    bool referenced = false;
    unp.reserve_buffer(sbuf.size());
    std::memcpy(unp.buffer(), sbuf.data(), sbuf.size());
    unp.buffer_consumed(sbuf.size());
    s_p = MSGPACK_NULLPTR;
    bool b = unp.next(oh, referenced);
    BOOST_CHECK(b);
    BOOST_CHECK(!referenced);
    BOOST_CHECK_EQUAL(&sbuf, s_p);
}

BOOST_AUTO_TEST_CASE(unpacker_string_sized_ref_5)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, std::string("12345"));

    msgpack::unpacker unp(sized_reference, &sbuf);
    msgpack::object_handle oh;
    bool referenced = false;
    unp.reserve_buffer(sbuf.size());
    std::memcpy(unp.buffer(), sbuf.data(), sbuf.size());
    unp.buffer_consumed(sbuf.size());
    s_p = MSGPACK_NULLPTR;
    bool b = unp.next(oh, referenced);
    BOOST_CHECK(b);
    BOOST_CHECK(referenced);
    BOOST_CHECK_EQUAL(&sbuf, s_p);
}


BOOST_AUTO_TEST_CASE(unpacker_bin_sized_ref_5)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char c[] = { 1, 2, 3, 4, 5 };
    packer.pack_bin(sizeof(c));
    packer.pack_bin_body(c, sizeof(c));

    msgpack::unpacker unp(sized_reference, &sbuf);
    msgpack::object_handle oh;
    bool referenced = false;
    unp.reserve_buffer(sbuf.size());
    std::memcpy(unp.buffer(), sbuf.data(), sbuf.size());
    unp.buffer_consumed(sbuf.size());
    s_p = MSGPACK_NULLPTR;
    bool b = unp.next(oh, referenced);
    BOOST_CHECK(b);
    BOOST_CHECK(!referenced);
    BOOST_CHECK_EQUAL(&sbuf, s_p);
}

BOOST_AUTO_TEST_CASE(unpacker_bin_sized_ref_6)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char c[] = { 1, 2, 3, 4, 5, 6 };
    packer.pack_bin(sizeof(c));
    packer.pack_bin_body(c, sizeof(c));

    msgpack::unpacker unp(sized_reference, &sbuf);
    msgpack::object_handle oh;
    bool referenced = false;
    unp.reserve_buffer(sbuf.size());
    std::memcpy(unp.buffer(), sbuf.data(), sbuf.size());
    unp.buffer_consumed(sbuf.size());
    s_p = MSGPACK_NULLPTR;
    bool b = unp.next(oh, referenced);
    BOOST_CHECK(b);
    BOOST_CHECK(referenced);
    BOOST_CHECK_EQUAL(&sbuf, s_p);
}

BOOST_AUTO_TEST_CASE(unpacker_ext_sized_ref_6)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char const buf [] = { 1, 2, 3, 4, 5 };

    packer.pack_ext(sizeof(buf), 1); // 5 + 1(type) = 6
    packer.pack_ext_body(buf, sizeof(buf));

    msgpack::unpacker unp(sized_reference, &sbuf);
    msgpack::object_handle oh;
    bool referenced = false;
    unp.reserve_buffer(sbuf.size());
    std::memcpy(unp.buffer(), sbuf.data(), sbuf.size());
    unp.buffer_consumed(sbuf.size());
    s_p = MSGPACK_NULLPTR;
    bool b = unp.next(oh, referenced);
    BOOST_CHECK(b);
    BOOST_CHECK(!referenced);
    BOOST_CHECK_EQUAL(&sbuf, s_p);
}

BOOST_AUTO_TEST_CASE(unpacker_ext_sized_ref_7)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char const buf [] = { 1, 2, 3, 4, 5, 6 };

    packer.pack_ext(sizeof(buf), 1); // 6 + 1(type) = 7
    packer.pack_ext_body(buf, sizeof(buf));

    msgpack::unpacker unp(sized_reference, &sbuf);
    msgpack::object_handle oh;
    bool referenced = false;
    unp.reserve_buffer(sbuf.size());
    std::memcpy(unp.buffer(), sbuf.data(), sbuf.size());
    unp.buffer_consumed(sbuf.size());
    s_p = MSGPACK_NULLPTR;
    bool b = unp.next(oh, referenced);
    BOOST_CHECK(b);
    BOOST_CHECK(referenced);
    BOOST_CHECK_EQUAL(&sbuf, s_p);
}
