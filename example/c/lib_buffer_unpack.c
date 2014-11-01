#include <msgpack.h>
#include <stdio.h>
#include <assert.h>


typedef struct receiver {
    msgpack_sbuffer sbuf;
    size_t rest;
} receiver;

receiver r;

void receiver_init(receiver *r) {
    msgpack_packer pk;

    msgpack_sbuffer_init(&r->sbuf);
    msgpack_packer_init(&pk, &r->sbuf, msgpack_sbuffer_write);
    /* 1st object */
    msgpack_pack_array(&pk, 3);
    msgpack_pack_int(&pk, 1);
    msgpack_pack_true(&pk);
    msgpack_pack_str(&pk, 7);
    msgpack_pack_str_body(&pk, "example", 7);
    /* 2nd object */
    msgpack_pack_str(&pk, 6);
    msgpack_pack_str_body(&pk, "second", 6);
    /* 3rd object */
    msgpack_pack_array(&pk, 2);
    msgpack_pack_int(&pk, 42);
    msgpack_pack_false(&pk);
    r->rest = r->sbuf.size;
}

size_t receiver_recv(receiver *r, char* buf, size_t try_size) {
    size_t off = r->sbuf.size - r->rest;

    size_t actual_size = try_size;
    if (actual_size > r->rest) actual_size = r->rest;

    memcpy(buf, r->sbuf.data + off, actual_size);
    r->rest -= actual_size;

    return actual_size;
}

#define EACH_RECV_SIZE 4

void unpack(receiver* r) {
    /* buf is allocated by unpacker. */
    msgpack_unpacker* unp = msgpack_unpacker_new(100);
    msgpack_unpacked result;
    msgpack_unpack_return ret;
    char* buf;
    size_t recv_len;

    msgpack_unpacked_init(&result);
    if (msgpack_unpacker_buffer_capacity(unp) < EACH_RECV_SIZE) {
        bool expanded = msgpack_unpacker_reserve_buffer(unp, 100);
        assert(expanded);
    }
    buf = msgpack_unpacker_buffer(unp);

    recv_len = receiver_recv(r, buf, EACH_RECV_SIZE);
    msgpack_unpacker_buffer_consumed(unp, recv_len);


    int recv_count = 0;
    while (recv_len > 0) {
        int i = 0;
        printf("receive count: %d %zd bytes received.:\n", recv_count++, recv_len);
        ret = msgpack_unpacker_next(unp, &result);
        while (ret == MSGPACK_UNPACK_SUCCESS) {
            msgpack_object obj = result.data;

            /* Use obj. */
            printf("Object no %d:\n", i++);
            msgpack_object_print(stdout, obj);
            printf("\n");
            /* If you want to allocate something on the zone, you can use zone. */
            /* msgpack_zone* zone = result.zone; */
            /* The lifetime of the obj and the zone,  */

            ret = msgpack_unpacker_next(unp, &result);
        }
        if (ret == MSGPACK_UNPACK_PARSE_ERROR) {
            printf("The data in the buf is invalid format.\n");
            msgpack_unpacked_destroy(&result);
            return;
        }
        if (msgpack_unpacker_buffer_capacity(unp) < EACH_RECV_SIZE) {
            bool expanded = msgpack_unpacker_reserve_buffer(unp, 100);
            assert(expanded);
        }
        buf = msgpack_unpacker_buffer(unp);
        recv_len = receiver_recv(r, buf, 4);
        msgpack_unpacker_buffer_consumed(unp, recv_len);
    }
    msgpack_unpacked_destroy(&result);
}

int main(void) {
    receiver r;
    receiver_init(&r);

    unpack(&r);

    return 0;
}

/* Output */

/*
Object no 1:
[1, true, "example"]
Object no 2:
"second"
Object no 3:
[42, false]
*/
