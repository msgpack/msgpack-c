//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2015 KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MSGPACK_META_HPP
#define MSGPACK_META_HPP

#if !defined(MSGPACK_USE_CPP03)

#include <type_traits>

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond

namespace detail {
template<bool...> struct bool_pack;

template<bool...values> struct all_of_imp
    : std::is_same<bool_pack<values..., true>, bool_pack<true, values...>>{};

} // namespace detail

template<template <class> class T, class... U>
using all_of = detail::all_of_imp<T<U>::value...>;

template<std::size_t... Is> struct seq {};

template<std::size_t N, std::size_t... Is>
struct gen_seq : gen_seq<N-1, N-1, Is...> {};

template<std::size_t... Is>
struct gen_seq<0, Is...> : seq<Is...> {};

/// @cond
} // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

} // namespace msgpack

#endif // !defined(MSGPACK_USE_CPP03)

#endif // MSGPACK_META_HPP
