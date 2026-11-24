#include <msgpack.hpp>
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
#include <msgpack/zbuffer.hpp>
#undef deflate

#define BOOST_TEST_MODULE zbuffer_sizes
#include <boost/test/unit_test.hpp>

#include <climits>
#include <cstdint>

// Needs a size_t wider than zlib's uInt. Guarded with the preprocessor rather
// than a runtime check so that the constant does not overflow (and warn) on
// 32-bit targets.
BOOST_AUTO_TEST_CASE(write_length_wider_than_avail_in)
{
#if SIZE_MAX > UINT_MAX
    const size_t len = static_cast<size_t>(UINT_MAX) + 2;
    const char byte = 0;
    msgpack::zbuffer zbuf(1);

    deflated_bytes = 0;
    zbuf.write(&byte, len);
    BOOST_CHECK_EQUAL(static_cast<unsigned long long>(len), deflated_bytes);
#endif // SIZE_MAX > UINT_MAX
}
