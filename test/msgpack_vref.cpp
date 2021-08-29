#include "msgpack.hpp"

#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <set>
#include <list>
#include <limits>

#define BOOST_TEST_MODULE MSGPACK
#include <boost/test/unit_test.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

#define GEN_TEST_VREF(test_type, vbuf)                                  \
    do {                                                                \
        vector<test_type> v;                                            \
        v.push_back(0);                                                 \
        for (unsigned int i = 0; i < v.size(); i++) {                   \
            test_type val1 = v[i];                                      \
            msgpack::pack(vbuf, val1);                                  \
            msgpack::sbuffer sbuf;                                      \
            const msgpack::iovec* cur = vbuf.vector();                  \
            const msgpack::iovec* end = cur + vbuf.vector_size();       \
            for(; cur != end; ++cur)                                    \
                sbuf.write((const char*)cur->iov_base, cur->iov_len);   \
            msgpack::object_handle oh;                                  \
            msgpack::unpack(oh, sbuf.data(), sbuf.size());              \
            test_type val2 = oh.get().as<test_type>();                  \
            BOOST_CHECK_EQUAL(val1, val2);                              \
        }                                                               \
    } while(0);

BOOST_AUTO_TEST_CASE(vrefbuffer_char)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(char, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_signed_char)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(signed char, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_unsigned_char)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(unsigned char, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_short)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(short, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_int)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(int, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_long)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(long, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_long_long)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(long long, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_unsigned_short)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(unsigned short, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_unsigned_int)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(unsigned int, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_unsigned_long)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(unsigned long, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_unsigned_long_long)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(unsigned long long, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_uint8)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(uint8_t, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_uint16)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(uint16_t, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_uint32)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(uint32_t, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_uint64)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(uint64_t, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_int8)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(int8_t, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_int16)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(int16_t, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_int32)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(int32_t, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_int64)
{
    msgpack::vrefbuffer vbuf;
    GEN_TEST_VREF(int64_t, vbuf);
}

// small ref_size and chunk_size
BOOST_AUTO_TEST_CASE(vrefbuffer_small_char)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(char, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_small_signed_char)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(signed char, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_small_unsigned_char)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(unsigned char, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_small_short)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(short, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_small_int)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(int, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_small_long)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(long, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_small_long_long)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(long long, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_small_unsigned_short)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(unsigned short, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_small_unsigned_int)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(unsigned int, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_small_unsigned_long)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(unsigned long, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_small_unsigned_long_long)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(unsigned long long, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_small_uint8)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(uint8_t, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_small_uint16)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(uint16_t, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_small_uint32)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(uint32_t, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_small_uint64)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(uint64_t, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_small_int8)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(int8_t, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_small_int16)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(int16_t, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_small_int32)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(int32_t, vbuf);
}

BOOST_AUTO_TEST_CASE(vrefbuffer_small_int64)
{
    msgpack::vrefbuffer vbuf(0, 0);
    GEN_TEST_VREF(int64_t, vbuf);
}

BOOST_AUTO_TEST_CASE(vref_buffer_overflow)
{
    size_t ref_size = 0;
    size_t chunk_size = std::numeric_limits<size_t>::max();
    char *buf = (char *)malloc(0x1000);
    BOOST_CHECK_THROW(msgpack::vrefbuffer vbuf(ref_size, chunk_size), std::bad_alloc);
    msgpack::vrefbuffer vbuf2(0, 0x1000);
    BOOST_CHECK_THROW(vbuf2.append_copy(buf, chunk_size), std::bad_alloc);
    free(buf);
}
