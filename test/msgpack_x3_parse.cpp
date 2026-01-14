#include "msgpack.hpp"

#include <sstream>
#include <limits>
#include <cmath>

#define BOOST_TEST_MODULE MSGPACK_X3_PARSE
#include <boost/test/unit_test.hpp>

// To avoid link error
BOOST_AUTO_TEST_CASE(dummy)
{
}

#if defined(MSGPACK_USE_X3_PARSE) && MSGPACK_DEFAULT_API_VERSION >= 2

using namespace std;

const double kEPS = 1e-10;

BOOST_AUTO_TEST_CASE(nil_t)
{
    msgpack::type::nil_t v;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK(oh.get().is_nil());
}

BOOST_AUTO_TEST_CASE(bool_false)
{
    bool v = false;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<bool>());
}

BOOST_AUTO_TEST_CASE(bool_true)
{
    bool v = true;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<bool>());
}

BOOST_AUTO_TEST_CASE(positive_fixint_1)
{
    uint8_t v = 0;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<uint8_t>());
}

BOOST_AUTO_TEST_CASE(positive_fixint_2)
{
    uint8_t v = 127;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<uint8_t>());
}

BOOST_AUTO_TEST_CASE(negative_fixint_1)
{
    int8_t v = -1;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<int8_t>());
}

BOOST_AUTO_TEST_CASE(negative_fixint_2)
{
    int8_t v = -32;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<int8_t>());
}

BOOST_AUTO_TEST_CASE(uint8_1)
{
    uint8_t v = 128U;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<uint8_t>());
}

BOOST_AUTO_TEST_CASE(uint8_2)
{
    uint8_t v = 0xffU;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<uint8_t>());
}

BOOST_AUTO_TEST_CASE(uint16_1)
{
    uint16_t v = 0x100U;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<uint16_t>());
}

BOOST_AUTO_TEST_CASE(uint16_2)
{
    uint16_t v = 0xffffU;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<uint16_t>());
}

BOOST_AUTO_TEST_CASE(uint32_1)
{
    uint32_t v = 0x10000UL;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<uint32_t>());
}

BOOST_AUTO_TEST_CASE(uint32_2)
{
    uint32_t v = 0xffffffffUL;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<uint32_t>());
}

BOOST_AUTO_TEST_CASE(uint64_1)
{
    uint64_t v = 0x100000000ULL;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<uint64_t>());
}

BOOST_AUTO_TEST_CASE(uint64_2)
{
    uint64_t v = 0xffffffffffffffffULL;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<uint64_t>());
}

BOOST_AUTO_TEST_CASE(int8_1)
{
    int8_t v = static_cast<int8_t>(0b11011111);
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<int8_t>());
}

BOOST_AUTO_TEST_CASE(int8_2)
{
    int8_t v = static_cast<int8_t>(0b10000000);
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<int8_t>());
}

BOOST_AUTO_TEST_CASE(int16_1)
{
    int16_t v = static_cast<int16_t>(0xff00);
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<int16_t>());
}

BOOST_AUTO_TEST_CASE(int16_2)
{
    int16_t v = static_cast<int16_t>(0x8000);
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<int16_t>());
}

BOOST_AUTO_TEST_CASE(int32_1)
{
    int32_t v = static_cast<int32_t>(0xff000000L);
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<int32_t>());
}

BOOST_AUTO_TEST_CASE(int32_2)
{
    int32_t v = static_cast<int32_t>(0x80000000L);
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<int32_t>());
}

BOOST_AUTO_TEST_CASE(int64_1)
{
    int64_t v = static_cast<int64_t>(0xff00000000000000LL);
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<int64_t>());
}

BOOST_AUTO_TEST_CASE(int64_2)
{
    int64_t v = static_cast<int64_t>(0x8000000000000000LL);
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<int64_t>());
}

BOOST_AUTO_TEST_CASE(array_1)
{
    std::vector<int> v;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK(v == oh.get().as<std::vector<int> >());
}

BOOST_AUTO_TEST_CASE(array_2)
{
    std::vector<int> v;
    std::stringstream ss;
    for (int i = 0; i != 0xffU; ++i) v.push_back(i);
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK(v == oh.get().as<std::vector<int> >());
}

BOOST_AUTO_TEST_CASE(array_3)
{
    std::vector<int> v;
    std::stringstream ss;
    for (int i = 0; i != 0xffU+1U; ++i) v.push_back(i);
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK(v == oh.get().as<std::vector<int> >());
}

BOOST_AUTO_TEST_CASE(array_4)
{
    std::vector<int> v;
    std::stringstream ss;
    for (int i = 0; i != 0xffffU; ++i) v.push_back(i);
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK(v == oh.get().as<std::vector<int> >());
}

BOOST_AUTO_TEST_CASE(array_5)
{
    std::vector<uint32_t> v;
    std::stringstream ss;
    for (uint32_t i = 0; i != 0xffffU+1U; ++i) v.push_back(i);
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK(v == oh.get().as<std::vector<uint32_t> >());
}

BOOST_AUTO_TEST_CASE(map_1)
{
    std::map<int, int> v;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK(v == (oh.get().as<std::map<int, int> >()));
}

BOOST_AUTO_TEST_CASE(map_2)
{
    std::map<int, int> v;
    std::stringstream ss;
    for (int i = 0; i != 0xffU; ++i) v.emplace(i, i);
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK(v == (oh.get().as<std::map<int, int> >()));
}

BOOST_AUTO_TEST_CASE(map_3)
{
    std::map<int, int> v;
    std::stringstream ss;
    for (int i = 0; i != 0xffU+1U; ++i) v.emplace(i, i);
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK(v == (oh.get().as<std::map<int, int> >()));
}

BOOST_AUTO_TEST_CASE(map_4)
{
    std::map<int, int> v;
    std::stringstream ss;
    for (int i = 0; i != 0xffffU; ++i) v.emplace(i, i);
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK(v == (oh.get().as<std::map<int, int> >()));
}

BOOST_AUTO_TEST_CASE(map_5)
{
    std::map<uint32_t, uint32_t> v;
    std::stringstream ss;
    for (uint32_t i = 0; i != 0xffffU+1U; ++i) v.emplace(i, i);
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK(v == (oh.get().as<std::map<uint32_t, uint32_t> >()));
}

BOOST_AUTO_TEST_CASE(float_1)
{
    std::vector<float> v;
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

    for (unsigned int i = 0; i < v.size() ; i++) {
        std::stringstream ss;
        float val1 = v[i];
        msgpack::pack(ss, val1);
        msgpack::object_handle oh =
            msgpack::unpack(ss.str().begin(), ss.str().end());
        float val2 = oh.get().as<float>();

        if (std::isnan(val1))
            BOOST_CHECK(std::isnan(val2));
        else if (std::isinf(val1))
            BOOST_CHECK(std::isinf(val2));
        else
            BOOST_CHECK(fabs(val2 - val1) <= kEPS);
    }
}

BOOST_AUTO_TEST_CASE(double_1)
{
    std::vector<double> v;
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

    for (unsigned int i = 0; i < v.size() ; i++) {
        std::stringstream ss;
        double val1 = v[i];
        msgpack::pack(ss, val1);
        msgpack::object_handle oh =
            msgpack::unpack(ss.str().begin(), ss.str().end());
        double val2 = oh.get().as<double>();

        if (std::isnan(val1))
            BOOST_CHECK(std::isnan(val2));
        else if (std::isinf(val1))
            BOOST_CHECK(std::isinf(val2));
        else
            BOOST_CHECK(fabs(val2 - val1) <= kEPS);
    }
}

BOOST_AUTO_TEST_CASE(string_1)
{
    std::string v;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<std::string>());
}

BOOST_AUTO_TEST_CASE(string_2)
{
    std::string v;

    for (uint64_t i = 0; i != 0x1fU; ++i) v.push_back(static_cast<char>('0'+(i%10)));

    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<std::string>());
}

BOOST_AUTO_TEST_CASE(string_3)
{
    std::string v;

    for (uint64_t i = 0; i != 0xffU; ++i) v.push_back(static_cast<char>('0'+(i%10)));

    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<std::string>());
}

BOOST_AUTO_TEST_CASE(string_4)
{
    std::string v;

    for (uint64_t i = 0; i != 0xffU+1U; ++i) v.push_back(static_cast<char>('0'+(i%10)));

    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<std::string>());
}

BOOST_AUTO_TEST_CASE(string_5)
{
    std::string v;

    for (uint64_t i = 0; i != 0xffffU; ++i) v.push_back(static_cast<char>('0'+(i%10)));

    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<std::string>());
}

BOOST_AUTO_TEST_CASE(string_6)
{
    std::string v;

    for (uint64_t i = 0; i != 0xffffUL + 1UL; ++i) v.push_back(static_cast<char>('0'+(i%10)));

    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, oh.get().as<std::string>());
}

BOOST_AUTO_TEST_CASE(bin_1)
{
    std::vector<char> v;
    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK(v == oh.get().as<std::vector<char>>());
}

BOOST_AUTO_TEST_CASE(bin_2)
{
    std::vector<char> v;

    for (uint64_t i = 0; i != 0x1fU; ++i) v.push_back(static_cast<char>(i%0xff));

    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK(v == oh.get().as<std::vector<char>>());
}

BOOST_AUTO_TEST_CASE(bin_3)
{
    std::vector<char> v;

    for (uint64_t i = 0; i != 0xffU; ++i) v.push_back(static_cast<char>(i%0xff));

    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK(v == oh.get().as<std::vector<char>>());
}

BOOST_AUTO_TEST_CASE(bin_4)
{
    std::vector<char> v;

    for (uint64_t i = 0; i != 0xffU+1U; ++i) v.push_back(static_cast<char>(i%0xff));

    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK(v == oh.get().as<std::vector<char>>());
}

BOOST_AUTO_TEST_CASE(bin_5)
{
    std::vector<char> v;

    for (uint64_t i = 0; i != 0xffffU; ++i) v.push_back(static_cast<char>(i%0xff));

    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK(v == oh.get().as<std::vector<char>>());
}

BOOST_AUTO_TEST_CASE(bin_6)
{
    std::vector<char> v;

    for (uint64_t i = 0; i != 0xffffUL + 1UL; ++i) v.push_back(static_cast<char>(i%0xff));

    std::stringstream ss;
    msgpack::pack(ss, v);

    auto oh = msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK(v == oh.get().as<std::vector<char>>());
}

BOOST_AUTO_TEST_CASE(fixext1)
{
    std::stringstream ss;
    msgpack::packer<std::stringstream> packer(ss);
    char const buf [] = { 2 };

    packer.pack_ext(sizeof(buf), 1);
    packer.pack_ext_body(buf, sizeof(buf));
    msgpack::object_handle oh =
        msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(1ul, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(1, oh.get().via.ext.type());
    BOOST_CHECK_EQUAL(2, oh.get().via.ext.data()[0]);
}

BOOST_AUTO_TEST_CASE(fixext2)
{
    std::stringstream ss;
    msgpack::packer<std::stringstream> packer(ss);
    char const buf [] = { 2, 3 };

    packer.pack_ext(sizeof(buf), 0);
    packer.pack_ext_body(buf, sizeof(buf));
    msgpack::object_handle oh =
        msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(2ul, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(0, oh.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh.get().via.ext.data()));
}

BOOST_AUTO_TEST_CASE(fixext4)
{
    std::stringstream ss;
    msgpack::packer<std::stringstream> packer(ss);
    char const buf [] = { 2, 3, 4, 5 };

    packer.pack_ext(sizeof(buf), 1);
    packer.pack_ext_body(buf, sizeof(buf));
    msgpack::object_handle oh =
        msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(4ul, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(1, oh.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh.get().via.ext.data()));
}

BOOST_AUTO_TEST_CASE(fixext8)
{
    std::stringstream ss;
    msgpack::packer<std::stringstream> packer(ss);
    char const buf [] = { 2, 3, 4, 5, 6, 7, 8, 9 };

    packer.pack_ext(sizeof(buf), 1);
    packer.pack_ext_body(buf, sizeof(buf));
    msgpack::object_handle oh =
        msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(8ul, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(1, oh.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh.get().via.ext.data()));
}

BOOST_AUTO_TEST_CASE(fixext16)
{
    std::stringstream ss;
    msgpack::packer<std::stringstream> packer(ss);
    char const buf [] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };

    packer.pack_ext(sizeof(buf), 1);
    packer.pack_ext_body(buf, sizeof(buf));
    msgpack::object_handle oh =
        msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(16ul, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(1, oh.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh.get().via.ext.data()));
}

BOOST_AUTO_TEST_CASE(ext_0)
{
    std::size_t const size = 0;
    std::stringstream ss;
    msgpack::packer<std::stringstream> packer(ss);

    packer.pack_ext(size, 77);
    msgpack::object_handle oh =
        msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(size, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(77, oh.get().via.ext.type());
}

BOOST_AUTO_TEST_CASE(ext_255)
{
    std::size_t const size = 255;
    std::stringstream ss;
    msgpack::packer<std::stringstream> packer(ss);
    char buf[size];
    for (std::size_t i = 0; i != size; ++i) buf[i] = static_cast<char>(i);
    packer.pack_ext(sizeof(buf), 77);
    packer.pack_ext_body(buf, sizeof(buf));

    msgpack::object_handle oh =
        msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(size, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(77, oh.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh.get().via.ext.data()));
}

BOOST_AUTO_TEST_CASE(ext_256)
{
    std::size_t const size = 256;
    std::stringstream ss;
    msgpack::packer<std::stringstream> packer(ss);
    char buf[size];
    for (std::size_t i = 0; i != size; ++i) buf[i] = static_cast<char>(i);
    packer.pack_ext(sizeof(buf), 77);
    packer.pack_ext_body(buf, sizeof(buf));

    msgpack::object_handle oh =
        msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(size, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(77, oh.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh.get().via.ext.data()));
}

BOOST_AUTO_TEST_CASE(ext_65535)
{
    std::size_t const size = 65535;
    std::stringstream ss;
    msgpack::packer<std::stringstream> packer(ss);
    char buf[size];
    for (std::size_t i = 0; i != size; ++i) buf[i] = static_cast<char>(i);
    packer.pack_ext(sizeof(buf), 77);
    packer.pack_ext_body(buf, sizeof(buf));

    msgpack::object_handle oh =
        msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(size, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(77, oh.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh.get().via.ext.data()));
}

BOOST_AUTO_TEST_CASE(ext_65536)
{
    std::size_t const size = 65536;
    std::stringstream ss;
    msgpack::packer<std::stringstream> packer(ss);
    char buf[size];
    for (std::size_t i = 0; i != size; ++i) buf[i] = static_cast<char>(i);
    packer.pack_ext(sizeof(buf), 77);
    packer.pack_ext_body(buf, sizeof(buf));

    msgpack::object_handle oh =
        msgpack::unpack(ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(size, oh.get().via.ext.size);
    BOOST_CHECK_EQUAL(77, oh.get().via.ext.type());
    BOOST_CHECK(
        std::equal(buf, buf + sizeof(buf), oh.get().via.ext.data()));
}

BOOST_AUTO_TEST_CASE(unpack_referenced_1)
{
    std::string v = "ABC";
    std::stringstream ss;
    msgpack::pack(ss, v);

    bool r;
    msgpack::object_handle oh =
        msgpack::unpack(ss.str().begin(), ss.str().end(), r);
    BOOST_CHECK(!r);
    BOOST_CHECK_EQUAL(v, oh.get().as<std::string>());
}

BOOST_AUTO_TEST_CASE(unpack_referenced_2)
{
    std::string v = "ABC";
    std::stringstream ss;
    msgpack::pack(ss, v);

    // copy is required because ss.str() returns temporary object.
    std::string str = ss.str();
    bool r;
    msgpack::object_handle oh =
        msgpack::unpack(
            str.begin(),
            str.end(),
            r,
            [](msgpack::type::object_type, std::size_t, void*) {
                return true;
            }
        );
    BOOST_CHECK(r);
    BOOST_CHECK_EQUAL(v, oh.get().as<std::string>());
}

BOOST_AUTO_TEST_CASE(unpack_zone_1)
{
    std::string v = "ABC";
    std::stringstream ss;
    msgpack::pack(ss, v);

    msgpack::zone z;
    msgpack::object obj =
        msgpack::unpack(z, ss.str().begin(), ss.str().end());
    BOOST_CHECK_EQUAL(v, obj.as<std::string>());
}

BOOST_AUTO_TEST_CASE(unpack_zone_2)
{
    std::string v = "ABC";
    std::stringstream ss;
    msgpack::pack(ss, v);

    msgpack::zone z;
    bool r;
    msgpack::object obj =
        msgpack::unpack(z, ss.str().begin(), ss.str().end(), r);
    BOOST_CHECK_EQUAL(v, obj.as<std::string>());

    BOOST_CHECK(!r);
    BOOST_CHECK_EQUAL(v, obj.as<std::string>());
}

BOOST_AUTO_TEST_CASE(unpack_zone_3)
{
    std::string v = "ABC";
    std::stringstream ss;
    msgpack::pack(ss, v);

    // copy is required because ss.str() returns temporary object.
    std::string str = ss.str();
    msgpack::zone z;
    bool r;
    msgpack::object obj =
        msgpack::unpack(
            z,
            str.begin(),
            str.end(),
            r,
            [](msgpack::type::object_type, std::size_t, void*) {
                return true;
            }
        );
    BOOST_CHECK(r);
    BOOST_CHECK_EQUAL(v, obj.as<std::string>());
}

#endif // defined(MSGPACK_USE_X3_PARSE) && MSGPACK_DEFAULT_API_VERSION >= 2
