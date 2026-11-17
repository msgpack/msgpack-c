#include <msgpack.hpp>

#define BOOST_TEST_MODULE pack_unpack
#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_CASE(pack_num)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
}


BOOST_AUTO_TEST_CASE(pack_vector)
{
    msgpack::sbuffer sbuf;
    std::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    msgpack::pack(sbuf, vec);
}


BOOST_AUTO_TEST_CASE(pack_to_ostream)
{
    std::ostringstream stream;
    msgpack::pack(stream, 1);
}


struct myclass {
    myclass() : num(0), str("default") { }

    myclass(int num, const std::string& str) :
        num(num), str(str) { }

    ~myclass() { }

    int num;
    std::string str;

    MSGPACK_DEFINE(num, str);
};


BOOST_AUTO_TEST_CASE(pack_myclass)
{
    msgpack::sbuffer sbuf;
    myclass m(1, "msgpack");
    msgpack::pack(sbuf, m);
}


BOOST_AUTO_TEST_CASE(unpack_int_ret_no_offset_no_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);

    msgpack::object_handle oh = msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(1, oh.get().as<int>());
}

BOOST_AUTO_TEST_CASE(unpack_int_ret_offset_no_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);

    std::size_t off = 0;

    msgpack::object_handle oh = msgpack::unpack(sbuf.data(), sbuf.size(), off);
    BOOST_CHECK_EQUAL(1, oh.get().as<int>());
    BOOST_CHECK_EQUAL(off, sbuf.size());
}

BOOST_AUTO_TEST_CASE(unpack_int_ret_no_offset_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    bool referenced;

    msgpack::object_handle oh = msgpack::unpack(sbuf.data(), sbuf.size(), referenced);
    BOOST_CHECK_EQUAL(1, oh.get().as<int>());
    BOOST_CHECK(!referenced);
}

BOOST_AUTO_TEST_CASE(unpack_int_ret_offset_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    std::size_t off = 0;
    bool referenced;

    msgpack::object_handle oh = msgpack::unpack(sbuf.data(), sbuf.size(), off, referenced);
    BOOST_CHECK_EQUAL(1, oh.get().as<int>());
    BOOST_CHECK(!referenced);
    BOOST_CHECK_EQUAL(off, sbuf.size());
}


BOOST_AUTO_TEST_CASE(unpack_int_no_offset_no_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::object_handle oh;

    msgpack::unpack(oh, sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(1, oh.get().as<int>());
}

BOOST_AUTO_TEST_CASE(unpack_int_offset_no_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::object_handle oh;

    std::size_t off = 0;

    msgpack::unpack(oh, sbuf.data(), sbuf.size(), off);
    BOOST_CHECK_EQUAL(1, oh.get().as<int>());
    BOOST_CHECK_EQUAL(off, sbuf.size());
}

BOOST_AUTO_TEST_CASE(unpack_int_no_offset_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::object_handle oh;
    bool referenced;

    msgpack::unpack(oh, sbuf.data(), sbuf.size(), referenced);
    BOOST_CHECK_EQUAL(1, oh.get().as<int>());
    BOOST_CHECK(!referenced);
}

BOOST_AUTO_TEST_CASE(unpack_int_offset_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::object_handle oh;
    std::size_t off = 0;
    bool referenced;

    msgpack::unpack(oh, sbuf.data(), sbuf.size(), off, referenced);
    BOOST_CHECK_EQUAL(1, oh.get().as<int>());
    BOOST_CHECK(!referenced);
    BOOST_CHECK_EQUAL(off, sbuf.size());
}

#if MSGPACK_DEFAULT_API_VERSION == 1

BOOST_AUTO_TEST_CASE(unpack_int_pointer_off_no_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::object_handle oh;

    std::size_t off = 0;

    // obsolete
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
    msgpack::unpack(&oh, sbuf.data(), sbuf.size(), &off);
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
    BOOST_CHECK_EQUAL(1, oh.get().as<int>());
    BOOST_CHECK_EQUAL(off, sbuf.size());
}

BOOST_AUTO_TEST_CASE(unpack_int_pointer_off_no_ref_explicit)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::object_handle oh;

    std::size_t off = 0;

    // obsolete
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
    msgpack::unpack(&oh, sbuf.data(), sbuf.size(), &off, MSGPACK_NULLPTR);
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
    BOOST_CHECK_EQUAL(1, oh.get().as<int>());
    BOOST_CHECK_EQUAL(off, sbuf.size());
}

BOOST_AUTO_TEST_CASE(unpack_int_pointer_no_off_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::object_handle oh;
    bool referenced;

    // obsolete
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
    msgpack::unpack(&oh, sbuf.data(), sbuf.size(), MSGPACK_NULLPTR, &referenced);
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
    BOOST_CHECK_EQUAL(1, oh.get().as<int>());
    BOOST_CHECK(!referenced);
}

BOOST_AUTO_TEST_CASE(unpack_int_pointer_off_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::object_handle oh;
    bool referenced;
    std::size_t off = 0;

    // obsolete
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
    msgpack::unpack(&oh, sbuf.data(), sbuf.size(), &off, &referenced);
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
    BOOST_CHECK_EQUAL(1, oh.get().as<int>());
    BOOST_CHECK_EQUAL(off, sbuf.size());
    BOOST_CHECK(!referenced);
}


BOOST_AUTO_TEST_CASE(unpack_int_default_null_pointer)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::object_handle oh;

    // obsolete
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
    msgpack::unpack(&oh, sbuf.data(), sbuf.size());
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
    BOOST_CHECK_EQUAL(1, oh.get().as<int>());
}

#endif // MSGPACK_DEFAULT_API_VERSION == 1

BOOST_AUTO_TEST_CASE(unpack_int_zone_no_offset_no_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);

    msgpack::zone z;
    msgpack::object obj = msgpack::unpack(z, sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(1, obj.as<int>());
}

BOOST_AUTO_TEST_CASE(unpack_int_zone_offset_no_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);

    std::size_t off = 0;

    msgpack::zone z;
    msgpack::object obj = msgpack::unpack(z, sbuf.data(), sbuf.size(), off);
    BOOST_CHECK_EQUAL(1, obj.as<int>());
    BOOST_CHECK_EQUAL(off, sbuf.size());
}

BOOST_AUTO_TEST_CASE(unpack_int_zone_no_offset_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    bool referenced;

    msgpack::zone z;
    msgpack::object obj = msgpack::unpack(z, sbuf.data(), sbuf.size(), referenced);
    BOOST_CHECK_EQUAL(1, obj.as<int>());
    BOOST_CHECK(!referenced);
}

BOOST_AUTO_TEST_CASE(unpack_int_zone_offset_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    std::size_t off = 0;
    bool referenced;

    msgpack::zone z;
    msgpack::object obj = msgpack::unpack(z, sbuf.data(), sbuf.size(), off, referenced);
    BOOST_CHECK_EQUAL(1, obj.as<int>());
    BOOST_CHECK(!referenced);
    BOOST_CHECK_EQUAL(off, sbuf.size());
}


BOOST_AUTO_TEST_CASE(unpack_sequence)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::pack(sbuf, 2);
    msgpack::pack(sbuf, 3);

    std::size_t off = 0;

    msgpack::object_handle oh;

    msgpack::unpack(oh, sbuf.data(), sbuf.size(), off);
    BOOST_CHECK_EQUAL(1, oh.get().as<int>());

    msgpack::unpack(oh, sbuf.data(), sbuf.size(), off);
    BOOST_CHECK_EQUAL(2, oh.get().as<int>());

    msgpack::unpack(oh, sbuf.data(), sbuf.size(), off);
    BOOST_CHECK_EQUAL(3, oh.get().as<int>());

    BOOST_CHECK_EQUAL(off, sbuf.size());
}


BOOST_AUTO_TEST_CASE(unpack_convert_to_object_handle)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::unpacked msg;

    msgpack::unpack(msg, sbuf.data(), sbuf.size());
    msgpack::object_handle oh(msgpack::move(msg));
    BOOST_CHECK_EQUAL(1, oh.get().as<int>());

}

BOOST_AUTO_TEST_CASE(unpack_convert_to_object_handle_direct)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::object_handle oh(msgpack::unpack(sbuf.data(), sbuf.size()));
    BOOST_CHECK_EQUAL(1, oh.get().as<int>());

}

BOOST_AUTO_TEST_CASE(unpack_convert_to_object_handle_direct_implicit)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::object_handle oh = msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(1, oh.get().as<int>());

}

BOOST_AUTO_TEST_CASE(unpack_insufficient_bytes_ref)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 255); // uint8 (2bytes)

    std::size_t off = 0;

    msgpack::object_handle oh;
    try {
        msgpack::unpack(oh, sbuf.data(), 1, off);
        BOOST_CHECK(false);
    }
    catch (msgpack::insufficient_bytes const&) {
        BOOST_CHECK(true);
#if MSGPACK_DEFAULT_API_VERSION < 3
        BOOST_CHECK_EQUAL(off, 0u);
#else  // MSGPACK_DEFAULT_API_VERSION < 3
        BOOST_CHECK_EQUAL(1u, off);
#endif // MSGPACK_DEFAULT_API_VERSION < 3
    }
}

BOOST_AUTO_TEST_CASE(unpack_insufficient_bytes_object_handle)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 255); // uint8 (2bytes)

    std::size_t off = 0;

    try {
        msgpack::object_handle oh(msgpack::unpack(sbuf.data(), 1, off));
        BOOST_CHECK(false);
    }
    catch (msgpack::insufficient_bytes const&) {
        BOOST_CHECK(true);
#if MSGPACK_DEFAULT_API_VERSION < 3
        BOOST_CHECK_EQUAL(off, 0u);
#else  // MSGPACK_DEFAULT_API_VERSION < 3
        BOOST_CHECK_EQUAL(1u, off);
#endif // MSGPACK_DEFAULT_API_VERSION < 3
    }
}

BOOST_AUTO_TEST_CASE(unpack_insufficient_bytes_zone)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 255); // uint8 (2bytes)

    std::size_t off = 0;

    try {
        msgpack::zone z;
        msgpack::unpack(z, sbuf.data(), 1, off);
        BOOST_CHECK(false);
    }
    catch (msgpack::insufficient_bytes const&) {
        BOOST_CHECK(true);
#if MSGPACK_DEFAULT_API_VERSION < 3
        BOOST_CHECK_EQUAL(off, 0u);
#else  // MSGPACK_DEFAULT_API_VERSION < 3
        BOOST_CHECK_EQUAL(1u, off);
#endif // MSGPACK_DEFAULT_API_VERSION < 3
    }
}

BOOST_AUTO_TEST_CASE(unpack_parse_error)
{
    msgpack::sbuffer sbuf;

    char c = '\xc1';
    sbuf.write(&c, 1);

    bool thrown = false;
    msgpack::object_handle oh;
    try {
        msgpack::unpack(oh, sbuf.data(), sbuf.size());
        BOOST_CHECK(false);
    }
    catch (msgpack::parse_error const&) {
        thrown = true;
    }
    BOOST_CHECK(thrown);
}

BOOST_AUTO_TEST_CASE(unpack_returned_parse_error)
{
    msgpack::sbuffer sbuf;

    char c = '\xc1';
    sbuf.write(&c, 1);

    bool thrown = false;
    try {
        msgpack::unpack(sbuf.data(), sbuf.size());
        BOOST_CHECK(false);
    }
    catch (msgpack::parse_error const&) {
        thrown = true;
    }
    BOOST_CHECK(thrown);
}

BOOST_AUTO_TEST_CASE(unpack_zone_parse_error)
{
    msgpack::sbuffer sbuf;

    char c = '\xc1';
    sbuf.write(&c, 1);

    bool thrown = false;
    msgpack::zone z;
    try {
        msgpack::unpack(z, sbuf.data(), sbuf.size());
        BOOST_CHECK(false);
    }
    catch (msgpack::parse_error const&) {
        thrown = true;
    }
    BOOST_CHECK(thrown);
}

BOOST_AUTO_TEST_CASE(unpack_extra_bytes)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);

    msgpack::object_handle oh = msgpack::unpack(sbuf.data(), sbuf.size() + 1);
    BOOST_CHECK_EQUAL(1, oh.get().as<int>());
}

BOOST_AUTO_TEST_CASE(unpack_zone_extra_bytes)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);

    msgpack::zone z;
    msgpack::object obj = msgpack::unpack(z, sbuf.data(), sbuf.size() + 1);
    BOOST_CHECK_EQUAL(1, obj.as<int>());
}

BOOST_AUTO_TEST_CASE(unpack_int_off_larger_than_length)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);

    std::size_t off = 2;

    bool thrown = false;
    try {
        msgpack::object_handle oh = msgpack::unpack(sbuf.data(), sbuf.size(), off);
    }
    catch (msgpack::insufficient_bytes const&) {
        thrown = true;
    }
    BOOST_CHECK(thrown);
    BOOST_CHECK_EQUAL(off, 2u);
}

BOOST_AUTO_TEST_CASE(unpack_empty_array_fix)
{
    std::string buf;
    buf.push_back(static_cast<char>(0x90u));
    std::size_t off = 0;

    msgpack::object_handle oh = msgpack::unpack(buf.data(), buf.size(), off);
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);
    BOOST_CHECK_EQUAL(oh.get().via.array.size, 0u);
    BOOST_CHECK_EQUAL(off, 1u);
}

BOOST_AUTO_TEST_CASE(unpack_empty_array_16)
{
    std::string buf;
    buf.push_back(static_cast<char>(0xdcu));
    buf.push_back(static_cast<char>(0x00));
    buf.push_back(static_cast<char>(0x00));
    std::size_t off = 0;

    msgpack::object_handle oh = msgpack::unpack(buf.data(), buf.size(), off);
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);
    BOOST_CHECK_EQUAL(oh.get().via.array.size, 0u);
    BOOST_CHECK_EQUAL(off, 3u);
}

BOOST_AUTO_TEST_CASE(unpack_empty_array_32)
{
    std::string buf;
    buf.push_back(static_cast<char>(0xddu));
    buf.push_back(static_cast<char>(0x00));
    buf.push_back(static_cast<char>(0x00));
    buf.push_back(static_cast<char>(0x00));
    buf.push_back(static_cast<char>(0x00));
    std::size_t off = 0;

    msgpack::object_handle oh = msgpack::unpack(buf.data(), buf.size(), off);
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::ARRAY);
    BOOST_CHECK_EQUAL(oh.get().via.array.size, 0u);
    BOOST_CHECK_EQUAL(off, 5u);
}

BOOST_AUTO_TEST_CASE(unpack_empty_map_fix)
{
    std::string buf;
    buf.push_back(static_cast<char>(0x80u));
    std::size_t off = 0;

    msgpack::object_handle oh = msgpack::unpack(buf.data(), buf.size(), off);
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::MAP);
    BOOST_CHECK_EQUAL(oh.get().via.map.size, 0u);
    BOOST_CHECK_EQUAL(off, 1u);
}

BOOST_AUTO_TEST_CASE(unpack_empty_map_16)
{
    std::string buf;
    buf.push_back(static_cast<char>(0xdeu));
    buf.push_back(static_cast<char>(0x00));
    buf.push_back(static_cast<char>(0x00));
    std::size_t off = 0;

    msgpack::object_handle oh = msgpack::unpack(buf.data(), buf.size(), off);
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::MAP);
    BOOST_CHECK_EQUAL(oh.get().via.map.size, 0u);
    BOOST_CHECK_EQUAL(off, 3u);
}

BOOST_AUTO_TEST_CASE(unpack_empty_map_32)
{
    std::string buf;
    buf.push_back(static_cast<char>(0xdfu));
    buf.push_back(static_cast<char>(0x00));
    buf.push_back(static_cast<char>(0x00));
    buf.push_back(static_cast<char>(0x00));
    buf.push_back(static_cast<char>(0x00));
    std::size_t off = 0;

    msgpack::object_handle oh = msgpack::unpack(buf.data(), buf.size(), off);
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::MAP);
    BOOST_CHECK_EQUAL(oh.get().via.map.size, 0u);
    BOOST_CHECK_EQUAL(off, 5u);
}

// Nesting deeper than MSGPACK_EMBED_STACK_SIZE: the parse stacks keep that many levels inline and the
// rest in a vector.
static std::string nested_array_and_map(std::size_t depth)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> pk(sbuf);
    for (std::size_t i = 0; i < depth; ++i) {
        if (i % 2 == 0) pk.pack_array(2);
        else { pk.pack_map(1); pk.pack(static_cast<int>(i)); }
    }
    pk.pack(42);
    for (std::size_t i = depth; i-- > 0; ) {    // the second element of each array, innermost first
        if (i % 2 == 0) pk.pack(static_cast<int>(i));
    }
    return std::string(sbuf.data(), sbuf.size());
}

static void check_nested_array_and_map(msgpack::object const* o, std::size_t depth)
{
    for (std::size_t i = 0; i < depth; ++i) {
        if (i % 2 == 0) {
            BOOST_REQUIRE_EQUAL(o->type, msgpack::type::ARRAY);
            BOOST_REQUIRE_EQUAL(o->via.array.size, 2u);
            BOOST_CHECK_EQUAL(o->via.array.ptr[1].as<int>(), static_cast<int>(i));
            o = &o->via.array.ptr[0];
        }
        else {
            BOOST_REQUIRE_EQUAL(o->type, msgpack::type::MAP);
            BOOST_REQUIRE_EQUAL(o->via.map.size, 1u);
            BOOST_CHECK_EQUAL(o->via.map.ptr[0].key.as<int>(), static_cast<int>(i));
            o = &o->via.map.ptr[0].val;
        }
    }
    BOOST_CHECK_EQUAL(o->as<int>(), 42);
}

BOOST_AUTO_TEST_CASE(unpack_deeply_nested)
{
    std::size_t const depths[] = { 1, MSGPACK_EMBED_STACK_SIZE - 1, MSGPACK_EMBED_STACK_SIZE,
                                   MSGPACK_EMBED_STACK_SIZE + 1, 3 * MSGPACK_EMBED_STACK_SIZE + 7 };
    for (std::size_t k = 0; k < sizeof(depths) / sizeof(depths[0]); ++k) {
        std::string buf = nested_array_and_map(depths[k]);
        msgpack::object_handle oh = msgpack::unpack(buf.data(), buf.size());
        check_nested_array_and_map(&oh.get(), depths[k]);

        // the same, one byte at a time through the streaming unpacker
        msgpack::unpacker unp;
        msgpack::object_handle soh;
        bool done = false;
        for (std::size_t i = 0; i < buf.size(); ++i) {
            unp.reserve_buffer(1);
            unp.buffer()[0] = buf[i];
            unp.buffer_consumed(1);
            if (unp.next(soh)) { BOOST_CHECK_EQUAL(i, buf.size() - 1); done = true; }
        }
        BOOST_REQUIRE(done);
        check_nested_array_and_map(&soh.get(), depths[k]);
    }
}
