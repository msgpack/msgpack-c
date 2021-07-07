//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2021 KONDO Takatoshi and Daniil Kovalev
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MSGPACK_V1_TYPE_CPP20_SPAN_HPP
#define MSGPACK_V1_TYPE_CPP20_SPAN_HPP

#include "msgpack/cpp_version.hpp"

// Some compilers still do not set the corresponding macro to 202002
#if MSGPACK_CPP_VERSION > 201703

#include "msgpack/versioning.hpp"
#include "msgpack/adaptor/adaptor_base.hpp"
#include "msgpack/adaptor/check_container_size.hpp"
#include "msgpack/meta.hpp"

#include <span>
#include <cstring>
#include <cstddef>

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond

namespace adaptor {

#define MSGPACK_ADAPTOR_CONVERT_SPAN_BINARY(byte_t)                                                                 \
    template <>                                                                                                     \
    struct convert<std::span<const byte_t> > {                                                                      \
        msgpack::object const& operator()(msgpack::object const& o, std::span<const byte_t>& v) const {             \
            switch (o.type) {                                                                                       \
            case msgpack::type::BIN:                                                                                \
                v = std::span(reinterpret_cast<const byte_t*>(o.via.bin.ptr), o.via.bin.size);                      \
                break;                                                                                              \
            default:                                                                                                \
                throw msgpack::type_error();                                                                        \
                break;                                                                                              \
            }                                                                                                       \
            return o;                                                                                               \
        }                                                                                                           \
    };

#define MSGPACK_ADAPTOR_PACK_SPAN_BINARY(byte_t)                                                                    \
    template <>                                                                                                     \
    struct pack<std::span<byte_t> > {                                                                               \
        template <typename Stream>                                                                                  \
        msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, const std::span<byte_t>& v) const {         \
            uint32_t size = checked_get_container_size(v.size());                                                   \
            o.pack_bin(size);                                                                                       \
            if (size != 0) {                                                                                        \
                o.pack_bin_body(reinterpret_cast<char const*>(v.data()), size);                                     \
            }                                                                                                       \
            return o;                                                                                               \
        }                                                                                                           \
    };

#define MSGPACK_ADAPTOR_OBJECT_SPAN_BINARY(byte_t)                                                                  \
    template <>                                                                                                     \
    struct object<std::span<byte_t> > {                                                                             \
        void operator()(msgpack::object& o, const std::span<byte_t>& v) const {                                     \
            uint32_t size = checked_get_container_size(v.size());                                                   \
            o.type = msgpack::type::BIN;                                                                            \
            if (size != 0) {                                                                                        \
                o.via.bin.ptr = reinterpret_cast<char const*>(v.data());                                            \
            }                                                                                                       \
            o.via.bin.size = size;                                                                                  \
        }                                                                                                           \
    };

#define MSGPACK_ADAPTOR_OBJECT_WITH_ZONE_SPAN_BINARY(byte_t)                                                        \
    template <>                                                                                                     \
    struct object_with_zone<std::span<byte_t> > {                                                                   \
        void operator()(msgpack::object::with_zone& o, const std::span<byte_t>& v) const {                          \
            uint32_t size = checked_get_container_size(v.size());                                                   \
            o.type = msgpack::type::BIN;                                                                            \
            o.via.bin.size = size;                                                                                  \
            if (size != 0) {                                                                                        \
                char* ptr = static_cast<char*>(o.zone.allocate_align(size, MSGPACK_ZONE_ALIGNOF(char)));            \
                o.via.bin.ptr = ptr;                                                                                \
                std::memcpy(ptr, v.data(), size);                                                                   \
            }                                                                                                       \
        }                                                                                                           \
    };

#define MSGPACK_ADAPTOR_SPAN_BINARY(byte_t)                                                                         \
    MSGPACK_ADAPTOR_CONVERT_SPAN_BINARY(byte_t)                                                                     \
    MSGPACK_ADAPTOR_PACK_SPAN_BINARY(byte_t)                                                                        \
    MSGPACK_ADAPTOR_PACK_SPAN_BINARY(const byte_t)                                                                  \
    MSGPACK_ADAPTOR_OBJECT_SPAN_BINARY(byte_t)                                                                      \
    MSGPACK_ADAPTOR_OBJECT_SPAN_BINARY(const byte_t)                                                                \
    MSGPACK_ADAPTOR_OBJECT_WITH_ZONE_SPAN_BINARY(byte_t)                                                            \
    MSGPACK_ADAPTOR_OBJECT_WITH_ZONE_SPAN_BINARY(const byte_t)

MSGPACK_ADAPTOR_SPAN_BINARY(char)
MSGPACK_ADAPTOR_SPAN_BINARY(unsigned char)
MSGPACK_ADAPTOR_SPAN_BINARY(std::byte)

#undef MSGPACK_ADAPTOR_SPAN_BINARY
#undef MSGPACK_ADAPTOR_OBJECT_WITH_ZONE_SPAN_BINARY
#undef MSGPACK_ADAPTOR_OBJECT_SPAN_BINARY
#undef MSGPACK_ADAPTOR_PACK_SPAN_BINARY
#undef MSGPACK_ADAPTOR_CONVERT_SPAN_BINARY

} // namespace adaptor

/// @cond
} // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

} // namespace msgpack

#endif // MSGPACK_CPP_VERSION > 201703

#endif // MSGPACK_V1_TYPE_CPP11_ARRAY_HPP
