#include <msgpack.hpp>
#include <sstream>
#include <iterator>
#include <gtest/gtest.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if defined(MSGPACK_USE_BOOST)

#include <boost/fusion/adapted/struct/define_struct.hpp>

BOOST_FUSION_DEFINE_STRUCT(
    BOOST_PP_EMPTY(),
    mystruct,
    (int, f1)
    (double, f2)
    )

TEST(MSGPACK_BOOST, fusion_pack_unpack_convert)
{
    std::stringstream ss;
    mystruct val1;
    val1.f1 = 42;
    val1.f2 = 123.45;
    msgpack::pack(ss, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    mystruct val2 = ret.get().as<mystruct>();
    EXPECT_TRUE(val1.f1 == val2.f1);
    EXPECT_TRUE(val1.f2 == val2.f2);
}

TEST(MSGPACK_BOOST, object_with_zone_convert)
{
    mystruct val1;
    val1.f1 = 42;
    val1.f2 = 123.45;
    msgpack::zone z;
    msgpack::object obj(val1, z);
    mystruct val2 = obj.as<mystruct>();
    EXPECT_TRUE(val1.f1 == val2.f1);
    EXPECT_TRUE(val1.f2 == val2.f2);
}

#endif // defined(MSGPACK_USE_BOOST)
