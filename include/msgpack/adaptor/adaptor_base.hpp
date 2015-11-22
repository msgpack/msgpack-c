//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2015 KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_ADAPTOR_BASE_HPP
#define MSGPACK_ADAPTOR_BASE_HPP

#include "msgpack/object_fwd.hpp"

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond

template <typename Stream>
class packer;

namespace adaptor {

// Adaptor functors

template <typename T, typename Enabler = void>
struct convert {
    msgpack::object const& operator()(msgpack::object const& o, T& v) const;
};

template <typename T, typename Enabler = void>
struct pack {
    template <typename Stream>
    msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, T const& v) const;
};

template <typename T, typename Enabler = void>
struct object {
    void operator()(msgpack::object& o, T const& v) const;
};

template <typename T, typename Enabler = void>
struct object_with_zone {
    void operator()(msgpack::object::with_zone& o, T const& v) const;
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
} // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

} // namespace msgpack


#endif // MSGPACK_ADAPTOR_BASE_HPP
