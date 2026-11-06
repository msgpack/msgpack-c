#include <zlib.h>

// A length over 4 GiB can't go through the real deflate() in a unit test: it
// would read all of it. This stand-in only counts the input it is given.
static unsigned long long deflated_bytes = 0;

static int count_deflated_bytes(z_streamp strm, int /* flush */)
{
    deflated_bytes += strm->avail_in;
    strm->avail_in = 0;
    return Z_OK;
}

#define deflate count_deflated_bytes
#include <msgpack/zbuffer.h>
#undef deflate

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif //defined(__GNUC__)

#include <gtest/gtest.h>

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif //defined(__GNUC__)

TEST(zbuffer_sizes, write_length_wider_than_avail_in)
{
    if(sizeof(size_t) <= sizeof(uInt)) return;

    const size_t len = static_cast<size_t>(UINT_MAX) + 2;
    const char byte = 0;
    msgpack_zbuffer zbuf;
    ASSERT_TRUE(msgpack_zbuffer_init(&zbuf, 1, MSGPACK_ZBUFFER_INIT_SIZE));

    deflated_bytes = 0;
    EXPECT_EQ(0, msgpack_zbuffer_write(&zbuf, &byte, len));
    EXPECT_EQ(static_cast<unsigned long long>(len), deflated_bytes);

    msgpack_zbuffer_destroy(&zbuf);
}

TEST(zbuffer_sizes, expand_refuses_to_wrap_the_doubled_size)
{
    msgpack_zbuffer zbuf;
    ASSERT_TRUE(msgpack_zbuffer_init(&zbuf, 1, MSGPACK_ZBUFFER_INIT_SIZE));

    // Claim more than half of the address space is in use: data is NULL here.
    zbuf.stream.next_out =
        reinterpret_cast<Bytef*>(static_cast<size_t>(-1) / 2 + 1);
    EXPECT_FALSE(msgpack_zbuffer_expand(&zbuf));

    msgpack_zbuffer_destroy(&zbuf);
}

TEST(zbuffer_sizes, expand_keeps_output_space_past_4_gib)
{
    if(sizeof(size_t) <= sizeof(uInt)) return;

    msgpack_zbuffer zbuf;
    ASSERT_TRUE(msgpack_zbuffer_init(&zbuf, 1, MSGPACK_ZBUFFER_INIT_SIZE));

    // 4 GiB in use, so 4 GiB are free after doubling: that used to truncate to
    // an avail_out of 0. The 8 GiB allocation is never touched.
    zbuf.stream.next_out =
        reinterpret_cast<Bytef*>(static_cast<size_t>(UINT_MAX) + 1);
    if(msgpack_zbuffer_expand(&zbuf)) {
        EXPECT_EQ(UINT_MAX, zbuf.stream.avail_out);
    }

    msgpack_zbuffer_destroy(&zbuf);
}
