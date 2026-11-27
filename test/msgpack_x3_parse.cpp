#include "msgpack.hpp"

#include <sstream>
#include <limits>
#include <cmath>
#include <deque>
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <cstddef>
#include <cstdint>

#include <boost/spirit/include/support_multi_pass.hpp>

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

BOOST_AUTO_TEST_CASE(ext32_max_size_truncated)
{
    // ext32 header announcing a 0xffffffff byte payload, followed by only
    // a type byte and a positive fixint
    const char data[] = { '\xc9', '\xff', '\xff', '\xff', '\xff', 0x01, 0x02 };
    std::string str(data, sizeof(data));
    BOOST_CHECK_THROW(
        msgpack::unpack(str.begin(), str.end()),
        msgpack::unpack_error);
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

// The matched str/bin/ext range must be read through the iterators.
// std::deque and std::list are not contiguous, so the visitor must not
// read `size` bytes starting from the first element.
template <typename Container>
static Container to_container(std::string const& s) {
    return Container(s.begin(), s.end());
}

template <typename Container>
static void check_non_contiguous_str() {
    std::string v(5000, 'A');
    for (std::size_t i = 0; i < v.size(); ++i) v[i] = static_cast<char>('A' + i % 26);
    std::stringstream ss;
    msgpack::pack(ss, v);
    Container c = to_container<Container>(ss.str());

    auto oh = msgpack::unpack(c.begin(), c.end());
    BOOST_CHECK_EQUAL(v, oh.get().template as<std::string>());
}

template <typename Container>
static void check_non_contiguous_bin() {
    std::vector<char> v(5000);
    for (std::size_t i = 0; i < v.size(); ++i) v[i] = static_cast<char>(i % 251);
    std::stringstream ss;
    msgpack::packer<std::stringstream> pk(ss);
    pk.pack_bin(static_cast<uint32_t>(v.size()));
    pk.pack_bin_body(v.data(), static_cast<uint32_t>(v.size()));
    Container c = to_container<Container>(ss.str());

    auto oh = msgpack::unpack(c.begin(), c.end());
    BOOST_CHECK(oh.get().template as<std::vector<char> >() == v);
}

template <typename Container>
static void check_non_contiguous_ext() {
    std::vector<char> v(5000);
    for (std::size_t i = 0; i < v.size(); ++i) v[i] = static_cast<char>(i % 251);
    std::stringstream ss;
    msgpack::packer<std::stringstream> pk(ss);
    pk.pack_ext(v.size(), 7);
    pk.pack_ext_body(v.data(), static_cast<uint32_t>(v.size()));
    Container c = to_container<Container>(ss.str());

    auto oh = msgpack::unpack(c.begin(), c.end());
    msgpack::object const& o = oh.get();
    BOOST_REQUIRE_EQUAL(o.type, msgpack::type::EXT);
    BOOST_CHECK_EQUAL(o.via.ext.type(), 7);
    BOOST_REQUIRE_EQUAL(o.via.ext.size, v.size());
    BOOST_CHECK(std::equal(v.begin(), v.end(), o.via.ext.data()));
}

BOOST_AUTO_TEST_CASE(non_contiguous_deque_str) { check_non_contiguous_str<std::deque<char> >(); }
BOOST_AUTO_TEST_CASE(non_contiguous_list_str) { check_non_contiguous_str<std::list<char> >(); }
BOOST_AUTO_TEST_CASE(non_contiguous_deque_bin) { check_non_contiguous_bin<std::deque<char> >(); }
BOOST_AUTO_TEST_CASE(non_contiguous_list_bin) { check_non_contiguous_bin<std::list<char> >(); }
BOOST_AUTO_TEST_CASE(non_contiguous_deque_ext) { check_non_contiguous_ext<std::deque<char> >(); }
BOOST_AUTO_TEST_CASE(non_contiguous_list_ext) { check_non_contiguous_ext<std::list<char> >(); }

BOOST_AUTO_TEST_CASE(is_contiguous_helper)
{
    std::string s("0123456789");
    std::vector<char> v(s.begin(), s.end());
    std::list<char> l(s.begin(), s.end());
    BOOST_CHECK(msgpack::v2::detail::is_contiguous(s.begin(), s.end()));
    BOOST_CHECK(msgpack::v2::detail::is_contiguous(v.begin(), v.end()));
    BOOST_CHECK(msgpack::v2::detail::is_contiguous(s.data(), s.data() + s.size()));
    BOOST_CHECK(!msgpack::v2::detail::is_contiguous(l.begin(), l.end()));
}

#if defined(MSGPACK_X3_EXACT_CONTIGUITY)
// A random-access range that is not contiguous: element i is blocks[i / 8][i % 8]. The first and the
// last block sit at the two ends of one array and the block between them lives elsewhere, so the first
// and the last element are exactly size - 1 bytes apart. The endpoint check before C++20 cannot tell it
// from a contiguous range; with C++20 the iterator is not a std::contiguous_iterator and the data is copied.
class segmented_iterator {
public:
    typedef std::random_access_iterator_tag iterator_category;
    typedef char value_type;
    typedef std::ptrdiff_t difference_type;
    typedef char const* pointer;
    typedef char const& reference;
    segmented_iterator() : blocks_(nullptr), i_(0) {}
    segmented_iterator(char const* const* blocks, difference_type i) : blocks_(blocks), i_(i) {}
    reference operator*() const { return blocks_[i_ / 8][i_ % 8]; }
    pointer operator->() const { return &**this; }
    reference operator[](difference_type n) const { return *(*this + n); }
    segmented_iterator& operator++() { ++i_; return *this; }
    segmented_iterator operator++(int) { segmented_iterator t(*this); ++i_; return t; }
    segmented_iterator& operator--() { --i_; return *this; }
    segmented_iterator operator--(int) { segmented_iterator t(*this); --i_; return t; }
    segmented_iterator& operator+=(difference_type n) { i_ += n; return *this; }
    segmented_iterator& operator-=(difference_type n) { i_ -= n; return *this; }
    friend segmented_iterator operator+(segmented_iterator a, difference_type n) { return a += n; }
    friend segmented_iterator operator+(difference_type n, segmented_iterator a) { return a += n; }
    friend segmented_iterator operator-(segmented_iterator a, difference_type n) { return a -= n; }
    friend difference_type operator-(segmented_iterator const& a, segmented_iterator const& b) { return a.i_ - b.i_; }
    friend bool operator==(segmented_iterator const& a, segmented_iterator const& b) { return a.i_ == b.i_; }
    friend bool operator!=(segmented_iterator const& a, segmented_iterator const& b) { return a.i_ != b.i_; }
    friend bool operator<(segmented_iterator const& a, segmented_iterator const& b) { return a.i_ < b.i_; }
    friend bool operator>(segmented_iterator const& a, segmented_iterator const& b) { return a.i_ > b.i_; }
    friend bool operator<=(segmented_iterator const& a, segmented_iterator const& b) { return a.i_ <= b.i_; }
    friend bool operator>=(segmented_iterator const& a, segmented_iterator const& b) { return a.i_ >= b.i_; }
private:
    char const* const* blocks_;
    difference_type i_;
};

// bin8 with 22 bytes of payload is 24 bytes, laid out in three blocks of 8: the first and the last
// block at the two ends of one arena, the middle block elsewhere.
struct segmented_bin {
    std::vector<char> payload;
    char arena[24];
    char middle[8];
    char const* blocks[3];
    // blocks points into this object's own arrays
    segmented_bin(segmented_bin const&) = delete;
    segmented_bin& operator=(segmented_bin const&) = delete;
    segmented_bin() : payload(22) {
        std::string msg("\xc4\x16", 2);
        for (std::size_t i = 0; i < payload.size(); ++i) payload[i] = static_cast<char>('a' + i);
        msg.append(payload.begin(), payload.end());
        std::fill(arena, arena + 24, 'X');
        std::copy(msg.begin(), msg.begin() + 8, arena);
        std::copy(msg.begin() + 16, msg.end(), arena + 16);
        std::copy(msg.begin() + 8, msg.begin() + 16, middle);
        blocks[0] = arena; blocks[1] = middle; blocks[2] = arena + 16;
    }
    segmented_iterator begin() const { return segmented_iterator(blocks, 0); }
    segmented_iterator end() const { return segmented_iterator(blocks, 24); }
};

BOOST_AUTO_TEST_CASE(non_contiguous_segmented_bin_cxx20)
{
    segmented_bin m;
    BOOST_REQUIRE_EQUAL(reinterpret_cast<std::uintptr_t>(&*(m.end() - 1)) - reinterpret_cast<std::uintptr_t>(&*m.begin()), 23u);
    BOOST_CHECK(!msgpack::v2::detail::is_contiguous(m.begin(), m.end()));

    auto oh = msgpack::unpack(m.begin(), m.end());
    BOOST_CHECK(oh.get().as<std::vector<char> >() == m.payload);
}
#endif // defined(MSGPACK_X3_EXACT_CONTIGUITY)

// Reference mode (zero-copy) must only hand out pointers into the caller's
// own contiguous input. A buffering input iterator such as
// boost::spirit::multi_pass keeps its data in an internal buffer that is
// reallocated while parsing, so those pointers would dangle.
static bool reference_all(msgpack::type::object_type, std::size_t, void*) { return true; }

static std::string pack_strings(int n, std::size_t len) {
    std::stringstream ss;
    msgpack::packer<std::stringstream> pk(ss);
    pk.pack_array(static_cast<uint32_t>(n));
    for (int i = 0; i < n; ++i) {
        std::string e(len, static_cast<char>('A' + i));
        pk.pack_str(static_cast<uint32_t>(e.size()));
        pk.pack_str_body(e.data(), static_cast<uint32_t>(e.size()));
    }
    return ss.str();
}

BOOST_AUTO_TEST_CASE(reference_multi_pass)
{
    const int n = 8;
    std::string src = pack_strings(n, 600);
    std::istringstream iss(src);
    iss.unsetf(std::ios::skipws);
    typedef std::istreambuf_iterator<char> base_it;
    boost::spirit::multi_pass<base_it> first{base_it(iss)};
    boost::spirit::multi_pass<base_it> last{base_it()};

    bool referenced = true;
    auto oh = msgpack::unpack(first, last, referenced, reference_all, nullptr, msgpack::unpack_limit());
    BOOST_CHECK(!referenced);
    msgpack::object const& o = oh.get();
    BOOST_REQUIRE_EQUAL(o.type, msgpack::type::ARRAY);
    BOOST_REQUIRE_EQUAL(o.via.array.size, static_cast<uint32_t>(n));
    for (int i = 0; i < n; ++i) {
        BOOST_CHECK_EQUAL(o.via.array.ptr[i].as<std::string>(), std::string(600, static_cast<char>('A' + i)));
    }
}

BOOST_AUTO_TEST_CASE(reference_contiguous_still_zero_copy)
{
    std::string src = pack_strings(2, 600);
    {
        bool referenced = false;
        char const* b = src.data();
        char const* e = src.data() + src.size();
        auto oh = msgpack::unpack(b, e, referenced, reference_all, nullptr, msgpack::unpack_limit());
        BOOST_CHECK(referenced);
        char const* p = oh.get().via.array.ptr[0].via.str.ptr;
        BOOST_CHECK(p >= src.data() && p < src.data() + src.size());
    }
    {
        bool referenced = false;
        auto b = src.cbegin();
        auto e = src.cend();
        auto oh = msgpack::unpack(b, e, referenced, reference_all, nullptr, msgpack::unpack_limit());
        BOOST_CHECK(referenced);
        char const* p = oh.get().via.array.ptr[0].via.str.ptr;
        BOOST_CHECK(p >= src.data() && p < src.data() + src.size());
    }
}

#if defined(MSGPACK_X3_EXACT_CONTIGUITY)
BOOST_AUTO_TEST_CASE(reference_segmented_cxx20)
{
    segmented_bin m;
    bool referenced = true;
    auto b = m.begin();
    auto e = m.end();
    auto oh = msgpack::unpack(b, e, referenced, reference_all, nullptr, msgpack::unpack_limit());
    BOOST_CHECK(!referenced);
    BOOST_CHECK(oh.get().as<std::vector<char> >() == m.payload);
}
#endif // defined(MSGPACK_X3_EXACT_CONTIGUITY)

#endif // defined(MSGPACK_USE_X3_PARSE) && MSGPACK_DEFAULT_API_VERSION >= 2
