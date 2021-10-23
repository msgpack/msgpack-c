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
#include <boost/mpl/list.hpp>

#if defined(_MSC_VER) || defined(__MINGW32__)
#define msgpack_rand() ((double)rand() / RAND_MAX)
#else  // _MSC_VER || __MINGW32__
#define msgpack_rand() drand48()
#endif // _MSC_VER || __MINGW32__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

const unsigned int kLoop = 10000;
const unsigned int kElements = 100;
const double kEPS = 1e-10;

#define GEN_TEST(test_type)                                 \
    do {                                                    \
        vector<test_type> v;                                \
        v.push_back(0);                                     \
        v.push_back(1);                                     \
        v.push_back(2);                                     \
        v.push_back(numeric_limits<test_type>::min());      \
        v.push_back(numeric_limits<test_type>::max());      \
        for (unsigned int i = 0; i < kLoop; i++)            \
            v.push_back(static_cast<test_type>(rand()));    \
        for (unsigned int i = 0; i < v.size() ; i++) {      \
            msgpack::sbuffer sbuf;                          \
            test_type val1 = v[i];                          \
            msgpack::pack(sbuf, val1);                      \
            msgpack::object_handle oh =                     \
                msgpack::unpack(sbuf.data(), sbuf.size());  \
            BOOST_CHECK_EQUAL(val1, oh.get().as<test_type>());      \
        }                                                   \
    } while(0)

BOOST_AUTO_TEST_CASE(simple_buffer_char)
{
    GEN_TEST(char);
}

BOOST_AUTO_TEST_CASE(simple_buffer_signed_char)
{
    GEN_TEST(signed char);
}

BOOST_AUTO_TEST_CASE(simple_buffer_unsigned_char)
{
    GEN_TEST(unsigned char);
}


BOOST_AUTO_TEST_CASE(simple_buffer_short)
{
    GEN_TEST(short);
}

BOOST_AUTO_TEST_CASE(simple_buffer_int)
{
    GEN_TEST(int);
}

BOOST_AUTO_TEST_CASE(simple_buffer_long)
{
    GEN_TEST(long);
}

BOOST_AUTO_TEST_CASE(simple_buffer_long_long)
{
    GEN_TEST(long long);
}

BOOST_AUTO_TEST_CASE(simple_buffer_unsigned_short)
{
    GEN_TEST(unsigned short);
}

BOOST_AUTO_TEST_CASE(simple_buffer_unsigned_int)
{
    GEN_TEST(unsigned int);
}

BOOST_AUTO_TEST_CASE(simple_buffer_unsigned_long)
{
    GEN_TEST(unsigned long);
}

BOOST_AUTO_TEST_CASE(simple_buffer_unsigned_long_long)
{
    GEN_TEST(unsigned long long);
}

BOOST_AUTO_TEST_CASE(simple_buffer_uint8)
{
    GEN_TEST(uint8_t);
}

BOOST_AUTO_TEST_CASE(simple_buffer_uint16)
{
    GEN_TEST(uint16_t);
}

BOOST_AUTO_TEST_CASE(simple_buffer_uint32)
{
    GEN_TEST(uint32_t);
}

BOOST_AUTO_TEST_CASE(simple_buffer_uint64)
{
    GEN_TEST(uint64_t);
}

BOOST_AUTO_TEST_CASE(simple_buffer_int8)
{
    GEN_TEST(int8_t);
}

BOOST_AUTO_TEST_CASE(simple_buffer_int16)
{
    GEN_TEST(int16_t);
}

BOOST_AUTO_TEST_CASE(simple_buffer_int32)
{
    GEN_TEST(int32_t);
}

BOOST_AUTO_TEST_CASE(simple_buffer_int64)
{
    GEN_TEST(int64_t);
}

#if !defined(_MSC_VER) || _MSC_VER >=1800

BOOST_AUTO_TEST_CASE(simple_buffer_float)
{
    vector<float> v;
    v.push_back(0.0);
    v.push_back(-0.0);
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
        msgpack::sbuffer sbuf;
        float val1 = v[i];
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        float val2 = oh.get().as<float>();

        if (std::isnan(val1))
            BOOST_CHECK(std::isnan(val2));
        else if (std::isinf(val1))
            BOOST_CHECK(std::isinf(val2));
        else
            BOOST_CHECK(fabs(val2 - val1) <= kEPS);
    }
}

#endif // !defined(_MSC_VER) || _MSC_VER >=1800

namespace {
template<typename F, typename I>
struct TypePair {
    typedef F float_type;
    typedef I integer_type;
};
} // namespace

typedef boost::mpl::list<TypePair<float, signed long long>,
                         TypePair<float, unsigned long long>,
                         TypePair<double, signed long long>,
                         TypePair<double, unsigned long long> > IntegerToFloatingPointTestTypes;

BOOST_AUTO_TEST_CASE_TEMPLATE(simple_buffer, TypeParam, IntegerToFloatingPointTestTypes)
{
    typedef typename TypeParam::float_type float_type;
    typedef typename TypeParam::integer_type integer_type;
    vector<integer_type> v;
    v.push_back(0);
    v.push_back(1);
    if (numeric_limits<integer_type>::is_signed) v.push_back(static_cast<integer_type>(-1));
    else v.push_back(2);
    for (unsigned int i = 0; i < kLoop; i++) {
        v.push_back(static_cast<integer_type>(rand() % 0x7FFFFF));
    }
    for (unsigned int i = 0; i < v.size() ; i++) {
        msgpack::sbuffer sbuf;
        integer_type val1 = v[i];
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        float_type val2 = oh.get().as<float_type>();
        BOOST_CHECK(fabs(val2 - static_cast<float_type>(val1)) <= kEPS);
    }
}

#if !defined(_MSC_VER) || _MSC_VER >=1800

BOOST_AUTO_TEST_CASE(simple_buffer_double)
{
    vector<double> v;
    v.push_back(0.0);
    v.push_back(-0.0);
    v.push_back(1.0);
    v.push_back(-1.0);
    v.push_back(numeric_limits<double>::min());
    v.push_back(numeric_limits<double>::max());
    v.push_back(nanf("tag"));
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

    for (unsigned int i = 0; i < kLoop; i++) {
        v.push_back(msgpack_rand());
        v.push_back(-msgpack_rand());
    }
    for (unsigned int i = 0; i < v.size() ; i++) {
        msgpack::sbuffer sbuf;
        double val1 = v[i];
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        double val2 = oh.get().as<double>();

        if (std::isnan(val1))
            BOOST_CHECK(std::isnan(val2));
        else if (std::isinf(val1))
            BOOST_CHECK(std::isinf(val2));
        else
            BOOST_CHECK(fabs(val2 - val1) <= kEPS);
    }
}

#endif // !defined(_MSC_VER) || _MSC_VER >=1800

BOOST_AUTO_TEST_CASE(simple_buffer_nil)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    packer.pack_nil();
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::NIL);
}

BOOST_AUTO_TEST_CASE(simple_buffer_true)
{
    msgpack::sbuffer sbuf;
    bool val1 = true;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    bool val2 = oh.get().as<bool>();
    BOOST_CHECK_EQUAL(val1, val2);
}

BOOST_AUTO_TEST_CASE(simple_buffer_false)
{
    msgpack::sbuffer sbuf;
    bool val1 = false;
    msgpack::pack(sbuf, val1);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    bool val2 = oh.get().as<bool>();
    BOOST_CHECK_EQUAL(val1, val2);
}

BOOST_AUTO_TEST_CASE(simple_buffer_fixext1)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char const buf [] = { 2 };

    packer.pack_ext(sizeof(buf), 1);
    packer.pack_ext_body(buf, sizeof(buf));
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(1ul, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(1, oh.get().via.ext.type());
    BOOST_CHECK_EQUAL(2, oh.get().via.ext.data()[0]);

    msgpack::sbuffer sbuf2;
    msgpack::pack(sbuf2, oh.get());
    msgpack::object_handle oh2 =
        msgpack::unpack(sbuf2.data(), sbuf2.size());
    BOOST_CHECK_EQUAL(1ul, oh2.get().via.ext.size);
    BOOST_CHECK_EQUAL(1, oh2.get().via.ext.type());
    BOOST_CHECK_EQUAL(2, oh2.get().via.ext.data()[0]);

    BOOST_CHECK_EQUAL(oh.get(), oh2.get());
}

BOOST_AUTO_TEST_CASE(simple_buffer_fixext2)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char const buf [] = { 2, 3 };

    packer.pack_ext(sizeof(buf), 0);
    packer.pack_ext_body(buf, sizeof(buf));
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(2ul, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(0, oh.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh.get().via.ext.data()));

    msgpack::sbuffer sbuf2;
    msgpack::pack(sbuf2, oh.get());
    msgpack::object_handle oh2 =
        msgpack::unpack(sbuf2.data(), sbuf2.size());
    BOOST_CHECK_EQUAL(2ul, oh2.get().via.ext.size);
    BOOST_CHECK_EQUAL(0, oh2.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh2.get().via.ext.data()));

    BOOST_CHECK_EQUAL(oh.get(), oh2.get());
}

BOOST_AUTO_TEST_CASE(simple_buffer_fixext4)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char const buf [] = { 2, 3, 4, 5 };

    packer.pack_ext(sizeof(buf), 1);
    packer.pack_ext_body(buf, sizeof(buf));
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(4ul, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(1, oh.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh.get().via.ext.data()));

    msgpack::sbuffer sbuf2;
    msgpack::pack(sbuf2, oh.get());
    msgpack::object_handle oh2 =
        msgpack::unpack(sbuf2.data(), sbuf2.size());
    BOOST_CHECK_EQUAL(4ul, oh2.get().via.ext.size);
    BOOST_CHECK_EQUAL(1, oh2.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh2.get().via.ext.data()));

    BOOST_CHECK_EQUAL(oh.get(), oh2.get());
}

BOOST_AUTO_TEST_CASE(simple_buffer_fixext8)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char const buf [] = { 2, 3, 4, 5, 6, 7, 8, 9 };

    packer.pack_ext(sizeof(buf), 1);
    packer.pack_ext_body(buf, sizeof(buf));
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(8ul, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(1, oh.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh.get().via.ext.data()));

    msgpack::sbuffer sbuf2;
    msgpack::pack(sbuf2, oh.get());
    msgpack::object_handle oh2 =
        msgpack::unpack(sbuf2.data(), sbuf2.size());
    BOOST_CHECK_EQUAL(8ul, oh2.get().via.ext.size);
    BOOST_CHECK_EQUAL(1, oh2.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh2.get().via.ext.data()));

    BOOST_CHECK_EQUAL(oh.get(), oh2.get());
}

BOOST_AUTO_TEST_CASE(simple_buffer_fixext16)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char const buf [] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };

    packer.pack_ext(sizeof(buf), 1);
    packer.pack_ext_body(buf, sizeof(buf));
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(16ul, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(1, oh.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh.get().via.ext.data()));

    msgpack::sbuffer sbuf2;
    msgpack::pack(sbuf2, oh.get());
    msgpack::object_handle oh2 =
        msgpack::unpack(sbuf2.data(), sbuf2.size());
    BOOST_CHECK_EQUAL(16ul, oh2.get().via.ext.size);
    BOOST_CHECK_EQUAL(1, oh2.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh2.get().via.ext.data()));

    BOOST_CHECK_EQUAL(oh.get(), oh2.get());
}

BOOST_AUTO_TEST_CASE(simple_buffer_fixext_1byte_0)
{
    std::size_t const size = 0;
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);

    packer.pack_ext(size, 77);
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(size, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(77, oh.get().via.ext.type());
}

BOOST_AUTO_TEST_CASE(simple_buffer_fixext_1byte_255)
{
    std::size_t const size = 255;
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char buf[size];
    for (std::size_t i = 0; i != size; ++i) buf[i] = static_cast<char>(i);
    packer.pack_ext(sizeof(buf), 77);
    packer.pack_ext_body(buf, sizeof(buf));

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(size, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(77, oh.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh.get().via.ext.data()));

    msgpack::sbuffer sbuf2;
    msgpack::pack(sbuf2, oh.get());
    msgpack::object_handle oh2 =
        msgpack::unpack(sbuf2.data(), sbuf2.size());
    BOOST_CHECK_EQUAL(size, oh2.get().via.ext.size);
    BOOST_CHECK_EQUAL(77, oh2.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh2.get().via.ext.data()));

    BOOST_CHECK_EQUAL(oh.get(), oh2.get());
}

BOOST_AUTO_TEST_CASE(simple_buffer_fixext_2byte_256)
{
    std::size_t const size = 256;
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char buf[size];
    for (std::size_t i = 0; i != size; ++i) buf[i] = static_cast<char>(i);
    packer.pack_ext(sizeof(buf), 77);
    packer.pack_ext_body(buf, sizeof(buf));

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(size, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(77, oh.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh.get().via.ext.data()));

    msgpack::sbuffer sbuf2;
    msgpack::pack(sbuf2, oh.get());
    msgpack::object_handle oh2 =
        msgpack::unpack(sbuf2.data(), sbuf2.size());
    BOOST_CHECK_EQUAL(size, oh2.get().via.ext.size);
    BOOST_CHECK_EQUAL(77, oh2.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh2.get().via.ext.data()));

    BOOST_CHECK_EQUAL(oh.get(), oh2.get());
}

BOOST_AUTO_TEST_CASE(simple_buffer_fixext_2byte_65535)
{
    std::size_t const size = 65535;
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char buf[size];
    for (std::size_t i = 0; i != size; ++i) buf[i] = static_cast<char>(i);
    packer.pack_ext(sizeof(buf), 77);
    packer.pack_ext_body(buf, sizeof(buf));

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(size, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(77, oh.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh.get().via.ext.data()));

    msgpack::sbuffer sbuf2;
    msgpack::pack(sbuf2, oh.get());
    msgpack::object_handle oh2 =
        msgpack::unpack(sbuf2.data(), sbuf2.size());
    BOOST_CHECK_EQUAL(size, oh2.get().via.ext.size);
    BOOST_CHECK_EQUAL(77, oh2.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh2.get().via.ext.data()));

    BOOST_CHECK_EQUAL(oh.get(), oh2.get());
}

BOOST_AUTO_TEST_CASE(simple_buffer_fixext_4byte_65536)
{
    std::size_t const size = 65536;
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char buf[size];
    for (std::size_t i = 0; i != size; ++i) buf[i] = static_cast<char>(i);
    packer.pack_ext(sizeof(buf), 77);
    packer.pack_ext_body(buf, sizeof(buf));

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    BOOST_CHECK_EQUAL(size, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(77, oh.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh.get().via.ext.data()));

    msgpack::sbuffer sbuf2;
    msgpack::pack(sbuf2, oh.get());
    msgpack::object_handle oh2 =
        msgpack::unpack(sbuf2.data(), sbuf2.size());
    BOOST_CHECK_EQUAL(size, oh2.get().via.ext.size);
    BOOST_CHECK_EQUAL(77, oh2.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh2.get().via.ext.data()));

    BOOST_CHECK_EQUAL(oh.get(), oh2.get());
}

BOOST_AUTO_TEST_CASE(simple_buffer_ext_convert)
{
    std::size_t const size = 65536;
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char buf[size];
    for (std::size_t i = 0; i != size; ++i) buf[i] = static_cast<char>(i);
    packer.pack_ext(sizeof(buf), 77);
    packer.pack_ext_body(buf, sizeof(buf));

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    msgpack::type::ext e;
    oh.get().convert(e);
    BOOST_CHECK_EQUAL(size, e.size());
    BOOST_CHECK_EQUAL(77, e.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), e.data()));
}

BOOST_AUTO_TEST_CASE(simple_buffer_ext_pack_convert)
{
    std::size_t const size = 65536;
    msgpack::sbuffer sbuf;
    msgpack::type::ext val1(77, size);
    char* buf = val1.data();
    for (std::size_t i = 0; i != size; ++i) buf[i] = static_cast<char>(i);
    msgpack::pack(sbuf, val1);

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    msgpack::type::ext val2;
    oh.get().convert(val2);
    BOOST_CHECK_EQUAL(size, val2.size());
    BOOST_CHECK_EQUAL(77, val2.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), val2.data()));
}

BOOST_AUTO_TEST_CASE(simple_buffer_ext_ref_convert)
{
    std::size_t const size = 65536;
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> packer(sbuf);
    char buf[size];
    for (std::size_t i = 0; i != size; ++i) buf[i] = static_cast<char>(i);
    packer.pack_ext(sizeof(buf), 77);
    packer.pack_ext_body(buf, sizeof(buf));

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    msgpack::type::ext_ref er;
    oh.get().convert(er);
    BOOST_CHECK_EQUAL(size, er.size());
    BOOST_CHECK_EQUAL(77, er.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), er.data()));
}

BOOST_AUTO_TEST_CASE(simple_buffer_ext_ref_pack_convert)
{
    std::size_t const buf_size = 65536;
    std::size_t const data_size = buf_size - 1;
    msgpack::sbuffer sbuf;
    char buf[buf_size];
    buf[0] = static_cast<char>(77);
    for (std::size_t i = 0; i != data_size; ++i) buf[i + 1] = static_cast<char>(i);
    msgpack::pack(sbuf, msgpack::type::ext_ref(buf, buf_size));

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());
    msgpack::type::ext_ref val2;
    oh.get().convert(val2);
    BOOST_CHECK_EQUAL(data_size, val2.size());
    BOOST_CHECK_EQUAL(77, val2.type());
    BOOST_CHECK(
        std::equal(&buf[1], &buf[buf_size], val2.data()));
}

BOOST_AUTO_TEST_CASE(simple_buffer_string)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        string val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1 += static_cast<char>('a' + rand() % 26);
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());
        BOOST_CHECK_EQUAL(oh.get().type, msgpack::type::STR);
        string val2 = oh.get().as<string>();
        BOOST_CHECK_EQUAL(val1.size(), val2.size());
        BOOST_CHECK_EQUAL(val1, val2);
    }
}
