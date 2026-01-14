//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2008-2016 FURUHASHI Sadayuki and KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MSGPACK_V1_OBJECT_FWD_DECL_HPP
#define MSGPACK_V1_OBJECT_FWD_DECL_HPP

#include "msgpack/versioning.hpp"
#include "msgpack/zone.hpp"
#include <stdint.h>
#include <typeinfo>

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond


namespace type {
    enum object_type {
        NIL                 = 0x00,
        BOOLEAN             = 0x01,
        POSITIVE_INTEGER    = 0x02,
        NEGATIVE_INTEGER    = 0x03,
        FLOAT32             = 0x0a,
        FLOAT64             = 0x04,
        FLOAT               = 0x04,
#if defined(MSGPACK_USE_LEGACY_NAME_AS_FLOAT)
        DOUBLE              = MSGPACK_DEPRECATED("please use FLOAT64 instead") FLOAT, // obsolete
#endif // MSGPACK_USE_LEGACY_NAME_AS_FLOAT
        STR                 = 0x05,
        BIN                 = 0x06,
        ARRAY               = 0x07,
        MAP                 = 0x08,
        EXT                 = 0x09
    };
}


struct object;
struct object_kv;

struct object_array;
struct object_map;
struct object_str;
struct object_bin;
struct object_ext;


#if !defined(MSGPACK_USE_CPP03)

namespace adaptor {
template <typename T, typename Enabler = void>
struct as;
} // namespace adaptor

template <typename T>
struct has_as;

#endif // !defined(MSGPACK_USE_CPP03)

class type_error;

/// @cond
} // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

} // namespace msgpack

#endif // MSGPACK_V1_OBJECT_FWD_DECL_HPP
