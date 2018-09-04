#include "msgpack.h"

#include <math.h>
#include <vector>
#include <limits>

#include <gtest/gtest.h>

#if defined(_MSC_VER) || defined(__MINGW32__)
#define msgpack_rand() ((double)rand() / RAND_MAX)
#else  // _MSC_VER || __MINGW32__
#define msgpack_rand() drand48()
#endif // _MSC_VER || __MINGW32__

using namespace std;

const unsigned int kLoop = 10000;
const double kEPS = 1e-10;

#define GEN_TEST_SIGNED(test_type, func_type)                           \
    do {                                                                \
        vector<test_type> v;                                            \
        v.push_back(0);                                                 \
        v.push_back(1);                                                 \
        v.push_back(-1);                                                \
        v.push_back(numeric_limits<test_type>::min());                  \
        v.push_back(numeric_limits<test_type>::max());                  \
        for (unsigned int i = 0; i < kLoop; i++)                        \
            v.push_back(rand());                                        \
        for (unsigned int i = 0; i < v.size() ; i++) {                  \
            test_type val = v[i];                                       \
            msgpack_sbuffer sbuf;                                       \
            msgpack_sbuffer_init(&sbuf);                                \
            msgpack_packer pk;                                          \
            msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);     \
            msgpack_pack_##func_type(&pk, val);                         \
            msgpack_zone z;                                             \
            msgpack_zone_init(&z, 2048);                                \
            msgpack_object obj;                                         \
            msgpack_unpack_return ret =                                 \
                msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);   \
            EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);                     \
            if (val < 0) {                                              \
                EXPECT_EQ(MSGPACK_OBJECT_NEGATIVE_INTEGER, obj.type);   \
                EXPECT_EQ(val, obj.via.i64);                            \
            } else {                                                    \
                EXPECT_EQ(MSGPACK_OBJECT_POSITIVE_INTEGER, obj.type);   \
                EXPECT_EQ(static_cast<uint64_t>(val), obj.via.u64);     \
            }                                                           \
            msgpack_zone_destroy(&z);                                   \
            msgpack_sbuffer_destroy(&sbuf);                             \
        }                                                               \
    } while(0)

#define GEN_TEST_UNSIGNED(test_type, func_type)                         \
    do {                                                                \
        vector<test_type> v;                                            \
        v.push_back(0);                                                 \
        v.push_back(1);                                                 \
        v.push_back(2);                                                 \
        v.push_back(numeric_limits<test_type>::min());                  \
        v.push_back(numeric_limits<test_type>::max());                  \
        for (unsigned int i = 0; i < kLoop; i++)                        \
            v.push_back(rand());                                        \
        for (unsigned int i = 0; i < v.size() ; i++) {                  \
            test_type val = v[i];                                       \
            msgpack_sbuffer sbuf;                                       \
            msgpack_sbuffer_init(&sbuf);                                \
            msgpack_packer pk;                                          \
            msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);     \
            msgpack_pack_##func_type(&pk, val);                         \
            msgpack_zone z;                                             \
            msgpack_zone_init(&z, 2048);                                \
            msgpack_object obj;                                         \
            msgpack_unpack_return ret =                                 \
                msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);   \
            EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);                     \
            EXPECT_EQ(MSGPACK_OBJECT_POSITIVE_INTEGER, obj.type);       \
            EXPECT_EQ(val, obj.via.u64);                                \
            msgpack_zone_destroy(&z);                                   \
            msgpack_sbuffer_destroy(&sbuf);                             \
        }                                                               \
    } while(0)

TEST(MSGPACKC, simple_buffer_char)
{
#if defined(CHAR_MIN)
#if CHAR_MIN < 0
    GEN_TEST_SIGNED(char, char);
#else
    GEN_TEST_UNSIGNED(char, char);
#endif
#else
#error CHAR_MIN is not defined
#endif
}

TEST(MSGPACKC, simple_buffer_singed_char)
{
    GEN_TEST_SIGNED(signed char, signed_char);
}

TEST(MSGPACKC, simple_buffer_short)
{
    GEN_TEST_SIGNED(short, short);
}

TEST(MSGPACKC, simple_buffer_int)
{
    GEN_TEST_SIGNED(int, int);
}

TEST(MSGPACKC, simple_buffer_long)
{
    GEN_TEST_SIGNED(long, long);
}

TEST(MSGPACKC, simple_buffer_long_long)
{
    GEN_TEST_SIGNED(long long, long_long);
}

TEST(MSGPACKC, simple_buffer_unsigned_char)
{
    GEN_TEST_UNSIGNED(unsigned char, unsigned_char);
}

TEST(MSGPACKC, simple_buffer_unsigned_short)
{
    GEN_TEST_UNSIGNED(unsigned short, unsigned_short);
}

TEST(MSGPACKC, simple_buffer_unsigned_int)
{
    GEN_TEST_UNSIGNED(unsigned int, unsigned_int);
}

TEST(MSGPACKC, simple_buffer_unsigned_long)
{
    GEN_TEST_UNSIGNED(unsigned long, unsigned_long);
}

TEST(MSGPACKC, simple_buffer_unsigned_long_long)
{
    GEN_TEST_UNSIGNED(unsigned long long, unsigned_long_long);
}

TEST(MSGPACKC, simple_buffer_uint8)
{
    GEN_TEST_UNSIGNED(uint8_t, uint8);
}

TEST(MSGPACKC, simple_buffer_uint16)
{
    GEN_TEST_UNSIGNED(uint16_t, uint16);
}

TEST(MSGPACKC, simple_buffer_uint32)
{
    GEN_TEST_UNSIGNED(uint32_t, uint32);
}

TEST(MSGPACKC, simple_buffer_uint64)
{
    GEN_TEST_UNSIGNED(uint64_t, uint64);
}

TEST(MSGPACKC, simple_buffer_int8)
{
    GEN_TEST_SIGNED(int8_t, int8);
}

TEST(MSGPACKC, simple_buffer_int16)
{
    GEN_TEST_SIGNED(int16_t, int16);
}

TEST(MSGPACKC, simple_buffer_int32)
{
    GEN_TEST_SIGNED(int32_t, int32);
}

TEST(MSGPACKC, simple_buffer_int64)
{
    GEN_TEST_SIGNED(int64_t, int64);
}

#if !defined(_MSC_VER) || _MSC_VER >=1800

TEST(MSGPACKC, simple_buffer_float)
{
    vector<float> v;
    v.push_back(0.0);
    v.push_back(1.0);
    v.push_back(-1.0);
    v.push_back(numeric_limits<float>::min());
    v.push_back(numeric_limits<float>::max());
    v.push_back(nanf("tag"));
    if (numeric_limits<float>::has_infinity) {
        v.push_back(numeric_limits<float>::infinity());
        v.push_back(-numeric_limits<float>::infinity());
    }
    if (numeric_limits<float>::has_quiet_NaN) {
        v.push_back(numeric_limits<float>::quiet_NaN());
    }
    if (numeric_limits<float>::has_signaling_NaN) {
        v.push_back(numeric_limits<float>::signaling_NaN());
    }

    for (unsigned int i = 0; i < kLoop; i++) {
        v.push_back(static_cast<float>(msgpack_rand()));
        v.push_back(static_cast<float>(-msgpack_rand()));
    }

    for (unsigned int i = 0; i < v.size() ; i++) {
        float val = v[i];
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
        msgpack_pack_float(&pk, val);
        msgpack_zone z;
        msgpack_zone_init(&z, 2048);
        msgpack_object obj;
        msgpack_unpack_return ret =
            msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
        EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
        EXPECT_EQ(MSGPACK_OBJECT_FLOAT32, obj.type);
        if (isnan(val)) {
            EXPECT_TRUE(isnan(obj.via.f64));
#if defined(MSGPACK_USE_LEGACY_NAME_AS_FLOAT)
            EXPECT_TRUE(isnan(obj.via.dec));
#endif // MSGPACK_USE_LEGACY_NAME_AS_FLOAT
        }
        else if (isinf(val)) {
            EXPECT_TRUE(isinf(obj.via.f64));
#if defined(MSGPACK_USE_LEGACY_NAME_AS_FLOAT)
            EXPECT_TRUE(isinf(obj.via.dec));
#endif // MSGPACK_USE_LEGACY_NAME_AS_FLOAT
        }
        else {
            EXPECT_TRUE(fabs(obj.via.f64 - val) <= kEPS);
#if defined(MSGPACK_USE_LEGACY_NAME_AS_FLOAT)
            EXPECT_TRUE(fabs(obj.via.dec - val) <= kEPS);
#endif // MSGPACK_USE_LEGACY_NAME_AS_FLOAT
        }
        msgpack_zone_destroy(&z);
        msgpack_sbuffer_destroy(&sbuf);
    }
}

TEST(MSGPACKC, simple_buffer_double)
{
    vector<double> v;
    v.push_back(0.0);
    v.push_back(-0.0);
    v.push_back(1.0);
    v.push_back(-1.0);
    v.push_back(numeric_limits<double>::min());
    v.push_back(numeric_limits<double>::max());
    v.push_back(nan("tag"));
    if (numeric_limits<double>::has_infinity) {
        v.push_back(numeric_limits<double>::infinity());
        v.push_back(-numeric_limits<double>::infinity());
    }
    if (numeric_limits<double>::has_quiet_NaN) {
        v.push_back(numeric_limits<double>::quiet_NaN());
    }
    if (numeric_limits<double>::has_signaling_NaN) {
        v.push_back(numeric_limits<double>::signaling_NaN());
    }
    for (unsigned int i = 0; i < kLoop; i++) {
        v.push_back(msgpack_rand());
        v.push_back(-msgpack_rand());
    }

    for (unsigned int i = 0; i < v.size() ; i++) {
        double val = v[i];
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
        msgpack_pack_double(&pk, val);
        msgpack_zone z;
        msgpack_zone_init(&z, 2048);
        msgpack_object obj;
        msgpack_unpack_return ret =
            msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
        EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
        EXPECT_EQ(MSGPACK_OBJECT_FLOAT64, obj.type);
        EXPECT_EQ(MSGPACK_OBJECT_FLOAT, obj.type);
#if defined(MSGPACK_USE_LEGACY_NAME_AS_FLOAT)
        EXPECT_EQ(MSGPACK_OBJECT_DOUBLE, obj.type);
#endif // MSGPACK_USE_LEGACY_NAME_AS_FLOAT
        if (isnan(val)) {
            EXPECT_TRUE(isnan(obj.via.f64));
#if defined(MSGPACK_USE_LEGACY_NAME_AS_FLOAT)
            EXPECT_TRUE(isnan(obj.via.dec));
#endif // MSGPACK_USE_LEGACY_NAME_AS_FLOAT
        }
        else if (isinf(val)) {
            EXPECT_TRUE(isinf(obj.via.f64));
#if defined(MSGPACK_USE_LEGACY_NAME_AS_FLOAT)
            EXPECT_TRUE(isinf(obj.via.dec));
#endif // MSGPACK_USE_LEGACY_NAME_AS_FLOAT
        }
        else {
            EXPECT_TRUE(fabs(obj.via.f64 - val) <= kEPS);
#if defined(MSGPACK_USE_LEGACY_NAME_AS_FLOAT)
            EXPECT_TRUE(fabs(obj.via.dec - val) <= kEPS);
#endif // MSGPACK_USE_LEGACY_NAME_AS_FLOAT
        }
        msgpack_zone_destroy(&z);
        msgpack_sbuffer_destroy(&sbuf);
    }
}

#endif // !defined(_MSC_VER) || _MSC_VER >=1800

TEST(MSGPACKC, simple_buffer_nil)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_nil(&pk);
    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret =
        msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_NIL, obj.type);
    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_true)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_true(&pk);
    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret =
        msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_BOOLEAN, obj.type);
    EXPECT_EQ(true, obj.via.boolean);
    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_false)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_false(&pk);
    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret =
        msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_BOOLEAN, obj.type);
    EXPECT_FALSE(obj.via.boolean);
    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_fixext1)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    char const buf[] = { 2 };

    msgpack_pack_ext(&pk, sizeof(buf), 1);
    msgpack_pack_ext_body(&pk, buf, sizeof(buf));
    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret =
        msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_EXT, obj.type);
    EXPECT_EQ(1u, obj.via.ext.size);
    EXPECT_EQ(1, obj.via.ext.type);
    EXPECT_EQ(2, obj.via.ext.ptr[0]);
    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_fixext2)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    char const buf[] = { 2, 3 };

    msgpack_pack_ext(&pk, sizeof(buf), 0);
    msgpack_pack_ext_body(&pk, buf, sizeof(buf));
    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret =
        msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_EXT, obj.type);
    EXPECT_EQ(2u, obj.via.ext.size);
    EXPECT_EQ(0, obj.via.ext.type);
    EXPECT_EQ(0, memcmp(buf, obj.via.ext.ptr, sizeof(buf)));
    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_fixext4)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    char const buf[] = { 2, 3, 4, 5 };

    msgpack_pack_ext(&pk, sizeof(buf), 1);
    msgpack_pack_ext_body(&pk, buf, sizeof(buf));
    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret =
        msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_EXT, obj.type);
    EXPECT_EQ(4u, obj.via.ext.size);
    EXPECT_EQ(1, obj.via.ext.type);
    EXPECT_EQ(0, memcmp(buf, obj.via.ext.ptr, sizeof(buf)));
    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_fixext8)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    char const buf [] = { 2, 3, 4, 5, 6, 7, 8, 9 };

    msgpack_pack_ext(&pk, sizeof(buf), 1);
    msgpack_pack_ext_body(&pk, buf, sizeof(buf));
    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret =
        msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_EXT, obj.type);
    EXPECT_EQ(8u, obj.via.ext.size);
    EXPECT_EQ(1, obj.via.ext.type);
    EXPECT_EQ(0, memcmp(buf, obj.via.ext.ptr, sizeof(buf)));
    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_fixext16)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    char const buf [] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };

    msgpack_pack_ext(&pk, sizeof(buf), 1);
    msgpack_pack_ext_body(&pk, buf, sizeof(buf));
    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret =
        msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_EXT, obj.type);
    EXPECT_EQ(16u, obj.via.ext.size);
    EXPECT_EQ(1, obj.via.ext.type);
    EXPECT_EQ(0, memcmp(buf, obj.via.ext.ptr, sizeof(buf)));
    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_fixext_1byte_0)
{
    const size_t size = 0;
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    msgpack_pack_ext(&pk, size, 77);
    // fprintf(stderr, "size: %u, data: \"", sbuf.size);
    // for (size_t i = 0; i < sbuf.size; i++)
    //   fprintf(stderr, "%02x ", (uint8_t)sbuf.data[i]);
    // fprintf(stderr, "\"\n");
    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret =
        msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_EXT, obj.type);
    EXPECT_EQ(size, obj.via.ext.size);
    EXPECT_EQ(77, obj.via.ext.type);
    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_fixext_1byte_255)
{
    const size_t size = 255;
    char buf[size];
    for (size_t i = 0; i != size; ++i) buf[i] = (char)i;

    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    msgpack_pack_ext(&pk, size, 78);
    msgpack_pack_ext_body(&pk, buf, sizeof(buf));
    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret =
        msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_EXT, obj.type);
    EXPECT_EQ(size, obj.via.ext.size);
    EXPECT_EQ(78, obj.via.ext.type);
    EXPECT_EQ(0, memcmp(buf, obj.via.ext.ptr, sizeof(buf)));
    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_fixext_2byte_256)
{
    const size_t size = 256;
    char buf[size];
    for (size_t i = 0; i != size; ++i) buf[i] = (char)i;

    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    msgpack_pack_ext(&pk, size, 79);
    msgpack_pack_ext_body(&pk, buf, sizeof(buf));
    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret =
        msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_EXT, obj.type);
    EXPECT_EQ(size, obj.via.ext.size);
    EXPECT_EQ(79, obj.via.ext.type);
    EXPECT_EQ(0, memcmp(buf, obj.via.ext.ptr, sizeof(buf)));
    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_fixext_2byte_65535)
{
    const size_t size = 65535;
    char buf[size];
    for (size_t i = 0; i != size; ++i) buf[i] = (char)i;

    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    msgpack_pack_ext(&pk, size, 80);
    msgpack_pack_ext_body(&pk, buf, sizeof(buf));
    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret =
        msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_EXT, obj.type);
    EXPECT_EQ(size, obj.via.ext.size);
    EXPECT_EQ(80, obj.via.ext.type);
    EXPECT_EQ(0, memcmp(buf, obj.via.ext.ptr, sizeof(buf)));
    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_fixext_4byte_65536)
{
    const size_t size = 65536;
    char buf[size];
    for (size_t i = 0; i != size; ++i) buf[i] = (char)i;

    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    msgpack_pack_ext(&pk, size, 81);
    msgpack_pack_ext_body(&pk, buf, sizeof(buf));
    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret =
        msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_EXT, obj.type);
    EXPECT_EQ(size, obj.via.ext.size);
    EXPECT_EQ(81, obj.via.ext.type);
    EXPECT_EQ(0, memcmp(buf, obj.via.ext.ptr, sizeof(buf)));
    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_timestamp_32)
{
    msgpack_timestamp ts = {
        0xffffffff,
        0
    };

    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    msgpack_pack_timestamp(&pk, &ts);
    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret =
        msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_EXT, obj.type);
    EXPECT_EQ(4u, obj.via.ext.size);
    EXPECT_EQ(-1, obj.via.ext.type);
    msgpack_timestamp ts2;
    bool r = msgpack_object_to_timestamp(&obj, &ts2);

    EXPECT_TRUE(r);
    EXPECT_EQ(ts.tv_sec, ts2.tv_sec);
    EXPECT_EQ(ts.tv_nsec, ts2.tv_nsec);

    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_timestamp_64)
{
    msgpack_timestamp ts = {
        0x3ffffffffL,
        999999999
    };

    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    msgpack_pack_timestamp(&pk, &ts);
    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret =
        msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_EXT, obj.type);
    EXPECT_EQ(8u, obj.via.ext.size);
    EXPECT_EQ(-1, obj.via.ext.type);
    msgpack_timestamp ts2;
    bool r = msgpack_object_to_timestamp(&obj, &ts2);

    EXPECT_TRUE(r);
    EXPECT_EQ(ts.tv_sec, ts2.tv_sec);
    EXPECT_EQ(ts.tv_nsec, ts2.tv_nsec);

    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_timestamp_96)
{
    msgpack_timestamp ts = {
        0x7fffffffffffffffLL,
        999999999
    };

    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    msgpack_pack_timestamp(&pk, &ts);
    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret =
        msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_EXT, obj.type);
    EXPECT_EQ(12u, obj.via.ext.size);
    EXPECT_EQ(-1, obj.via.ext.type);
    msgpack_timestamp ts2;
    bool r = msgpack_object_to_timestamp(&obj, &ts2);

    EXPECT_TRUE(r);
    EXPECT_EQ(ts.tv_sec, ts2.tv_sec);
    EXPECT_EQ(ts.tv_nsec, ts2.tv_nsec);

    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_array)
{
    unsigned int array_size = 5;

    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_array(&pk, array_size);
    msgpack_pack_nil(&pk);
    msgpack_pack_true(&pk);
    msgpack_pack_false(&pk);
    msgpack_pack_int(&pk, 10);
    msgpack_pack_int(&pk, -10);

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_ARRAY, obj.type);
    EXPECT_EQ(array_size, obj.via.array.size);

    for (unsigned int i = 0; i < obj.via.array.size; i++) {
        msgpack_object o = obj.via.array.ptr[i];
        switch (i) {
        case 0:
            EXPECT_EQ(MSGPACK_OBJECT_NIL, o.type);
            break;
        case 1:
            EXPECT_EQ(MSGPACK_OBJECT_BOOLEAN, o.type);
            EXPECT_EQ(true, o.via.boolean);
            break;
        case 2:
            EXPECT_EQ(MSGPACK_OBJECT_BOOLEAN, o.type);
            EXPECT_FALSE(o.via.boolean);
            break;
        case 3:
            EXPECT_EQ(MSGPACK_OBJECT_POSITIVE_INTEGER, o.type);
            EXPECT_EQ(10u, o.via.u64);
            break;
        case 4:
            EXPECT_EQ(MSGPACK_OBJECT_NEGATIVE_INTEGER, o.type);
            EXPECT_EQ(-10, o.via.i64);
            break;
        }
    }

    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_map)
{
    unsigned int map_size = 2;

    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_map(&pk, map_size);
    msgpack_pack_true(&pk);
    msgpack_pack_false(&pk);
    msgpack_pack_int(&pk, 10);
    msgpack_pack_int(&pk, -10);

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_MAP, obj.type);
    EXPECT_EQ(map_size, obj.via.map.size);

    for (unsigned int i = 0; i < map_size; i++) {
        msgpack_object key = obj.via.map.ptr[i].key;
        msgpack_object val = obj.via.map.ptr[i].val;
        switch (i) {
        case 0:
            EXPECT_EQ(MSGPACK_OBJECT_BOOLEAN, key.type);
            EXPECT_EQ(true, key.via.boolean);
            EXPECT_EQ(MSGPACK_OBJECT_BOOLEAN, val.type);
            EXPECT_FALSE(val.via.boolean);
            break;
        case 1:
            EXPECT_EQ(MSGPACK_OBJECT_POSITIVE_INTEGER, key.type);
            EXPECT_EQ(10u, key.via.u64);
            EXPECT_EQ(MSGPACK_OBJECT_NEGATIVE_INTEGER, val.type);
            EXPECT_EQ(-10, val.via.i64);
            break;
        }
    }

    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_str)
{
    unsigned int str_size = 7;

    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_str(&pk, str_size);
    msgpack_pack_str_body(&pk, "fr", 2);
    msgpack_pack_str_body(&pk, "syuki", 5);
    // invalid data
    msgpack_pack_str_body(&pk, "", 0);
    msgpack_pack_str_body(&pk, "kzk", 0);

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_STR, obj.type);
    EXPECT_EQ(str_size, obj.via.str.size);
    EXPECT_EQ(0, memcmp("frsyuki", obj.via.str.ptr, str_size));

    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_str_fix_l)
{
    char const* str = NULL;
    unsigned int str_size = 0;
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_str(&pk, str_size);
    msgpack_pack_str_body(&pk, str, str_size);
    EXPECT_EQ(sbuf.size, 0x01u);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xa0u));

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_STR, obj.type);
    EXPECT_EQ(str_size, obj.via.str.size);

    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_str_fix_h)
{
    char str[0x1f] = {'0'};
    unsigned int str_size = sizeof(str);
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_str(&pk, str_size);
    msgpack_pack_str_body(&pk, str, str_size);
    EXPECT_EQ(sbuf.size, 0x1f+1u);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xbfu));

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_STR, obj.type);
    EXPECT_EQ(str_size, obj.via.str.size);
    EXPECT_EQ(0, memcmp(str, obj.via.str.ptr, str_size));

    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_str_8_l)
{
    char str[0x1f+1] = {'0'};
    unsigned int str_size = sizeof(str);
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_str(&pk, str_size);
    msgpack_pack_str_body(&pk, str, str_size);
    EXPECT_EQ(sbuf.size, 0x1f+1+2u);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd9u));
    EXPECT_EQ(sbuf.data[1], static_cast<char>(0x20u));

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_STR, obj.type);
    EXPECT_EQ(str_size, obj.via.str.size);
    EXPECT_EQ(0, memcmp(str, obj.via.str.ptr, str_size));

    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_str_8_h)
{
    char str[0xff] = {'0'};
    unsigned int str_size = sizeof(str);
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_str(&pk, str_size);
    msgpack_pack_str_body(&pk, str, str_size);
    EXPECT_EQ(sbuf.size, 0xff+2u);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd9u));
    EXPECT_EQ(sbuf.data[1], static_cast<char>(0xffu));

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_STR, obj.type);
    EXPECT_EQ(str_size, obj.via.str.size);
    EXPECT_EQ(0, memcmp(str, obj.via.str.ptr, str_size));

    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_str_16_l)
{
    char str[0xff+1] = {'0'};
    unsigned int str_size = sizeof(str);
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_str(&pk, str_size);
    msgpack_pack_str_body(&pk, str, str_size);
    EXPECT_EQ(sbuf.size, 0xff+1+3u);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xdau));
    EXPECT_EQ(sbuf.data[1], static_cast<char>(0x01u));
    EXPECT_EQ(sbuf.data[2], static_cast<char>(0x00u));

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_STR, obj.type);
    EXPECT_EQ(str_size, obj.via.str.size);
    EXPECT_EQ(0, memcmp(str, obj.via.str.ptr, str_size));

    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_str_16_h)
{
    char str[0xffff] = {'0'};
    unsigned int str_size = sizeof(str);
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_str(&pk, str_size);
    msgpack_pack_str_body(&pk, str, str_size);
    EXPECT_EQ(sbuf.size, 0xffff+3u);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xdau));
    EXPECT_EQ(sbuf.data[1], static_cast<char>(0xffu));
    EXPECT_EQ(sbuf.data[2], static_cast<char>(0xffu));

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_STR, obj.type);
    EXPECT_EQ(str_size, obj.via.str.size);
    EXPECT_EQ(0, memcmp(str, obj.via.str.ptr, str_size));

    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_str_32_l)
{
    char str[0xffff+1] = {'0'};
    unsigned int str_size = sizeof(str);
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_str(&pk, str_size);
    msgpack_pack_str_body(&pk, str, str_size);
    EXPECT_EQ(sbuf.size, 0xffff+1+5u);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xdbu));
    EXPECT_EQ(sbuf.data[1], static_cast<char>(0x00u));
    EXPECT_EQ(sbuf.data[2], static_cast<char>(0x01u));
    EXPECT_EQ(sbuf.data[3], static_cast<char>(0x00u));
    EXPECT_EQ(sbuf.data[4], static_cast<char>(0x00u));

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_STR, obj.type);
    EXPECT_EQ(str_size, obj.via.str.size);
    EXPECT_EQ(0, memcmp(str, obj.via.str.ptr, str_size));

    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_v4raw_fix_l)
{
    char const* str = NULL;
    unsigned int str_size = 0;
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_v4raw(&pk, str_size);
    msgpack_pack_v4raw_body(&pk, str, str_size);
    EXPECT_EQ(sbuf.size, 0x01u);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xa0u));

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_STR, obj.type);
    EXPECT_EQ(str_size, obj.via.str.size);

    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_v4raw_fix_h)
{
    char str[0x1f] = {'0'};
    unsigned int str_size = sizeof(str);
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_v4raw(&pk, str_size);
    msgpack_pack_v4raw_body(&pk, str, str_size);
    EXPECT_EQ(sbuf.size, 0x1f+1u);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xbfu));

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_STR, obj.type);
    EXPECT_EQ(str_size, obj.via.str.size);
    EXPECT_EQ(0, memcmp(str, obj.via.str.ptr, str_size));

    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_v4raw_16_l)
{
    char str[0x1f+1] = {'0'};
    unsigned int str_size = sizeof(str);
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_v4raw(&pk, str_size);
    msgpack_pack_v4raw_body(&pk, str, str_size);
    EXPECT_EQ(sbuf.size, 0x1f+1+3u);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xdau));
    EXPECT_EQ(sbuf.data[1], static_cast<char>(0x00u));
    EXPECT_EQ(sbuf.data[2], static_cast<char>(0x20u));

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_STR, obj.type);
    EXPECT_EQ(str_size, obj.via.str.size);
    EXPECT_EQ(0, memcmp(str, obj.via.str.ptr, str_size));

    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_v4raw_16_h)
{
    char str[0xffff] = {'0'};
    unsigned int str_size = sizeof(str);
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_v4raw(&pk, str_size);
    msgpack_pack_v4raw_body(&pk, str, str_size);
    EXPECT_EQ(sbuf.size, 0xffff+3u);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xdau));
    EXPECT_EQ(sbuf.data[1], static_cast<char>(0xffu));
    EXPECT_EQ(sbuf.data[2], static_cast<char>(0xffu));

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_STR, obj.type);
    EXPECT_EQ(str_size, obj.via.str.size);
    EXPECT_EQ(0, memcmp(str, obj.via.str.ptr, str_size));

    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}

TEST(MSGPACKC, simple_buffer_v4raw_32_l)
{
    char str[0xffff+1] = {'0'};
    unsigned int str_size = sizeof(str);
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_v4raw(&pk, str_size);
    msgpack_pack_v4raw_body(&pk, str, str_size);
    EXPECT_EQ(sbuf.size, 0xffff+1+5u);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xdbu));
    EXPECT_EQ(sbuf.data[1], static_cast<char>(0x00u));
    EXPECT_EQ(sbuf.data[2], static_cast<char>(0x01u));
    EXPECT_EQ(sbuf.data[3], static_cast<char>(0x00u));
    EXPECT_EQ(sbuf.data[4], static_cast<char>(0x00u));

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(sbuf.data, sbuf.size, NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_STR, obj.type);
    EXPECT_EQ(str_size, obj.via.str.size);
    EXPECT_EQ(0, memcmp(str, obj.via.str.ptr, str_size));

    msgpack_zone_destroy(&z);
    msgpack_sbuffer_destroy(&sbuf);
}


TEST(MSGPACKC, unpack_fixstr)
{
    size_t str_size = 7;
    const char buf[] = {
        (char)0xa7, 'f', 'r', 's', 'y', 'u', 'k', 'i'
    };

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(buf, sizeof(buf), NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_STR, obj.type);
    EXPECT_EQ(str_size, obj.via.str.size);
    EXPECT_EQ(0, memcmp("frsyuki", obj.via.str.ptr, str_size));

    msgpack_zone_destroy(&z);
}

TEST(MSGPACKC, unpack_str8)
{
    size_t str_size = 7;
    const char buf[] = {
        (char)0xd9, 7, 'f', 'r', 's', 'y', 'u', 'k', 'i'
    };

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(buf, sizeof(buf), NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_STR, obj.type);
    EXPECT_EQ(str_size, obj.via.str.size);
    EXPECT_EQ(0, memcmp("frsyuki", obj.via.str.ptr, str_size));

    msgpack_zone_destroy(&z);
}

TEST(MSGPACKC, unpack_str16)
{
    size_t str_size = 7;
    const char buf[] = {
        (char)0xda, 0, 7, 'f', 'r', 's', 'y', 'u', 'k', 'i'
    };

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(buf, sizeof(buf), NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_STR, obj.type);
    EXPECT_EQ(str_size, obj.via.str.size);
    EXPECT_EQ(0, memcmp("frsyuki", obj.via.str.ptr, str_size));

    msgpack_zone_destroy(&z);
}

TEST(MSGPACKC, unpack_str32)
{
    size_t str_size = 7;
    const char buf[] = {
        (char)0xdb, 0, 0, 0, 7, 'f', 'r', 's', 'y', 'u', 'k', 'i'
    };

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(buf, sizeof(buf), NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_STR, obj.type);
    EXPECT_EQ(str_size, obj.via.str.size);
    EXPECT_EQ(0, memcmp("frsyuki", obj.via.str.ptr, str_size));

    msgpack_zone_destroy(&z);
}

TEST(MSGPACKC, unpack_bin8)
{
    size_t bin_size = 7;
    const char buf[] = {
        (char)0xc4, 7, 'f', 'r', 's', 'y', 'u', 'k', 'i'
    };

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(buf, sizeof(buf), NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_BIN, obj.type);
    EXPECT_EQ(bin_size, obj.via.bin.size);
    EXPECT_EQ(0, memcmp("frsyuki", obj.via.bin.ptr, bin_size));

    msgpack_zone_destroy(&z);
}

TEST(MSGPACKC, unpack_bin16)
{
    size_t bin_size = 7;
    const char buf[] = {
        (char)0xc5, 0, 7, 'f', 'r', 's', 'y', 'u', 'k', 'i'
    };

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(buf, sizeof(buf), NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_BIN, obj.type);
    EXPECT_EQ(bin_size, obj.via.bin.size);
    EXPECT_EQ(0, memcmp("frsyuki", obj.via.bin.ptr, bin_size));

    msgpack_zone_destroy(&z);
}

TEST(MSGPACKC, unpack_bin32)
{
    size_t bin_size = 7;
    const char buf[] = {
        (char)0xc6, 0, 0, 0, 7, 'f', 'r', 's', 'y', 'u', 'k', 'i'
    };

    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(buf, sizeof(buf), NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_BIN, obj.type);
    EXPECT_EQ(bin_size, obj.via.bin.size);
    EXPECT_EQ(0, memcmp("frsyuki", obj.via.bin.ptr, bin_size));

    msgpack_zone_destroy(&z);
}

TEST(MSGPACKC, unpack_array_uint64)
{
    const char buf[] = {
        (char)0x91, (char)0xcf, (char)0xff, (char)0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
    };
    msgpack_zone z;
    msgpack_zone_init(&z, 2048);
    msgpack_object obj;
    msgpack_unpack_return ret;
    ret = msgpack_unpack(buf, sizeof(buf), NULL, &z, &obj);
    EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, ret);
    EXPECT_EQ(MSGPACK_OBJECT_ARRAY, obj.type);
    EXPECT_EQ(1u, obj.via.array.size);
    EXPECT_EQ(MSGPACK_OBJECT_POSITIVE_INTEGER, obj.via.array.ptr[0].type);
    EXPECT_EQ(0xFFF0000000000001LL, obj.via.array.ptr[0].via.u64);
    msgpack_zone_destroy(&z);
}


TEST(MSGPACKC, vref_buffer_overflow)
{
    msgpack_vrefbuffer vbuf;
    msgpack_vrefbuffer to;
    size_t ref_size = 0;
    size_t chunk_size = std::numeric_limits<size_t>::max();
    EXPECT_FALSE(msgpack_vrefbuffer_init(&vbuf, ref_size, chunk_size));
    EXPECT_EQ(-1, msgpack_vrefbuffer_migrate(&vbuf, &to));
}
