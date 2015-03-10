//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2014-2015 KONDO Takatoshi
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

#ifndef MSGPACK_CPP11_ARRAY_HPP
#define MSGPACK_CPP11_ARRAY_HPP

#include "msgpack/versioning.hpp"
#include "msgpack/object_fwd.hpp"
#include "msgpack/adaptor/check_container_size.hpp"

#include <array>

namespace msgpack {

MSGPACK_API_VERSION_NAMESPACE(v1) {

template <typename T, std::size_t N>
inline msgpack::object const& operator>> (msgpack::object const& o, std::array<T, N>& v) {
    if(o.type != msgpack::type::ARRAY) { throw msgpack::type_error(); }
    if(o.via.array.size != N) { throw msgpack::type_error(); }
    if(o.via.array.size > 0) {
        msgpack::object* p = o.via.array.ptr;
        msgpack::object* const pend = o.via.array.ptr + o.via.array.size;
        T* it = &v[0];
        do {
            p->convert(*it);
            ++p;
            ++it;
        } while(p < pend);
    }
    return o;
}

template <typename Stream, typename T, std::size_t N>
inline msgpack::packer<Stream>& operator<< (msgpack::packer<Stream>& o, const std::array<T, N>& v) {
    uint32_t size = checked_get_container_size(v.size());
    o.pack_array(size);
    for(auto const& e : v) o.pack(e);
    return o;
}

template <typename T, std::size_t N>
inline void operator<< (msgpack::object::with_zone& o, const std::array<T, N>& v) {
    o.type = msgpack::type::ARRAY;
    if(v.empty()) {
        o.via.array.ptr = nullptr;
        o.via.array.size = 0;
    } else {
        uint32_t size = checked_get_container_size(v.size());
        msgpack::object* p = static_cast<msgpack::object*>(o.zone.allocate_align(sizeof(msgpack::object)*size));
        o.via.array.size = size;
        o.via.array.ptr = p;
        for (auto const& e : v) *p++ = msgpack::object(e, o.zone);
    }
}

}  // MSGPACK_API_VERSION_NAMESPACE(v1)

} // namespace msgpack

#endif // MSGPACK_CPP11_ARRAY_HPP
