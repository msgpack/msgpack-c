/*
 * MessagePack for C utilities
 *
 * Copyright (C) 2019 KONDO Takatoshi
 *
 *    Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *    http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef MSGPACK_ALLOCATOR_H
#define MSGPACK_ALLOCATOR_H

#if !defined(MSGPACK_MALLOC)
#include <stdlib.h>
#define MSGPACK_MALLOC(size) malloc(size)
#endif // !defined(MSGPACK_MALLOC)

#if !defined(MSGPACK_REALLOC)
#include <stdlib.h>
#define MSGPACK_REALLOC(p, size) realloc(p, size)
#endif // !defined(MSGPACK_MALLOC)

#if !defined(MSGPACK_FREE)
#include <stdlib.h>
#define MSGPACK_FREE(p) free(p)
#endif // !defined(MSGPACK_MALLOC)

#endif /* MSGPACK_ALLOCATOR_H */
