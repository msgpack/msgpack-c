//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2008-2014 FURUHASHI Sadayuki and KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_DEFINE_HPP
#define MSGPACK_DEFINE_HPP

#include "msgpack/cpp_config.hpp"

#if defined(MSGPACK_USE_CPP03)
#include "detail/cpp03_define_array.hpp"
#include "detail/cpp03_define_map.hpp"
#else  // MSGPACK_USE_CPP03
#include "detail/cpp11_define_array.hpp"
#include "detail/cpp11_define_map.hpp"
#endif // MSGPACK_USE_CPP03

#if defined(MSGPACK_USE_DEFINE_MAP)
#define MSGPACK_DEFINE MSGPACK_DEFINE_MAP
#define MSGPACK_BASE MSGPACK_BASE_MAP
#else  // defined(MSGPACK_USE_DEFINE_MAP)
#define MSGPACK_DEFINE MSGPACK_DEFINE_ARRAY
#define MSGPACK_BASE MSGPACK_BASE_ARRAY
#endif // defined(MSGPACK_USE_DEFINE_MAP)

#endif // MSGPACK_DEFINE_HPP
