#include "msgpack.hpp"

#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <set>
#include <list>
#include <limits>

#include <gtest/gtest.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

const unsigned int kLoop = 10000;
const unsigned int kElements = 100;
const double kEPS = 1e-10;

#define GEN_TEST(test_type)                                           \
  do {                                                                \
    vector<test_type> v;                                              \
    v.push_back(0);                                                   \
    v.push_back(1);                                                   \
    v.push_back(2);                                                   \
    v.push_back(numeric_limits<test_type>::min());                    \
    v.push_back(numeric_limits<test_type>::max());                    \
    for (unsigned int i = 0; i < kLoop; i++)                          \
      v.push_back(rand());                                            \
    for (unsigned int i = 0; i < v.size() ; i++) {                    \
      msgpack::sbuffer sbuf;                                          \
      test_type val1 = v[i];                                          \
      msgpack::pack(sbuf, val1);                                      \
      msgpack::unpacked ret;                                          \
      msgpack::unpack(ret, sbuf.data(), sbuf.size());                 \
      EXPECT_EQ(val1, ret.get().as<test_type>());                     \
    }                                                                 \
} while(0)

TEST(MSGPACK, simple_buffer_char)
{
  GEN_TEST(char);
}

TEST(MSGPACK, simple_buffer_signed_char)
{
  GEN_TEST(signed char);
}

TEST(MSGPACK, simple_buffer_unsigned_char)
{
  GEN_TEST(unsigned char);
}


TEST(MSGPACK, simple_buffer_short)
{
  GEN_TEST(short);
}

TEST(MSGPACK, simple_buffer_int)
{
  GEN_TEST(int);
}

TEST(MSGPACK, simple_buffer_long)
{
  GEN_TEST(long);
}

TEST(MSGPACK, simple_buffer_long_long)
{
  GEN_TEST(long long);
}

TEST(MSGPACK, simple_buffer_unsigned_short)
{
  GEN_TEST(unsigned short);
}

TEST(MSGPACK, simple_buffer_unsigned_int)
{
  GEN_TEST(unsigned int);
}

TEST(MSGPACK, simple_buffer_unsigned_long)
{
  GEN_TEST(unsigned long);
}

TEST(MSGPACK, simple_buffer_unsigned_long_long)
{
  GEN_TEST(unsigned long long);
}

TEST(MSGPACK, simple_buffer_uint8)
{
  GEN_TEST(uint8_t);
}

TEST(MSGPACK, simple_buffer_uint16)
{
  GEN_TEST(uint16_t);
}

TEST(MSGPACK, simple_buffer_uint32)
{
  GEN_TEST(uint32_t);
}

TEST(MSGPACK, simple_buffer_uint64)
{
  GEN_TEST(uint64_t);
}

TEST(MSGPACK, simple_buffer_int8)
{
  GEN_TEST(int8_t);
}

TEST(MSGPACK, simple_buffer_int16)
{
  GEN_TEST(int16_t);
}

TEST(MSGPACK, simple_buffer_int32)
{
  GEN_TEST(int32_t);
}

TEST(MSGPACK, simple_buffer_int64)
{
  GEN_TEST(int64_t);
}

TEST(MSGPACK, simple_buffer_float)
{
  vector<float> v;
  v.push_back(0.0);
  v.push_back(-0.0);
  v.push_back(1.0);
  v.push_back(-1.0);
  v.push_back(numeric_limits<float>::min());
  v.push_back(numeric_limits<float>::max());
  v.push_back(nanf("tag"));
  v.push_back(1.0/0.0); // inf
  v.push_back(-(1.0/0.0)); // -inf
  for (unsigned int i = 0; i < kLoop; i++) {
    v.push_back(drand48());
    v.push_back(-drand48());
  }
  for (unsigned int i = 0; i < v.size() ; i++) {
    msgpack::sbuffer sbuf;
    float val1 = v[i];
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    float val2 = ret.get().as<float>();

    if (std::isnan(val1))
      EXPECT_TRUE(std::isnan(val2));
    else if (std::isinf(val1))
      EXPECT_TRUE(std::isinf(val2));
    else
      EXPECT_TRUE(fabs(val2 - val1) <= kEPS);
  }
}

namespace {
template<typename F, typename I>
struct TypePair {
  typedef F float_type;
  typedef I integer_type;
};
} // namespace

template <typename T>
class IntegerToFloatingPointTest : public testing::Test {
};
TYPED_TEST_CASE_P(IntegerToFloatingPointTest);

TYPED_TEST_P(IntegerToFloatingPointTest, simple_buffer)
{
  typedef typename TypeParam::float_type float_type;
  typedef typename TypeParam::integer_type integer_type;
  vector<integer_type> v;
  v.push_back(0);
  v.push_back(1);
  if (numeric_limits<integer_type>::is_signed) v.push_back(-1);
  else v.push_back(2);
  for (unsigned int i = 0; i < kLoop; i++) {
    v.push_back(rand() % 0x7FFFFF);
  }
  for (unsigned int i = 0; i < v.size() ; i++) {
    msgpack::sbuffer sbuf;
    integer_type val1 = v[i];
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    float_type val2 = ret.get().as<float_type>();
    EXPECT_TRUE(fabs(val2 - val1) <= kEPS);
  }
}

REGISTER_TYPED_TEST_CASE_P(IntegerToFloatingPointTest,
                           simple_buffer);

typedef testing::Types<TypePair<float, signed long long>,
                       TypePair<float, unsigned long long>,
                       TypePair<double, signed long long>,
                       TypePair<double, unsigned long long> > IntegerToFloatingPointTestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(IntegerToFloatingPointTestInstance,
                              IntegerToFloatingPointTest,
                              IntegerToFloatingPointTestTypes);

TEST(MSGPACK, simple_buffer_double)
{
  vector<double> v;
  v.push_back(0.0);
  v.push_back(-0.0);
  v.push_back(1.0);
  v.push_back(-1.0);
  v.push_back(numeric_limits<double>::min());
  v.push_back(numeric_limits<double>::max());
  v.push_back(nanf("tag"));
  v.push_back(1.0/0.0); // inf
  v.push_back(-(1.0/0.0)); // -inf
  for (unsigned int i = 0; i < kLoop; i++) {
    v.push_back(drand48());
    v.push_back(-drand48());
  }
  for (unsigned int i = 0; i < v.size() ; i++) {
    msgpack::sbuffer sbuf;
    double val1 = v[i];
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    double val2 = ret.get().as<double>();

    if (std::isnan(val1))
      EXPECT_TRUE(std::isnan(val2));
    else if (std::isinf(val1))
      EXPECT_TRUE(std::isinf(val2));
    else
      EXPECT_TRUE(fabs(val2 - val1) <= kEPS);
  }
}

TEST(MSGPACK, simple_buffer_true)
{
  msgpack::sbuffer sbuf;
  bool val1 = true;
  msgpack::pack(sbuf, val1);
  msgpack::unpacked ret;
  msgpack::unpack(ret, sbuf.data(), sbuf.size());
  bool val2 = ret.get().as<bool>();
  EXPECT_EQ(val1, val2);
}

TEST(MSGPACK, simple_buffer_false)
{
  msgpack::sbuffer sbuf;
  bool val1 = false;
  msgpack::pack(sbuf, val1);
  msgpack::unpacked ret;
  msgpack::unpack(ret, sbuf.data(), sbuf.size());
  bool val2 = ret.get().as<bool>();
  EXPECT_EQ(val1, val2);
}

TEST(MSGPACK, simple_buffer_fixext1)
{
  msgpack::sbuffer sbuf;
  msgpack::packer<msgpack::sbuffer> packer(sbuf);
  char const buf [] = { 2 };

  packer.pack_ext(sizeof(buf), 1);
  packer.pack_ext_body(buf, sizeof(buf));
  msgpack::unpacked ret;
  msgpack::unpack(ret, sbuf.data(), sbuf.size());
  EXPECT_EQ(1, ret.get().via.ext.size);
  EXPECT_EQ(1, ret.get().via.ext.type());
  EXPECT_EQ(2, ret.get().via.ext.data()[0]);
}

TEST(MSGPACK, simple_buffer_fixext2)
{
  msgpack::sbuffer sbuf;
  msgpack::packer<msgpack::sbuffer> packer(sbuf);
  char const buf [] = { 2, 3 };

  packer.pack_ext(sizeof(buf), 0);
  packer.pack_ext_body(buf, sizeof(buf));
  msgpack::unpacked ret;
  msgpack::unpack(ret, sbuf.data(), sbuf.size());
  EXPECT_EQ(2, ret.get().via.ext.size);
  EXPECT_EQ(0, ret.get().via.ext.type());
  EXPECT_TRUE(
      std::equal(buf, buf + sizeof(buf), ret.get().via.ext.data()));
}

TEST(MSGPACK, simple_buffer_fixext4)
{
  msgpack::sbuffer sbuf;
  msgpack::packer<msgpack::sbuffer> packer(sbuf);
  char const buf [] = { 2, 3, 4, 5 };

  packer.pack_ext(sizeof(buf), 1);
  packer.pack_ext_body(buf, sizeof(buf));
  msgpack::unpacked ret;
  msgpack::unpack(ret, sbuf.data(), sbuf.size());
  EXPECT_EQ(4, ret.get().via.ext.size);
  EXPECT_EQ(1, ret.get().via.ext.type());
  EXPECT_TRUE(
      std::equal(buf, buf + sizeof(buf), ret.get().via.ext.data()));
}

TEST(MSGPACK, simple_buffer_fixext8)
{
  msgpack::sbuffer sbuf;
  msgpack::packer<msgpack::sbuffer> packer(sbuf);
  char const buf [] = { 2, 3, 4, 5, 6, 7, 8, 9 };

  packer.pack_ext(sizeof(buf), 1);
  packer.pack_ext_body(buf, sizeof(buf));
  msgpack::unpacked ret;
  msgpack::unpack(ret, sbuf.data(), sbuf.size());
  EXPECT_EQ(8, ret.get().via.ext.size);
  EXPECT_EQ(1, ret.get().via.ext.type());
  EXPECT_TRUE(
      std::equal(buf, buf + sizeof(buf), ret.get().via.ext.data()));
}

TEST(MSGPACK, simple_buffer_fixext16)
{
  msgpack::sbuffer sbuf;
  msgpack::packer<msgpack::sbuffer> packer(sbuf);
  char const buf [] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };

  packer.pack_ext(sizeof(buf), 1);
  packer.pack_ext_body(buf, sizeof(buf));
  msgpack::unpacked ret;
  msgpack::unpack(ret, sbuf.data(), sbuf.size());
  EXPECT_EQ(16, ret.get().via.ext.size);
  EXPECT_EQ(1, ret.get().via.ext.type());
  EXPECT_TRUE(
      std::equal(buf, buf + sizeof(buf), ret.get().via.ext.data()));
}

TEST(MSGPACK, simple_buffer_fixext_1byte_0)
{
  std::size_t const size = 0;
  msgpack::sbuffer sbuf;
  msgpack::packer<msgpack::sbuffer> packer(sbuf);

  packer.pack_ext(size, 77);
  msgpack::unpacked ret;
  msgpack::unpack(ret, sbuf.data(), sbuf.size());
  EXPECT_EQ(size, ret.get().via.ext.size);
  EXPECT_EQ(77, ret.get().via.ext.type());
}

TEST(MSGPACK, simple_buffer_fixext_1byte_255)
{
  std::size_t const size = 255;
  msgpack::sbuffer sbuf;
  msgpack::packer<msgpack::sbuffer> packer(sbuf);
  char buf[size];
  for (int i = 0; i != size; ++i) buf[i] = static_cast<char>(i);
  packer.pack_ext(sizeof(buf), 77);
  packer.pack_ext_body(buf, sizeof(buf));

  msgpack::unpacked ret;
  msgpack::unpack(ret, sbuf.data(), sbuf.size());
  EXPECT_EQ(size, ret.get().via.ext.size);
  EXPECT_EQ(77, ret.get().via.ext.type());
  EXPECT_TRUE(
      std::equal(buf, buf + sizeof(buf), ret.get().via.ext.data()));
}

TEST(MSGPACK, simple_buffer_fixext_2byte_256)
{
  std::size_t const size = 256;
  msgpack::sbuffer sbuf;
  msgpack::packer<msgpack::sbuffer> packer(sbuf);
  char buf[size];
  for (int i = 0; i != size; ++i) buf[i] = static_cast<char>(i);
  packer.pack_ext(sizeof(buf), 77);
  packer.pack_ext_body(buf, sizeof(buf));

  msgpack::unpacked ret;
  msgpack::unpack(ret, sbuf.data(), sbuf.size());
  EXPECT_EQ(size, ret.get().via.ext.size);
  EXPECT_EQ(77, ret.get().via.ext.type());
  EXPECT_TRUE(
      std::equal(buf, buf + sizeof(buf), ret.get().via.ext.data()));
}

TEST(MSGPACK, simple_buffer_fixext_2byte_65535)
{
  std::size_t const size = 65535;
  msgpack::sbuffer sbuf;
  msgpack::packer<msgpack::sbuffer> packer(sbuf);
  char buf[size];
  for (int i = 0; i != size; ++i) buf[i] = static_cast<char>(i);
  packer.pack_ext(sizeof(buf), 77);
  packer.pack_ext_body(buf, sizeof(buf));

  msgpack::unpacked ret;
  msgpack::unpack(ret, sbuf.data(), sbuf.size());
  EXPECT_EQ(size, ret.get().via.ext.size);
  EXPECT_EQ(77, ret.get().via.ext.type());
  EXPECT_TRUE(
      std::equal(buf, buf + sizeof(buf), ret.get().via.ext.data()));
}

TEST(MSGPACK, simple_buffer_fixext_4byte_65536)
{
  std::size_t const size = 65536;
  msgpack::sbuffer sbuf;
  msgpack::packer<msgpack::sbuffer> packer(sbuf);
  char buf[size];
  for (int i = 0; i != size; ++i) buf[i] = static_cast<char>(i);
  packer.pack_ext(sizeof(buf), 77);
  packer.pack_ext_body(buf, sizeof(buf));

  msgpack::unpacked ret;
  msgpack::unpack(ret, sbuf.data(), sbuf.size());
  EXPECT_EQ(size, ret.get().via.ext.size);
  EXPECT_EQ(77, ret.get().via.ext.type());
  EXPECT_TRUE(
      std::equal(buf, buf + sizeof(buf), ret.get().via.ext.data()));
}

//-----------------------------------------------------------------------------

// STL

TEST(MSGPACK_STL, simple_buffer_string)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    string val1;
    for (unsigned int i = 0; i < kElements; i++)
      val1 += 'a' + rand() % 26;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    EXPECT_EQ(ret.get().type, msgpack::type::STR);
    string val2 = ret.get().as<string>();
    EXPECT_EQ(val1.size(), val2.size());
    EXPECT_EQ(val1, val2);
  }
}

TEST(MSGPACK_STL, simple_buffer_cstring)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    string val1;
    for (unsigned int i = 0; i < kElements; i++)
      val1 += 'a' + rand() % 26;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1.c_str());
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    EXPECT_EQ(ret.get().type, msgpack::type::STR);
    string val2 = ret.get().as<string>();
    EXPECT_EQ(val1.size(), val2.size());
    EXPECT_EQ(val1, val2);
  }
}

TEST(MSGPACK_STL, simple_buffer_vector)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    vector<int> val1;
    for (unsigned int i = 0; i < kElements; i++)
      val1.push_back(rand());
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    EXPECT_EQ(ret.get().type, msgpack::type::ARRAY);
    vector<int> val2 = ret.get().as<vector<int> >();
    EXPECT_EQ(val1.size(), val2.size());
    EXPECT_TRUE(equal(val1.begin(), val1.end(), val2.begin()));
  }
}

TEST(MSGPACK_STL, simple_buffer_vector_char)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    vector<char> val1;
    for (unsigned int i = 0; i < kElements; i++)
      val1.push_back(rand());
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    EXPECT_EQ(ret.get().type, msgpack::type::BIN);
    vector<char> val2 = ret.get().as<vector<char> >();
    EXPECT_EQ(val1.size(), val2.size());
    EXPECT_TRUE(equal(val1.begin(), val1.end(), val2.begin()));
  }
}

TEST(MSGPACK_STL, simple_buffer_map)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    map<int, int> val1;
    for (unsigned int i = 0; i < kElements; i++)
      val1[rand()] = rand();
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    map<int, int> val2 = ret.get().as<map<int, int> >();
    EXPECT_EQ(val1.size(), val2.size());
    EXPECT_TRUE(equal(val1.begin(), val1.end(), val2.begin()));
  }
}

TEST(MSGPACK_STL, simple_buffer_deque)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    deque<int> val1;
    for (unsigned int i = 0; i < kElements; i++)
      val1.push_back(rand());
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    deque<int> val2 = ret.get().as<deque<int> >();
    EXPECT_EQ(val1.size(), val2.size());
    EXPECT_TRUE(equal(val1.begin(), val1.end(), val2.begin()));
  }
}

TEST(MSGPACK_STL, simple_buffer_list)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    list<int> val1;
    for (unsigned int i = 0; i < kElements; i++)
      val1.push_back(rand());
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    list<int> val2 = ret.get().as<list<int> >();
    EXPECT_EQ(val1.size(), val2.size());
    EXPECT_TRUE(equal(val1.begin(), val1.end(), val2.begin()));
  }
}

TEST(MSGPACK_STL, simple_buffer_set)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    set<int> val1;
    for (unsigned int i = 0; i < kElements; i++)
      val1.insert(rand());
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    set<int> val2 = ret.get().as<set<int> >();
    EXPECT_EQ(val1.size(), val2.size());
    EXPECT_TRUE(equal(val1.begin(), val1.end(), val2.begin()));
  }
}

TEST(MSGPACK_STL, simple_buffer_pair)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    pair<int, int> val1 = make_pair(rand(), rand());
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    pair<int, int> val2 = ret.get().as<pair<int, int> >();
    EXPECT_EQ(val1.first, val2.first);
    EXPECT_EQ(val1.second, val2.second);
  }
}

TEST(MSGPACK_STL, simple_buffer_multimap)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    multimap<int, int> val1;
    for (unsigned int i = 0; i < kElements; i++) {
      int i1 = rand();
      val1.insert(make_pair(i1, rand()));
      val1.insert(make_pair(i1, rand()));
    }
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    multimap<int, int> val2 = ret.get().as<multimap<int, int> >();

    vector<pair<int, int> > v1, v2;
    multimap<int, int>::const_iterator it;
    for (it = val1.begin(); it != val1.end(); ++it)
      v1.push_back(make_pair(it->first, it->second));
    for (it = val2.begin(); it != val2.end(); ++it)
      v2.push_back(make_pair(it->first, it->second));
    EXPECT_EQ(val1.size(), val2.size());
    EXPECT_EQ(v1.size(), v2.size());
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    EXPECT_TRUE(v1 == v2);
  }
}

TEST(MSGPACK_STL, simple_buffer_multiset)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    multiset<int> val1;
    for (unsigned int i = 0; i < kElements; i++)
      val1.insert(rand());
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    multiset<int> val2 = ret.get().as<multiset<int> >();

    vector<int> v1, v2;
    multiset<int>::const_iterator it;
    for (it = val1.begin(); it != val1.end(); ++it)
      v1.push_back(*it);
    for (it = val2.begin(); it != val2.end(); ++it)
      v2.push_back(*it);
    EXPECT_EQ(val1.size(), val2.size());
    EXPECT_EQ(v1.size(), v2.size());
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    EXPECT_TRUE(v1 == v2);
  }
}

// TR1

#ifdef MSGPACK_HAS_STD_TR1_UNOURDERED_MAP
#include <tr1/unordered_map>
#include "msgpack/adaptor/tr1/unordered_map.hpp"
TEST(MSGPACK_TR1, simple_buffer_tr1_unordered_map)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    tr1::unordered_map<int, int> val1;
    for (unsigned int i = 0; i < kElements; i++)
      val1[rand()] = rand();
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    tr1::unordered_map<int, int> val2 = ret.get().as<tr1::unordered_map<int, int> >();
    EXPECT_EQ(val1.size(), val2.size());
    tr1::unordered_map<int, int>::const_iterator it;
    for (it = val1.begin(); it != val1.end(); ++it) {
      EXPECT_TRUE(val2.find(it->first) != val2.end());
      EXPECT_EQ(it->second, val2.find(it->first)->second);
    }
  }
}

TEST(MSGPACK_TR1, simple_buffer_tr1_unordered_multimap)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    tr1::unordered_multimap<int, int> val1;
    for (unsigned int i = 0; i < kElements; i++) {
      int i1 = rand();
      val1.insert(make_pair(i1, rand()));
      val1.insert(make_pair(i1, rand()));
    }
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    tr1::unordered_multimap<int, int> val2 = ret.get().as<tr1::unordered_multimap<int, int> >();

    vector<pair<int, int> > v1, v2;
    tr1::unordered_multimap<int, int>::const_iterator it;
    for (it = val1.begin(); it != val1.end(); ++it)
      v1.push_back(make_pair(it->first, it->second));
    for (it = val2.begin(); it != val2.end(); ++it)
      v2.push_back(make_pair(it->first, it->second));
    EXPECT_EQ(val1.size(), val2.size());
    EXPECT_EQ(v1.size(), v2.size());
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    EXPECT_TRUE(v1 == v2);
  }
}
#endif

#ifdef MSGPACK_HAS_STD_TR1_UNOURDERED_SET
#include <tr1/unordered_set>
#include "msgpack/adaptor/tr1/unordered_set.hpp"
TEST(MSGPACK_TR1, simple_buffer_tr1_unordered_set)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    tr1::unordered_set<int> val1;
    for (unsigned int i = 0; i < kElements; i++)
      val1.insert(rand());
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    tr1::unordered_set<int> val2 = ret.get().as<tr1::unordered_set<int> >();
    EXPECT_EQ(val1.size(), val2.size());
    tr1::unordered_set<int>::const_iterator it;
    for (it = val1.begin(); it != val1.end(); ++it)
      EXPECT_TRUE(val2.find(*it) != val2.end());
  }
}

TEST(MSGPACK_TR1, simple_buffer_tr1_unordered_multiset)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    tr1::unordered_multiset<int> val1;
    for (unsigned int i = 0; i < kElements; i++)
      val1.insert(rand());
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    tr1::unordered_multiset<int> val2 = ret.get().as<tr1::unordered_multiset<int> >();

    vector<int> v1, v2;
    tr1::unordered_multiset<int>::const_iterator it;
    for (it = val1.begin(); it != val1.end(); ++it)
      v1.push_back(*it);
    for (it = val2.begin(); it != val2.end(); ++it)
      v2.push_back(*it);
    EXPECT_EQ(val1.size(), val2.size());
    EXPECT_EQ(v1.size(), v2.size());
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    EXPECT_TRUE(v1 == v2);
  }
}
#endif

#ifdef MSGPACK_HAS_STD_UNOURDERED_MAP
#include <unordered_map>
#include "msgpack/adaptor/tr1/unordered_map.hpp"
TEST(MSGPACK_TR1, simple_buffer_unordered_map)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    unordered_map<int, int> val1;
    for (unsigned int i = 0; i < kElements; i++)
      val1[rand()] = rand();
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    unordered_map<int, int> val2 = ret.get().as<unordered_map<int, int> >();
    EXPECT_EQ(val1.size(), val2.size());
    unordered_map<int, int>::const_iterator it;
    for (it = val1.begin(); it != val1.end(); ++it) {
      EXPECT_TRUE(val2.find(it->first) != val2.end());
      EXPECT_EQ(it->second, val2.find(it->first)->second);
    }
  }
}

TEST(MSGPACK_TR1, simple_buffer_unordered_multimap)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    unordered_multimap<int, int> val1;
    for (unsigned int i = 0; i < kElements; i++) {
      int i1 = rand();
      val1.insert(make_pair(i1, rand()));
      val1.insert(make_pair(i1, rand()));
    }
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    unordered_multimap<int, int> val2 = ret.get().as<unordered_multimap<int, int> >();

    vector<pair<int, int> > v1, v2;
    unordered_multimap<int, int>::const_iterator it;
    for (it = val1.begin(); it != val1.end(); ++it)
      v1.push_back(make_pair(it->first, it->second));
    for (it = val2.begin(); it != val2.end(); ++it)
      v2.push_back(make_pair(it->first, it->second));
    EXPECT_EQ(val1.size(), val2.size());
    EXPECT_EQ(v1.size(), v2.size());
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    EXPECT_TRUE(v1 == v2);
  }
}
#endif

#ifdef MSGPACK_HAS_STD_UNOURDERED_SET
#include <unordered_set>
#include "msgpack/adaptor/tr1/unordered_set.hpp"
TEST(MSGPACK_TR1, simple_buffer_unordered_set)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    unordered_set<int> val1;
    for (unsigned int i = 0; i < kElements; i++)
      val1.insert(rand());
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    unordered_set<int> val2 = ret.get().as<unordered_set<int> >();
    EXPECT_EQ(val1.size(), val2.size());
    unordered_set<int>::const_iterator it;
    for (it = val1.begin(); it != val1.end(); ++it)
      EXPECT_TRUE(val2.find(*it) != val2.end());
  }
}

TEST(MSGPACK_TR1, simple_buffer_unordered_multiset)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    unordered_multiset<int> val1;
    for (unsigned int i = 0; i < kElements; i++)
      val1.insert(rand());
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    unordered_multiset<int> val2 = ret.get().as<unordered_multiset<int> >();

    vector<int> v1, v2;
    unordered_multiset<int>::const_iterator it;
    for (it = val1.begin(); it != val1.end(); ++it)
      v1.push_back(*it);
    for (it = val2.begin(); it != val2.end(); ++it)
      v2.push_back(*it);
    EXPECT_EQ(val1.size(), val2.size());
    EXPECT_EQ(v1.size(), v2.size());
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    EXPECT_TRUE(v1 == v2);
  }
}
#endif


// User-Defined Structures

class TestClass
{
public:
  TestClass() : i(0), s("kzk") {}
  int i;
  string s;
  MSGPACK_DEFINE(i, s);
};

TEST(MSGPACK_USER_DEFINED, simple_buffer_class)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    TestClass val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    TestClass val2 = ret.get().as<TestClass>();
    EXPECT_EQ(val1.i, val2.i);
    EXPECT_EQ(val1.s, val2.s);
  }
}

class TestClass2
{
public:
  TestClass2() : i(0), s("kzk") {
    for (unsigned int i = 0; i < kElements; i++)
      v.push_back(rand());
  }
  int i;
  string s;
  vector<int> v;
  MSGPACK_DEFINE(i, s, v);
};

TEST(MSGPACK_USER_DEFINED, simple_buffer_class_old_to_new)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    TestClass val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    TestClass2 val2 = ret.get().as<TestClass2>();
    EXPECT_EQ(val1.i, val2.i);
    EXPECT_EQ(val1.s, val2.s);
    EXPECT_FALSE(val2.s.empty());
  }
}

TEST(MSGPACK_USER_DEFINED, simple_buffer_class_new_to_old)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    TestClass2 val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    TestClass val2 = ret.get().as<TestClass>();
    EXPECT_EQ(val1.i, val2.i);
    EXPECT_EQ(val1.s, val2.s);
    EXPECT_FALSE(val2.s.empty());
  }
}

class TestEnumMemberClass
{
public:
  TestEnumMemberClass()
    : t1(STATE_A), t2(STATE_B), t3(STATE_C) {}

  enum TestEnumType {
    STATE_INVALID = 0,
    STATE_A = 1,
    STATE_B = 2,
    STATE_C = 3
  };
  TestEnumType t1;
  TestEnumType t2;
  TestEnumType t3;

  MSGPACK_DEFINE(t1, t2, t3);
};

MSGPACK_ADD_ENUM(TestEnumMemberClass::TestEnumType);

TEST(MSGPACK_USER_DEFINED, simple_buffer_enum_member)
{
  TestEnumMemberClass val1;
  msgpack::sbuffer sbuf;
  msgpack::pack(sbuf, val1);
  msgpack::unpacked ret;
  msgpack::unpack(ret, sbuf.data(), sbuf.size());
  TestEnumMemberClass val2 = ret.get().as<TestEnumMemberClass>();
  EXPECT_EQ(val1.t1, val2.t1);
  EXPECT_EQ(val1.t2, val2.t2);
  EXPECT_EQ(val1.t3, val2.t3);
}

class TestUnionMemberClass
{
public:
  TestUnionMemberClass() {}
  TestUnionMemberClass(double f) {
    is_double = true;
    value.f = f;
  }
  TestUnionMemberClass(int i) {
    is_double = false;
    value.i = i;
  }

  union {
    double f;
    int i;
  } value;
  bool is_double;

  template <typename Packer>
  void msgpack_pack(Packer& pk) const
  {
    if (is_double)
      pk.pack(msgpack::type::tuple<bool, double>(true, value.f));
    else
      pk.pack(msgpack::type::tuple<bool, int>(false, value.i));
  }

  void msgpack_unpack(msgpack::object o)
  {
    msgpack::type::tuple<bool, msgpack::object> tuple;
    o.convert(&tuple);

    is_double = tuple.get<0>();
    if (is_double)
      tuple.get<1>().convert(&value.f);
    else
      tuple.get<1>().convert(&value.i);
  }
};

TEST(MSGPACK_USER_DEFINED, simple_buffer_union_member)
{
  {
    // double
    TestUnionMemberClass val1(1.0);
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    TestUnionMemberClass val2 = ret.get().as<TestUnionMemberClass>();
    EXPECT_EQ(val1.is_double, val2.is_double);
    EXPECT_TRUE(fabs(val1.value.f - val2.value.f) < kEPS);
  }
  {
    // int
    TestUnionMemberClass val1(1);
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    TestUnionMemberClass val2 = ret.get().as<TestUnionMemberClass>();
    EXPECT_EQ(val1.is_double, val2.is_double);
    EXPECT_EQ(val1.value.i, 1);
    EXPECT_EQ(val1.value.i, val2.value.i);
  }
}

//-----------------------------------------------------------------------------

#define GEN_TEST_VREF(test_type, vbuf)                                  \
  do {                                                                  \
    vector<test_type> v;                                                \
    v.push_back(0);                                                     \
    for (unsigned int i = 0; i < v.size(); i++) {                       \
      test_type val1 = v[i];                                            \
      msgpack::pack(vbuf, val1);                                        \
      msgpack::sbuffer sbuf;                                            \
      const struct iovec* cur = vbuf.vector();                          \
      const struct iovec* end = cur + vbuf.vector_size();               \
      for(; cur != end; ++cur)                                          \
        sbuf.write((const char*)cur->iov_base, cur->iov_len);           \
      msgpack::unpacked ret;                                            \
      msgpack::unpack(ret, sbuf.data(), sbuf.size());                   \
      test_type val2 = ret.get().as<test_type>();                       \
      EXPECT_EQ(val1, val2);                                            \
    }                                                                   \
  } while(0);

TEST(MSGPACK, vrefbuffer_char)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(char, vbuf);
}

TEST(MSGPACK, vrefbuffer_signed_char)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(signed char, vbuf);
}

TEST(MSGPACK, vrefbuffer_unsigned_char)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(unsigned char, vbuf);
}

TEST(MSGPACK, vrefbuffer_short)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(short, vbuf);
}

TEST(MSGPACK, vrefbuffer_int)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(int, vbuf);
}

TEST(MSGPACK, vrefbuffer_long)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(long, vbuf);
}

TEST(MSGPACK, vrefbuffer_long_long)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(long long, vbuf);
}

TEST(MSGPACK, vrefbuffer_unsigned_short)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(unsigned short, vbuf);
}

TEST(MSGPACK, vrefbuffer_unsigned_int)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(unsigned int, vbuf);
}

TEST(MSGPACK, vrefbuffer_unsigned_long)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(unsigned long, vbuf);
}

TEST(MSGPACK, vrefbuffer_unsigned_long_long)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(unsigned long long, vbuf);
}

TEST(MSGPACK, vrefbuffer_uint8)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(uint8_t, vbuf);
}

TEST(MSGPACK, vrefbuffer_uint16)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(uint16_t, vbuf);
}

TEST(MSGPACK, vrefbuffer_uint32)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(uint32_t, vbuf);
}

TEST(MSGPACK, vrefbuffer_uint64)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(uint64_t, vbuf);
}

TEST(MSGPACK, vrefbuffer_int8)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(int8_t, vbuf);
}

TEST(MSGPACK, vrefbuffer_int16)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(int16_t, vbuf);
}

TEST(MSGPACK, vrefbuffer_int32)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(int32_t, vbuf);
}

TEST(MSGPACK, vrefbuffer_int64)
{
  msgpack::vrefbuffer vbuf;
  GEN_TEST_VREF(int64_t, vbuf);
}

// small ref_size and chunk_size
TEST(MSGPACK, vrefbuffer_small_char)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(char, vbuf);
}

TEST(MSGPACK, vrefbuffer_small_signed_char)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(signed char, vbuf);
}

TEST(MSGPACK, vrefbuffer_small_unsigned_char)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(unsigned char, vbuf);
}

TEST(MSGPACK, vrefbuffer_small_short)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(short, vbuf);
}

TEST(MSGPACK, vrefbuffer_small_int)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(int, vbuf);
}

TEST(MSGPACK, vrefbuffer_small_long)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(long, vbuf);
}

TEST(MSGPACK, vrefbuffer_small_long_long)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(long long, vbuf);
}

TEST(MSGPACK, vrefbuffer_small_unsigned_short)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(unsigned short, vbuf);
}

TEST(MSGPACK, vrefbuffer_small_unsigned_int)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(unsigned int, vbuf);
}

TEST(MSGPACK, vrefbuffer_small_unsigned_long)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(unsigned long, vbuf);
}

TEST(MSGPACK, vrefbuffer_small_unsigned_long_long)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(unsigned long long, vbuf);
}

TEST(MSGPACK, vrefbuffer_small_uint8)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(uint8_t, vbuf);
}

TEST(MSGPACK, vrefbuffer_small_uint16)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(uint16_t, vbuf);
}

TEST(MSGPACK, vrefbuffer_small_uint32)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(uint32_t, vbuf);
}

TEST(MSGPACK, vrefbuffer_small_uint64)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(uint64_t, vbuf);
}

TEST(MSGPACK, vrefbuffer_small_int8)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(int8_t, vbuf);
}

TEST(MSGPACK, vrefbuffer_small_int16)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(int16_t, vbuf);
}

TEST(MSGPACK, vrefbuffer_small_int32)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(int32_t, vbuf);
}

TEST(MSGPACK, vrefbuffer_small_int64)
{
  msgpack::vrefbuffer vbuf(0, 0);
  GEN_TEST_VREF(int64_t, vbuf);
}

//-----------------------------------------------------------------------------

#define GEN_TEST_STREAM(test_type)                                      \
  for (unsigned int k = 0; k < kLoop; k++) {                            \
    msgpack::sbuffer sbuf;                                              \
    msgpack::packer<msgpack::sbuffer> pk(sbuf);                         \
    typedef std::vector<test_type> vec_type;                            \
    vec_type vec;                                                       \
    for(unsigned int i = 0; i < rand() % kLoop; ++i) {                  \
      vec_type::value_type r = rand();                                  \
      vec.push_back(r);                                                 \
      pk.pack(r);                                                       \
    }                                                                   \
    msgpack::unpacker pac;                                              \
    vec_type::const_iterator it = vec.begin();                          \
    const char *p = sbuf.data();                                        \
    const char * const pend = p + sbuf.size();                          \
    while (p < pend) {                                                  \
      const size_t sz = std::min<size_t>(pend - p, rand() % 128);       \
      pac.reserve_buffer(sz);                                           \
      memcpy(pac.buffer(), p, sz);                                      \
      pac.buffer_consumed(sz);                                          \
      msgpack::unpacked result;                                         \
      while (pac.next(result)) {                                        \
        if (it == vec.end()) goto out;                                  \
        msgpack::object obj = result.get();                             \
        vec_type::value_type val;                                       \
        obj.convert(&val);                                              \
        EXPECT_EQ(*it, val);                                            \
        ++it;                                                           \
      }                                                                 \
      p += sz;                                                          \
    }                                                                   \
  out:                                                                  \
    ;                                                                   \
  }

TEST(MSGPACK, stream_char)
{
  GEN_TEST_STREAM(char);
}

TEST(MSGPACK, stream_signed_char)
{
  GEN_TEST_STREAM(signed char);
}

TEST(MSGPACK, stream_unsigned_char)
{
  GEN_TEST_STREAM(unsigned char);
}

TEST(MSGPACK, stream_short)
{
  GEN_TEST_STREAM(short);
}

TEST(MSGPACK, stream_int)
{
  GEN_TEST_STREAM(int);
}

TEST(MSGPACK, stream_long)
{
  GEN_TEST_STREAM(long);
}

TEST(MSGPACK, stream_long_long)
{
  GEN_TEST_STREAM(long long);
}

TEST(MSGPACK, stream_unsigned_short)
{
  GEN_TEST_STREAM(unsigned short);
}

TEST(MSGPACK, stream_unsigned_int)
{
  GEN_TEST_STREAM(unsigned int);
}

TEST(MSGPACK, stream_unsigned_long)
{
  GEN_TEST_STREAM(unsigned long);
}

TEST(MSGPACK, stream_unsigned_long_long)
{
  GEN_TEST_STREAM(unsigned long long);
}

TEST(MSGPACK, stream_uint8)
{
  GEN_TEST_STREAM(uint8_t);
}

TEST(MSGPACK, stream_uint16)
{
  GEN_TEST_STREAM(uint16_t);
}

TEST(MSGPACK, stream_uint32)
{
  GEN_TEST_STREAM(uint32_t);
}

TEST(MSGPACK, stream_uint64)
{
  GEN_TEST_STREAM(uint64_t);
}

TEST(MSGPACK, stream_int8)
{
  GEN_TEST_STREAM(int8_t);
}

TEST(MSGPACK, stream_int16)
{
  GEN_TEST_STREAM(int16_t);
}

TEST(MSGPACK, stream_int32)
{
  GEN_TEST_STREAM(int32_t);
}

TEST(MSGPACK, stream_int64)
{
  GEN_TEST_STREAM(int64_t);
}
