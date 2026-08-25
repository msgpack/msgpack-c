#include <msgpack.hpp>

#define BOOST_TEST_MODULE security_fixes
#include <boost/test/unit_test.hpp>

#include <array>
#include <chrono>
#include <cstring>
#include <limits>
#include <string>
#include <tuple>
#include <vector>

// Regression tests for memory-safety / correctness fixes.
// See the audit that accompanied the msgpack_unpacker_expand_buffer overflow fix.

// A1: convert of an empty msgpack array into a C array must not dereference a
// null/oversized pointer. size < N leaves the remaining elements untouched;
// size > N throws.
BOOST_AUTO_TEST_CASE(carray_empty_array_no_crash)
{
    msgpack::object_handle oh = msgpack::unpack("\x90", 1); // empty fixarray
    int v[3] = {7, 8, 9};
    oh.get().convert(v); // must not crash
    BOOST_CHECK_EQUAL(v[0], 7);

    msgpack::object_handle oh2 = msgpack::unpack("\x94\x01\x02\x03\x04", 5); // 4 elems
    int v2[3];
    BOOST_CHECK_THROW(oh2.get().convert(v2), msgpack::type_error);
}

// A2: convert of an empty array into msgpack::type::tuple must not dereference
// a null pointer (the N==1 base specialization was missing the size guard).
BOOST_AUTO_TEST_CASE(msgpack_tuple_convert_empty_array)
{
    msgpack::object_handle oh = msgpack::unpack("\x90", 1);
    msgpack::type::tuple<int> t;
    oh.get().convert(t); // must not crash
    BOOST_CHECK(true);
}

// A3: as<tuple> on an array shorter than the tuple must throw, not read OOB
// (index computation size - sizeof...(Args) - 1 used to underflow).
BOOST_AUTO_TEST_CASE(tuple_as_short_array_throws)
{
    using tp = std::chrono::system_clock::time_point;
    const char b[] = "\x91\x01"; // [1]
    msgpack::object_handle oh = msgpack::unpack(b, 2);
    BOOST_CHECK_THROW((oh.get().as<std::tuple<tp, tp, tp> >()), msgpack::type_error);
    BOOST_CHECK_THROW((oh.get().as<msgpack::type::tuple<tp, tp, tp> >()), msgpack::type_error);
}

// A4: converting a short STR into std::array<char, N> must copy only str.size
// bytes, not N (which over-read the source).
BOOST_AUTO_TEST_CASE(array_char_str_no_overread)
{
    msgpack::sbuffer sb;
    msgpack::pack(sb, std::string("ab"));
    msgpack::object_handle oh = msgpack::unpack(sb.data(), sb.size());
    std::array<char, 8> a;
    a.fill('Z');
    oh.get().convert(a);
    BOOST_CHECK_EQUAL(a[0], 'a');
    BOOST_CHECK_EQUAL(a[1], 'b');
    BOOST_CHECK_EQUAL(a[2], 'Z'); // untouched
}

// B1: moving a (v2) unpacker must not leave the parser referencing the
// moved-from object's buffer hook.
BOOST_AUTO_TEST_CASE(unpacker_move_buffer_hook)
{
    msgpack::unpacker u1(MSGPACK_NULLPTR, MSGPACK_NULLPTR, 64);
    msgpack::unpacker u2(std::move(u1));
    const char m[] = "\x92\xa3" "abc"; // array(2) + referencing str, 2nd elem missing
    u2.reserve_buffer(5);
    std::memcpy(u2.buffer(), m, 5);
    u2.buffer_consumed(5);
    msgpack::object_handle oh;
    u2.next(oh);
    u2.reserve_buffer(1 << 20); // exercises the referenced-buffer hook
    BOOST_CHECK(true);

    // move-assignment and self-move-assignment
    msgpack::unpacker u3(MSGPACK_NULLPTR, MSGPACK_NULLPTR, 64);
    u3 = std::move(u2);
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-move"
#endif
    u3 = std::move(u3); // self-move must not crash
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    BOOST_CHECK(true);
}

// B2: zone move-assignment must run finalizers before freeing chunks.
BOOST_AUTO_TEST_CASE(zone_move_assignment)
{
    msgpack::zone z1;
    z1.allocate<std::string>("a fairly long string that lives in the chunk");
    msgpack::zone z2;
    z1 = std::move(z2); // must not use-after-free
    BOOST_CHECK(true);
}

// B4: vrefbuffer::migrate must grow the destination iovec array when needed.
BOOST_AUTO_TEST_CASE(vrefbuffer_migrate)
{
    // Use distinct 256-byte buffers so each write becomes its own iovec and
    // fills the initial iovec array of both buffers, forcing migrate() to grow
    // the destination array.
    std::vector<std::string> bufs;
    for (std::size_t i = 0; i < 8; ++i) bufs.push_back(std::string(256, static_cast<char>('a' + i)));
    msgpack::vrefbuffer from;
    msgpack::vrefbuffer to;
    for (std::size_t i = 0; i < 4; ++i) from.write(bufs[i].data(), bufs[i].size());
    for (std::size_t i = 4; i < 8; ++i) to.write(bufs[i].data(), bufs[i].size());
    const size_t from_n = from.vector_size();
    const size_t to_n = to.vector_size();
    from.migrate(&to); // must not overflow to's iovec array
    BOOST_CHECK_EQUAL(to.vector_size(), from_n + to_n);
}

// B5: an impossibly large zone allocation must throw, not wrap the malloc size.
BOOST_AUTO_TEST_CASE(zone_allocate_overflow)
{
    msgpack::zone z;
    BOOST_CHECK_THROW(
        z.allocate_no_align(std::numeric_limits<std::size_t>::max() - 4),
        std::bad_alloc);
}

// C1: ext_ref comparison must include the whole payload (used to drop the last
// byte via memcmp(..., m_size)).
BOOST_AUTO_TEST_CASE(ext_ref_full_payload_compare)
{
    char ba[] = {7, 'x', 'y', 'A'};
    char bb[] = {7, 'x', 'y', 'B'};
    msgpack::type::ext_ref ra(ba, sizeof(ba));
    msgpack::type::ext_ref rb(bb, sizeof(bb));
    BOOST_CHECK(!(ra == rb));
    BOOST_CHECK(ra != rb);
    BOOST_CHECK((ra < rb) || (rb < ra));
}

// C2: array_ref<T[N]> relational operators must compare element-wise.
BOOST_AUTO_TEST_CASE(array_ref_carray_compare)
{
    int x[3] = {1, 2, 3};
    int y[3] = {1, 2, 4};
    msgpack::type::array_ref<int[3]> rx = msgpack::type::make_array_ref(x);
    msgpack::type::array_ref<int[3]> ry = msgpack::type::make_array_ref(y);
    BOOST_CHECK(rx == rx);
    BOOST_CHECK(rx != ry);
    BOOST_CHECK(rx < ry);
    BOOST_CHECK(ry > rx);
    BOOST_CHECK(rx <= rx);
    BOOST_CHECK(ry >= rx);
}

// C3: v1 unpacker must enforce the str limit even on the reference path.
BOOST_AUTO_TEST_CASE(v1_unpacker_reference_path_limit)
{
    msgpack::sbuffer sb;
    msgpack::pack(sb, std::string("0123456789"));
    // array, map, str=2, ...
    msgpack::v1::unpacker u(MSGPACK_NULLPTR, MSGPACK_NULLPTR, 64,
                            msgpack::unpack_limit(0xffffffff, 0xffffffff, 2));
    u.reserve_buffer(sb.size());
    std::memcpy(u.buffer(), sb.data(), sb.size());
    u.buffer_consumed(sb.size());
    msgpack::object_handle oh;
    BOOST_CHECK_THROW(u.next(oh), msgpack::str_size_overflow);
}
