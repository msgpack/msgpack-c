//
// MessagePack for C++ memory pool
//
// Copyright (C) 2008-2014 FURUHASHI Sadayuki and KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_ZONE_HPP
#define MSGPACK_ZONE_HPP

#include "msgpack/cpp_config.hpp"

#if defined(MSGPACK_USE_CPP03)
#include "detail/cpp03_zone.hpp"
#else  // MSGPACK_USE_CPP03
#include "detail/cpp11_zone.hpp"
#endif // MSGPACK_USE_CPP03

#endif // MSGPACK_ZONE_HPP
