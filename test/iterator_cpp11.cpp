#include <msgpack.hpp>

#define BOOST_TEST_MODULE iterator
#include <boost/test/unit_test.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if !defined(MSGPACK_USE_CPP03)
#include <iterator>

// C++11

constexpr unsigned int VECTOR_SIZE = 100;
constexpr unsigned int MAP_SIZE    = 100;

BOOST_AUTO_TEST_CASE(vector)
{
    using vec_type = std::vector<unsigned>;
    vec_type vec;
    vec.reserve(VECTOR_SIZE);
    for (unsigned int i = 0; i < VECTOR_SIZE; i++) {
        vec.push_back(static_cast<unsigned>(rand()));
    }
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, vec);

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());

    auto const& msgarr = oh.get().via.array;
    auto dist = std::distance(begin(msgarr), end(msgarr));
    auto vecSize = vec.size();
    BOOST_CHECK_EQUAL(static_cast<size_t>(dist), vecSize);

    vec_type::const_iterator correct = std::begin(vec);
    for (auto const& obj : msgarr) {
        auto u64 = *correct;
        BOOST_CHECK_EQUAL(obj.as<unsigned>(), u64);
        ++correct;
    }
}

BOOST_AUTO_TEST_CASE(map)
{
    using map_type = std::map<unsigned, unsigned>;
    map_type map;
    for (unsigned i = 0; i < MAP_SIZE; i++) {
        map[static_cast<unsigned>(rand())] = static_cast<unsigned>(rand());
    }
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, map);

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());

    auto const& msgmap = oh.get().via.map;
    auto dist = std::distance(begin(msgmap), end(msgmap));
    auto mapSize = map.size();
    BOOST_CHECK_EQUAL(static_cast<size_t>(dist), mapSize);

    for (auto const& kv : msgmap) {
        auto key = kv.key.as<unsigned>();
        auto val = kv.val.as<unsigned>();
        auto correct = map[key];
        BOOST_CHECK_EQUAL(val, correct);
    }
}

#endif // !defined(MSGPACK_USE_CPP03
