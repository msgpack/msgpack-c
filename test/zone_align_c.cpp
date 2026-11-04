/* msgpack_zone_malloc() is inline, so its alignment is fixed per translation
 * unit. Use one stricter than the zone chunk header, as CMakeLists.txt does
 * on sparc (MSGPACK_ZONE_ALIGN=8 with a 4-byte header), so that an object
 * served from a newly expanded chunk needs padding. */
#undef MSGPACK_ZONE_ALIGN
#define MSGPACK_ZONE_ALIGN (2 * sizeof(void*))

#include <msgpack/zone.h>

#include <gtest/gtest.h>

#include <string.h>

TEST(zone, zone_c_malloc_expand_align)
{
    msgpack_zone z;
    const size_t size = 4096;
    EXPECT_TRUE(msgpack_zone_init(&z, 2048));
    /* larger than the chunk size: served by msgpack_zone_malloc_expand() */
    char* p = (char*)msgpack_zone_malloc(&z, size);
    ASSERT_TRUE(p != NULL);
    EXPECT_TRUE((uintptr_t)p % MSGPACK_ZONE_ALIGN == 0);
    /* the object has to stay inside the size + MSGPACK_ZONE_ALIGN - 1 bytes
     * reserved by the expansion, not overlap the new chunk's header */
    ASSERT_TRUE(p >= z.chunk_list.ptr - (size + MSGPACK_ZONE_ALIGN - 1));
    ASSERT_TRUE(p + size <= z.chunk_list.ptr);
    memset(p, 0xff, size);
    msgpack_zone_destroy(&z);
}
