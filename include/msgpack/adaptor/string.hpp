//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2008-2015 FURUHASHI Sadayuki
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
#ifndef MSGPACK_TYPE_STRING_HPP
#define MSGPACK_TYPE_STRING_HPP

#include "msgpack/versioning.hpp"
#include "msgpack/object_fwd.hpp"
#include "msgpack/adaptor/check_container_size.hpp"

#include <string>

namespace msgpack {

MSGPACK_API_VERSION_NAMESPACE(v1) {

inline msgpack::object const& operator>> (msgpack::object const& o, std::string& v)
{
    switch (o.type) {
    case msgpack::type::BIN:
        v.assign(o.via.bin.ptr, o.via.bin.size);
        break;
    case msgpack::type::STR:
        v.assign(o.via.str.ptr, o.via.str.size);
        break;
    default:
        throw msgpack::type_error();
        break;
    }
    return o;
}

template <typename Stream>
inline msgpack::packer<Stream>& operator<< (msgpack::packer<Stream>& o, const std::string& v)
{
    uint32_t size = checked_get_container_size(v.size());
    o.pack_str(size);
    o.pack_str_body(v.data(), size);
    return o;
}

inline void operator<< (msgpack::object::with_zone& o, const std::string& v)
{
    uint32_t size = checked_get_container_size(v.size());
    o.type = msgpack::type::STR;
    char* ptr = static_cast<char*>(o.zone.allocate_align(size));
    o.via.str.ptr = ptr;
    o.via.str.size = size;
    std::memcpy(ptr, v.data(), v.size());
}

inline void operator<< (msgpack::object& o, const std::string& v)
{
    uint32_t size = checked_get_container_size(v.size());
    o.type = msgpack::type::STR;
    o.via.str.ptr = v.data();
    o.via.str.size = size;
}

}  // MSGPACK_API_VERSION_NAMESPACE(v1)

}  // namespace msgpack

#endif // MSGPACK_TYPE_STRING_HPP
