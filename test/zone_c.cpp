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
    /* size + the alignment padding would wrap around: the request has to
     * be rejected without any allocation attempt. */
    EXPECT_TRUE(msgpack_zone_malloc(&z, (size_t)-1 - 2) == NULL);
    /* sizeof(chunk) + size would wrap around in the expansion: the request
     * has to be rejected without any allocation attempt. */
    EXPECT_TRUE(msgpack_zone_malloc_no_align(&z, (size_t)-1) == NULL);
    EXPECT_TRUE(msgpack_zone_malloc_expand(&z, (size_t)-1) == NULL);
    msgpack_zone_destroy(&z);
}

TEST(zone, zone_c_chunk_size_overflow)
{
    msgpack_zone z;
    /* sizeof(chunk) + chunk_size would wrap around in the initial chunk
     * allocation: the request has to be rejected without any allocation
     * attempt. The chunk header is one pointer, so (size_t)-1 -
     * (sizeof(void*) - 1) is the smallest chunk size that wraps. */
    EXPECT_FALSE(msgpack_zone_init(&z, (size_t)-1));
    EXPECT_FALSE(msgpack_zone_init(&z, (size_t)-1 - (sizeof(void*) - 1)));
    EXPECT_TRUE(msgpack_zone_new((size_t)-1) == NULL);
    EXPECT_TRUE(msgpack_zone_new((size_t)-1 - (sizeof(void*) - 1)) == NULL);

    /* A zone with the default chunk size is still usable. */
    EXPECT_TRUE(msgpack_zone_init(&z, MSGPACK_ZONE_CHUNK_SIZE));
    EXPECT_TRUE(msgpack_zone_malloc(&z, 100) != NULL);
    msgpack_zone_destroy(&z);
}
