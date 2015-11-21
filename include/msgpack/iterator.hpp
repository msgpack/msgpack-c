//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2015 MIZUKI Hirata
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MSGPACK_ITERATOR_HPP
#define MSGPACK_ITERATOR_HPP
#if !defined(MSGPACK_USE_CPP03)

#include <msgpack/object_fwd.hpp>

namespace msgpack
{
    /// @cond
    MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
    {
    /// @endcond
        inline object_kv* begin(object_map &map) { return map.ptr; }
        inline const object_kv* begin(const object_map &map) { return map.ptr; }
        inline object_kv* end(object_map &map) { return map.ptr + map.size; }
        inline const object_kv* end(const object_map &map) { return map.ptr + map.size; }

        inline object* begin(object_array &array) { return array.ptr; }
        inline const object* begin(const object_array &array) { return array.ptr; }
        inline object* end(object_array &array) { return array.ptr + array.size; }
        inline const object* end(const object_array &array) { return array.ptr + array.size; }
    /// @cond
    }
    /// @endcond
}

#endif // !defined(MSGPACK_USE_CPP03)
#endif // MSGPACK_ITERATOR_HPP
