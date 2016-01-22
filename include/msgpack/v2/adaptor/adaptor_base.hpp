//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2015-2016 KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_V2_ADAPTOR_BASE_HPP
#define MSGPACK_V2_ADAPTOR_BASE_HPP

#include "msgpack/v2/adaptor/adaptor_base_decl.hpp"

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v2) {
/// @endcond


namespace adaptor {

// Adaptor functors

template <typename T, typename Enabler>
struct convert : v1::adaptor::convert<T, Enabler> {
};

template <typename T, typename Enabler>
struct pack : v1::adaptor::pack<T, Enabler> {
};

template <typename T, typename Enabler>
struct object : v1::adaptor::object<T, Enabler> {
};

template <typename T, typename Enabler>
struct object_with_zone : v1::adaptor::object_with_zone<T, Enabler> {
};

} // namespace adaptor

// operators

template <typename T>
inline
msgpack::object const& operator>> (msgpack::object const& o, T& v) {
    return adaptor::convert<T>()(o, v);
}

template <typename Stream, typename T>
inline
msgpack::packer<Stream>& operator<< (msgpack::packer<Stream>& o, T const& v) {
    return adaptor::pack<T>()(o, v);
}

template <typename T>
inline
void operator<< (msgpack::object& o, T const& v) {
    adaptor::object<T>()(o, v);
}

template <typename T>
inline
void operator<< (msgpack::object::with_zone& o, T const& v) {
    adaptor::object_with_zone<T>()(o, v);
}

/// @cond
} // MSGPACK_API_VERSION_NAMESPACE(v2)
/// @endcond

} // namespace msgpack


#endif // MSGPACK_V2_ADAPTOR_BASE_HPP
