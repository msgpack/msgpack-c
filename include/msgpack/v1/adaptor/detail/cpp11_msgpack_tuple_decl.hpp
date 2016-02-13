//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2008-2015 FURUHASHI Sadayuki and KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_V1_CPP11_MSGPACK_TUPLE_DECL_HPP
#define MSGPACK_V1_CPP11_MSGPACK_TUPLE_DECL_HPP

#include "msgpack/versioning.hpp"
#include "msgpack/object_fwd.hpp"
#include "msgpack/meta.hpp"

#include <tuple>

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond

namespace type {
    // tuple
    using std::get;
    using std::tuple_size;
    using std::tuple_element;
    using std::uses_allocator;
    using std::ignore;
    using std::swap;

    template< class... Types >
    class tuple;

    template <class... Args>
    tuple<Args...> make_tuple(Args&&... args);

    template<class... Args>
    tuple<Args&&...> forward_as_tuple (Args&&... args) noexcept;

    template <class... Tuples>
    auto tuple_cat(Tuples&&... args) ->
        decltype(
            std::tuple_cat(std::forward<typename std::remove_reference<Tuples>::type::base>(args)...)
        );

    template <class... Args>
    tuple<Args&...> tie(Args&... args);

} // namespace type

// --- Pack from tuple to packer stream ---
template <typename Stream, typename Tuple, std::size_t N>
struct MsgpackTuplePacker;

// --- Convert from tuple to object ---
template <typename... Args>
struct MsgpackTupleAs;

template <typename T, typename... Args>
struct MsgpackTupleAsImpl;

template <typename Tuple, std::size_t N>
struct MsgpackTupleConverter;

// --- Convert from tuple to object with zone ---
template <typename Tuple, std::size_t N>
struct MsgpackTupleToObjectWithZone;

/// @cond
}  // MSGPACK_API_VERSION_NAMESPACE(v1)
///@endcond

}  // namespace msgpack

#endif // MSGPACK_V1_CPP11_MSGPACK_TUPLE_DECL_HPP
