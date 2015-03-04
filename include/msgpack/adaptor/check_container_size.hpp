//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2015 KONDO Takatoshi
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
//
#ifndef MSGPACK_CHECK_CONTAINER_SIZE_HPP
#define MSGPACK_CHECK_CONTAINER_SIZE_HPP

#include "msgpack/versioning.hpp"
#include <stdexcept>

namespace msgpack {

MSGPACK_API_VERSION_NAMESPACE(v1) {

struct container_size_overflow : public std::runtime_error {
    explicit container_size_overflow(const std::string& msg)
        :std::runtime_error(msg) {}
#if !defined(MSGPACK_USE_CPP03)
    explicit container_size_overflow(const char* msg):
        std::runtime_error(msg) {}
#endif // !defined(MSGPACK_USE_CPP03)
};

namespace detail {

template <std::size_t N>
inline void check_container_size(std::size_t size) {
    if (size > 0xffffffff) throw container_size_overflow("container size overflow");
}

template <>
inline void check_container_size<4>(std::size_t size) {
}

} // namespace detail

template <typename T>
inline uint32_t checked_get_container_size(T size) {
    detail::check_container_size<sizeof(T)>(size);
    return static_cast<uint32_t>(size);
}


}  // MSGPACK_API_VERSION_NAMESPACE(v1)

}  // namespace msgpack

#endif // MSGPACK_CHECK_CONTAINER_SIZE_HPP
