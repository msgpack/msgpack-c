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

const unsigned int kLoop = 1000;


#define GEN_TEST_STREAM(test_type)                                      \
    for (unsigned int k = 0; k < kLoop; k++) {                          \
        msgpack::sbuffer sbuf;                                          \
        msgpack::packer<msgpack::sbuffer> pk(sbuf);                     \
        typedef std::vector<test_type> vec_type;                        \
        vec_type vec;                                                   \
        for(unsigned int i = 0; i < static_cast<unsigned int>(rand()) % kLoop; ++i) { \
            vec_type::value_type r = static_cast<test_type>(rand());    \
            vec.push_back(r);                                           \
            pk.pack(r);                                                 \
        }                                                               \
        msgpack::unpacker pac;                                          \
        vec_type::const_iterator it = vec.begin();                      \
        const char *p = sbuf.data();                                    \
        const char * const pend = p + sbuf.size();                      \
        while (p < pend) {                                              \
            const size_t sz = std::min<size_t>(static_cast<std::size_t>(pend - p), static_cast<std::size_t>(rand() % 128)); \
            pac.reserve_buffer(sz);                                     \
            memcpy(pac.buffer(), p, sz);                                \
            pac.buffer_consumed(sz);                                    \
            msgpack::object_handle oh;                                  \
            while (pac.next(oh)) {                                      \
                if (it == vec.end()) goto out;                          \
                msgpack::object obj = oh.get();                         \
                vec_type::value_type val;                               \
                obj.convert(val);                                       \
                BOOST_CHECK_EQUAL(*it, val);                            \
                ++it;                                                   \
            }                                                           \
            p += sz;                                                    \
        }                                                               \
    out:                                                                \
        ;                                                               \
    }

BOOST_AUTO_TEST_CASE(stream_char)
{
    GEN_TEST_STREAM(char);
}

BOOST_AUTO_TEST_CASE(stream_signed_char)
{
    GEN_TEST_STREAM(signed char);
}

BOOST_AUTO_TEST_CASE(stream_unsigned_char)
{
    GEN_TEST_STREAM(unsigned char);
}

BOOST_AUTO_TEST_CASE(stream_short)
{
    GEN_TEST_STREAM(short);
}

BOOST_AUTO_TEST_CASE(stream_int)
{
    GEN_TEST_STREAM(int);
}

BOOST_AUTO_TEST_CASE(stream_long)
{
    GEN_TEST_STREAM(long);
}

BOOST_AUTO_TEST_CASE(stream_long_long)
{
    GEN_TEST_STREAM(long long);
}

BOOST_AUTO_TEST_CASE(stream_unsigned_short)
{
    GEN_TEST_STREAM(unsigned short);
}

BOOST_AUTO_TEST_CASE(stream_unsigned_int)
{
    GEN_TEST_STREAM(unsigned int);
}

BOOST_AUTO_TEST_CASE(stream_unsigned_long)
{
    GEN_TEST_STREAM(unsigned long);
}

BOOST_AUTO_TEST_CASE(stream_unsigned_long_long)
{
    GEN_TEST_STREAM(unsigned long long);
}

BOOST_AUTO_TEST_CASE(stream_uint8)
{
    GEN_TEST_STREAM(uint8_t);
}

BOOST_AUTO_TEST_CASE(stream_uint16)
{
    GEN_TEST_STREAM(uint16_t);
}

BOOST_AUTO_TEST_CASE(stream_uint32)
{
    GEN_TEST_STREAM(uint32_t);
}

BOOST_AUTO_TEST_CASE(stream_uint64)
{
    GEN_TEST_STREAM(uint64_t);
}

BOOST_AUTO_TEST_CASE(stream_int8)
{
    GEN_TEST_STREAM(int8_t);
}

BOOST_AUTO_TEST_CASE(stream_int16)
{
    GEN_TEST_STREAM(int16_t);
}

BOOST_AUTO_TEST_CASE(stream_int32)
{
    GEN_TEST_STREAM(int32_t);
}

BOOST_AUTO_TEST_CASE(stream_int64)
{
    GEN_TEST_STREAM(int64_t);
}
