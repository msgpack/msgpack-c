//
// MessagePack for C++ deserializing routine
//
// Copyright (C) 2016 KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_V2_UNPACK_DECL_HPP
#define MSGPACK_V2_UNPACK_DECL_HPP

#include "msgpack/v1/unpack_decl.hpp"

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v2) {
/// @endcond

using v1::unpack_reference_func;

using v1::unpack_error;
using v1::parse_error;
using v1::insufficient_bytes;
using v1::size_overflow;
using v1::array_size_overflow;
using v1::map_size_overflow;
using v1::str_size_overflow;
using v1::bin_size_overflow;
using v1::ext_size_overflow;
using v1::depth_size_overflow;
using v1::unpack_limit;

namespace detail {

using v1::detail::unpack_user;

using v1::detail::unpack_uint8;
using v1::detail::unpack_uint16;
using v1::detail::unpack_uint32;
using v1::detail::unpack_uint64;

using v1::detail::unpack_int8;
using v1::detail::unpack_int16;
using v1::detail::unpack_int32;
using v1::detail::unpack_int64;

using v1::detail::unpack_float;
using v1::detail::unpack_double;

using v1::detail::unpack_nil;

using v1::detail::unpack_true;
using v1::detail::unpack_false;

using v1::detail::unpack_array;
using v1::detail::unpack_array_item;
using v1::detail::unpack_map;
using v1::detail::unpack_map_item;
using v1::detail::unpack_str;
using v1::detail::unpack_bin;
using v1::detail::unpack_ext;

using  v1::detail::unpack_stack;

using v1::detail::init_count;
using v1::detail::decr_count;
using v1::detail::incr_count;

using v1::detail::get_count;

using v1::detail::fix_tag;

using v1::detail::value;

using v1::detail::load;

using v1::detail::context;

} // detail


using v1::unpacked;

using v1::unpacker;

using v1::unpack;

using v1::unpack_return;

namespace detail {

using v1::detail::unpack_imp;

} // detail

/// @cond
}  // MSGPACK_API_VERSION_NAMESPACE(v2)
/// @endcond

}  // namespace msgpack

#endif // MSGPACK_V2_UNPACK_DECL_HPP
