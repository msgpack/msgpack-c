#include <msgpack.hpp>
#include <gtest/gtest.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

#if !defined(MSGPACK_USE_CPP03)
#include <iterator>

// C++11

constexpr unsigned int VECTOR_SIZE = 100;
constexpr unsigned int MAP_SIZE    = 100;

TEST(iterator, vector)
{
    using vec_type = vector<unsigned int>;
    vec_type vec;
    vec.reserve(VECTOR_SIZE);
    for (unsigned int i = 0; i < VECTOR_SIZE; i++) {
        vec.push_back(rand());
    }
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, vec);

    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());

    auto &msgarr = ret.get().via.array;
    auto dist = std::distance(begin(msgarr), end(msgarr));
    auto vecSize = vec.size();
    EXPECT_EQ(dist, vecSize);
    
    vec_type::iterator correct = std::begin(vec);
    for (auto i : msgarr) {
        auto u64 = *correct;
        EXPECT_EQ(i, u64);
        ++correct;
    }
}

TEST(iterator, map)
{
    using map_type = map<unsigned int, unsigned int>;
    map_type map;
    for (unsigned int i = 0; i < MAP_SIZE; i++) {
        map[rand()] = rand();
    }
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, map);

    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());

    auto &msgmap = ret.get().via.map;
    auto dist = std::distance(begin(msgmap), end(msgmap));
    auto mapSize = map.size();
    EXPECT_EQ(dist, mapSize);

    for (auto kv : msgmap) {
      auto val = kv.val.via.u64;
      auto correct = map[kv.key.via.u64];
      EXPECT_EQ(val, correct);
    }
}

#endif // !defined(MSGPACK_USE_CPP03
