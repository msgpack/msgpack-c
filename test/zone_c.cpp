#include <msgpack/zone.h>

#include <gtest/gtest.h>

TEST(zone, zone_c_malloc)
{
    msgpack_zone z;
    EXPECT_TRUE(msgpack_zone_init(&z, 2048));
    void* p = msgpack_zone_malloc(&z, 100);
    EXPECT_TRUE(p != NULL);
    void* q = msgpack_zone_malloc(&z, 100);
    EXPECT_TRUE(q != NULL);
    msgpack_zone_destroy(&z);
}

TEST(zone, zone_c_size_overflow)
{
    msgpack_zone z;
    EXPECT_TRUE(msgpack_zone_init(&z, 2048));
    /* sizeof(chunk) + size must not wrap around: the allocation has to fail */
    EXPECT_TRUE(msgpack_zone_malloc(&z, (size_t)-1 - 8) == NULL);
    msgpack_zone_destroy(&z);
}
