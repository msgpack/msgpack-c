#include <stdio.h>

/* You can define your own allocator here */

#define MSGPACK_ALLOC_SIZE_MAX  1024*1024
#define MSGPACK_ALLOC_COUNT_MAX 1024

static char g_buf[MSGPACK_ALLOC_COUNT_MAX][MSGPACK_ALLOC_SIZE_MAX];
static size_t g_index = 0;

void* my_malloc(size_t size) {
    if (g_index == MSGPACK_ALLOC_COUNT_MAX) return NULL;
    if (size > MSGPACK_ALLOC_SIZE_MAX) return NULL;
    return &g_buf[g_index++][0];
}

void* my_realloc(void* p, size_t size) {
    if (p == NULL) return my_malloc(size);
    if (size > MSGPACK_ALLOC_SIZE_MAX) return NULL;
    return p;
}

void my_free(void* p) {
    (void)p;
}

/* Define the following three macros before include msgpack.h */

#define MSGPACK_MALLOC(size) my_malloc(size)
#define MSGPACK_REALLOC(p, size) my_realloc(p, size)
#define MSGPACK_FREE(p) my_free(p)

#include <msgpack.h>

void print(char const* buf, unsigned int len)
{
    size_t i = 0;
    for(; i < len ; ++i)
        printf("%02x ", 0xff & buf[i]);
    printf("\n");
}

int main(void)
{
    msgpack_sbuffer sbuf;
    msgpack_packer pk;
    msgpack_zone mempool;
    msgpack_object deserialized;

    /* msgpack::sbuffer is a simple buffer implementation. */
    msgpack_sbuffer_init(&sbuf);

    /* serialize values into the buffer using msgpack_sbuffer_write callback function. */
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    msgpack_pack_array(&pk, 3);
    msgpack_pack_int(&pk, 1);
    msgpack_pack_true(&pk);
    msgpack_pack_str(&pk, 7);
    msgpack_pack_str_body(&pk, "example", 7);

    print(sbuf.data, sbuf.size);

    /* deserialize the buffer into msgpack_object instance. */
    /* deserialized object is valid during the msgpack_zone instance alive. */
    msgpack_zone_init(&mempool, 2048);

    msgpack_unpack(sbuf.data, sbuf.size, NULL, &mempool, &deserialized);

    /* print the deserialized object. */
    msgpack_object_print(stdout, deserialized);
    puts("");

    msgpack_zone_destroy(&mempool);
    msgpack_sbuffer_destroy(&sbuf);

    return 0;
}
