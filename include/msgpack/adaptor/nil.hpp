//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2008-2009 FURUHASHI Sadayuki
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_TYPE_NIL_HPP
#define MSGPACK_TYPE_NIL_HPP

#include "msgpack/versioning.hpp"
#include "msgpack/adaptor/adaptor_base.hpp"

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond

namespace type {

struct nil { };

inline bool operator<(nil const& lhs, nil const& rhs) {
    return &lhs < &rhs;
}

inline bool operator==(nil const& lhs, nil const& rhs) {
    return &lhs == &rhs;
}

}  // namespace type

namespace adaptor {

template <>
struct convert<type::nil> {
    msgpack::object const& operator()(msgpack::object const& o, type::nil&) const {
        if(o.type != msgpack::type::NIL) { throw msgpack::type_error(); }
        return o;
    }
};

template <>
struct pack<type::nil> {
    template <typename Stream>
    msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, const type::nil&) const {
        o.pack_nil();
        return o;
    }
};

template <>
struct object<type::nil> {
    void operator()(msgpack::object& o, type::nil) const {
        o.type = msgpack::type::NIL;
    }
};

template <>
struct object_with_zone<type::nil> {
    void operator()(msgpack::object::with_zone& o, type::nil v) const {
        static_cast<msgpack::object&>(o) << v;
    }
};

} // namespace adaptor

template <>
inline void msgpack::object::as<void>() const
{
    msgpack::type::nil v;
    convert(v);
}

/// @cond
}  // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

}  // namespace msgpack

#endif // MSGPACK_TYPE_NIL_HPP
