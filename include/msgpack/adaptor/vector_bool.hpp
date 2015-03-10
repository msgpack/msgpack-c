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
#ifndef MSGPACK_TYPE_VECTOR_BOOL_HPP
#define MSGPACK_TYPE_VECTOR_BOOL_HPP

#include "msgpack/versioning.hpp"
#include "msgpack/object_fwd.hpp"
#include <vector>

namespace msgpack {

MSGPACK_API_VERSION_NAMESPACE(v1) {

inline msgpack::object const& operator>> (msgpack::object const& o, std::vector<bool>& v)
{
    if (o.type != msgpack::type::ARRAY) { throw msgpack::type_error(); }
    if (o.via.array.size > 0) {
        v.resize(o.via.array.size);
        msgpack::object* p = o.via.array.ptr;
        for (std::vector<bool>::iterator it = v.begin(), end = v.end();
             it != end;
             ++it) {
            *it = p->as<bool>();
            ++p;
        }
    }
    return o;
}

template <typename Stream>
inline msgpack::packer<Stream>& operator<< (msgpack::packer<Stream>& o, const std::vector<bool>& v)
{
    o.pack_array(v.size());
    for(std::vector<bool>::const_iterator it(v.begin()), it_end(v.end());
        it != it_end; ++it) {
        o.pack(static_cast<bool>(*it));
    }
    return o;
}

inline void operator<< (msgpack::object::with_zone& o, const std::vector<bool>& v)
{
    o.type = msgpack::type::ARRAY;
    if(v.empty()) {
        o.via.array.ptr = nullptr;
        o.via.array.size = 0;
    } else {
        msgpack::object* p = static_cast<msgpack::object*>(o.zone.allocate_align(sizeof(msgpack::object)*v.size()));
        msgpack::object* const pend = p + v.size();
        o.via.array.ptr = p;
        o.via.array.size = v.size();
        std::vector<bool>::const_iterator it(v.begin());
        do {
            *p = object(static_cast<bool>(*it), o.zone);
            ++p;
            ++it;
        } while(p < pend);
    }
}

}  // MSGPACK_API_VERSION_NAMESPACE(v1)

}  // namespace msgpack

#endif // MSGPACK_TYPE_VECTOR_BOOL_HPP
