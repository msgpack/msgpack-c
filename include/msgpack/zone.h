/*
 * MessagePack for C memory pool implementation
 *
 * Copyright (C) 2008-2010 FURUHASHI Sadayuki
 *
 *    Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *    http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef MSGPACK_ZONE_H
#define MSGPACK_ZONE_H

#include "sysdep.h"

/* AddressSanitizer interior redzones.
 *
 * The zone carves many objects out of one malloc()ed chunk, so an overflow
 * from one zone-allocated object into its neighbour stays inside a live heap
 * block and is invisible to ASan. When building with ASan, every allocation
 * is followed by a poisoned redzone, and chunks of the configured size are
 * kept poisoned until handed out, so that msgpack_zone_clear() also poisons
 * stale objects. Builds without ASan are not affected.
 * Define MSGPACK_ZONE_NO_ASAN_ANNOTATIONS to disable this, e.g. when
 * instrumented and uninstrumented code share a zone.
 */
#if !defined(MSGPACK_ZONE_NO_ASAN_ANNOTATIONS)
#if defined(__SANITIZE_ADDRESS__)
#define MSGPACK_ZONE_ASAN_ANNOTATIONS
#elif defined(__has_feature)
#if __has_feature(address_sanitizer)
#define MSGPACK_ZONE_ASAN_ANNOTATIONS
#endif
#endif
#endif

#if defined(MSGPACK_ZONE_ASAN_ANNOTATIONS)
#include <sanitizer/asan_interface.h>
/* ASan shadow granularity: objects start on a granule boundary. */
#define MSGPACK_ZONE_ASAN_GRANULE 8
/* Minimum number of poisoned bytes between two zone allocations. */
#ifndef MSGPACK_ZONE_ASAN_REDZONE_SIZE
#define MSGPACK_ZONE_ASAN_REDZONE_SIZE 16
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup msgpack_zone Memory zone
 * @ingroup msgpack
 * @{
 */

typedef struct msgpack_zone_finalizer {
    void (*func)(void* data);
    void* data;
} msgpack_zone_finalizer;

typedef struct msgpack_zone_finalizer_array {
    msgpack_zone_finalizer* tail;
    msgpack_zone_finalizer* end;
    msgpack_zone_finalizer* array;
} msgpack_zone_finalizer_array;

struct msgpack_zone_chunk;
typedef struct msgpack_zone_chunk msgpack_zone_chunk;

typedef struct msgpack_zone_chunk_list {
    size_t free;
    char* ptr;
    msgpack_zone_chunk* head;
} msgpack_zone_chunk_list;

typedef struct msgpack_zone {
    msgpack_zone_chunk_list chunk_list;
    msgpack_zone_finalizer_array finalizer_array;
    size_t chunk_size;
} msgpack_zone;

#ifndef MSGPACK_ZONE_CHUNK_SIZE
#define MSGPACK_ZONE_CHUNK_SIZE 8192
#endif

MSGPACK_DLLEXPORT
bool msgpack_zone_init(msgpack_zone* zone, size_t chunk_size);
MSGPACK_DLLEXPORT
void msgpack_zone_destroy(msgpack_zone* zone);

MSGPACK_DLLEXPORT
msgpack_zone* msgpack_zone_new(size_t chunk_size);
MSGPACK_DLLEXPORT
void msgpack_zone_free(msgpack_zone* zone);

static inline void* msgpack_zone_malloc(msgpack_zone* zone, size_t size);
static inline void* msgpack_zone_malloc_no_align(msgpack_zone* zone, size_t size);

static inline bool msgpack_zone_push_finalizer(msgpack_zone* zone,
        void (*func)(void* data), void* data);

static inline void msgpack_zone_swap(msgpack_zone* a, msgpack_zone* b);

MSGPACK_DLLEXPORT
bool msgpack_zone_is_empty(msgpack_zone* zone);

MSGPACK_DLLEXPORT
void msgpack_zone_clear(msgpack_zone* zone);

/** @} */


#ifndef MSGPACK_ZONE_ALIGN
#define MSGPACK_ZONE_ALIGN sizeof(void*)
#endif

MSGPACK_DLLEXPORT
void* msgpack_zone_malloc_expand(msgpack_zone* zone, size_t size);

#if defined(MSGPACK_ZONE_ASAN_ANNOTATIONS)
/* Same as msgpack_zone_malloc(), but each object starts on an ASan granule
 * and is followed by MSGPACK_ZONE_ASAN_REDZONE_SIZE bytes that stay
 * poisoned, so writing past the object is reported as use-after-poison. */
static inline void* msgpack_zone_asan_malloc(msgpack_zone* zone, size_t size, size_t align)
{
    msgpack_zone_chunk_list* const cl = &zone->chunk_list;
    const size_t redzone = MSGPACK_ZONE_ASAN_REDZONE_SIZE;
    char* aligned;
    size_t adjusted_size;

    if (align < MSGPACK_ZONE_ASAN_GRANULE) {
        align = MSGPACK_ZONE_ASAN_GRANULE;
    }
    if (size > (size_t)-1 - (align - 1) - redzone) {
        return NULL;
    }
    aligned = (char*)(
        ((uintptr_t)cl->ptr + (align - 1)) & ~(uintptr_t)(align - 1));
    adjusted_size = size + redzone + (size_t)(aligned - cl->ptr);
    if (cl->free >= adjusted_size) {
        cl->free -= adjusted_size;
        cl->ptr  += adjusted_size;
        ASAN_UNPOISON_MEMORY_REGION(aligned, size);
    } else {
        /* A new chunk is unpoisoned where it is handed out. */
        char* ptr = (char*)msgpack_zone_malloc_expand(zone, size + redzone + (align - 1));
        if (ptr == NULL) {
            return NULL;
        }
        aligned = (char*)(
            ((uintptr_t)ptr + (align - 1)) & ~(uintptr_t)(align - 1));
        ASAN_POISON_MEMORY_REGION(ptr, (size_t)(aligned - ptr));
    }
    ASAN_POISON_MEMORY_REGION(aligned + size, redzone);
    return aligned;
}
#endif

static inline void* msgpack_zone_malloc_no_align(msgpack_zone* zone, size_t size)
{
#if defined(MSGPACK_ZONE_ASAN_ANNOTATIONS)
    return msgpack_zone_asan_malloc(zone, size, 1);
#else
    char* ptr;
    msgpack_zone_chunk_list* cl = &zone->chunk_list;

    if(zone->chunk_list.free < size) {
        return msgpack_zone_malloc_expand(zone, size);
    }

    ptr = cl->ptr;
    cl->free -= size;
    cl->ptr  += size;

    return ptr;
#endif
}

static inline void* msgpack_zone_malloc(msgpack_zone* zone, size_t size)
{
#if defined(MSGPACK_ZONE_ASAN_ANNOTATIONS)
    return msgpack_zone_asan_malloc(zone, size, MSGPACK_ZONE_ALIGN);
#else
    char* aligned =
        (char*)(
            (uintptr_t)(
                zone->chunk_list.ptr + (MSGPACK_ZONE_ALIGN - 1)
            ) & ~(uintptr_t)(MSGPACK_ZONE_ALIGN - 1)
        );
    size_t adjusted_size = size + (size_t)(aligned - zone->chunk_list.ptr);

    if (size + (MSGPACK_ZONE_ALIGN - 1) < size) {
        /* size + the alignment padding would wrap around */
        return NULL;
    }
    if(zone->chunk_list.free >= adjusted_size) {
        zone->chunk_list.free -= adjusted_size;
        zone->chunk_list.ptr  += adjusted_size;
        return aligned;
    }
    {
        void* ptr = msgpack_zone_malloc_expand(zone, size + (MSGPACK_ZONE_ALIGN - 1));
        if (ptr) {
            return (char*)((uintptr_t)(ptr) & ~(uintptr_t)(MSGPACK_ZONE_ALIGN - 1));
        }
    }
    return NULL;
#endif
}


bool msgpack_zone_push_finalizer_expand(msgpack_zone* zone,
        void (*func)(void* data), void* data);

static inline bool msgpack_zone_push_finalizer(msgpack_zone* zone,
        void (*func)(void* data), void* data)
{
    msgpack_zone_finalizer_array* const fa = &zone->finalizer_array;
    msgpack_zone_finalizer* fin = fa->tail;

    if(fin == fa->end) {
        return msgpack_zone_push_finalizer_expand(zone, func, data);
    }

    fin->func = func;
    fin->data = data;

    ++fa->tail;

    return true;
}

static inline void msgpack_zone_swap(msgpack_zone* a, msgpack_zone* b)
{
    msgpack_zone tmp = *a;
    *a = *b;
    *b = tmp;
}


#ifdef __cplusplus
}
#endif

#endif /* msgpack/zone.h */
