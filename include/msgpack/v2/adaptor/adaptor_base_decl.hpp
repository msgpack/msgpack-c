//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2016 KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_V2_ADAPTOR_BASE_DECL_HPP
#define MSGPACK_V2_ADAPTOR_BASE_DECL_HPP

#include "msgpack/v1/adaptor/adaptor_base_decl.hpp"

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v2) {
/// @endcond

using v1::packer;

namespace adaptor {

// Adaptor functors

template <typename T, typename Enabler = void>
struct convert;

template <typename T, typename Enabler = void>
struct pack;

template <typename T, typename Enabler = void>
struct object;

template <typename T, typename Enabler = void>
struct object_with_zone;

} // namespace adaptor

// operators

template <typename T>
 msgpack::object const& operator>> (msgpack::object const& o, T& v);

template <typename Stream, typename T>
msgpack::packer<Stream>& operator<< (msgpack::packer<Stream>& o, T const& v);

template <typename T>
void operator<< (msgpack::object& o, T const& v);

template <typename T>
void operator<< (msgpack::object::with_zone& o, T const& v);

/// @cond
} // MSGPACK_API_VERSION_NAMESPACE(v2)
/// @endcond

} // namespace msgpack

#endif // MSGPACK_V2_ADAPTOR_BASE_DECL_HPP
