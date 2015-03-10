//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2008-2009 FURUHASHI Sadayuki
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
#ifndef MSGPACK_TYPE_FLOAT_HPP
#define MSGPACK_TYPE_FLOAT_HPP

#include "msgpack/versioning.hpp"
#include "msgpack/object_fwd.hpp"
#include <vector>

namespace msgpack {

MSGPACK_API_VERSION_NAMESPACE(v1) {

// FIXME check overflow, underflow


inline msgpack::object const& operator>> (msgpack::object const& o, float& v)
{
    if(o.type == msgpack::type::FLOAT) {
        v = static_cast<float>(o.via.f64);
    }
    else if (o.type == msgpack::type::POSITIVE_INTEGER) {
        v = static_cast<float>(o.via.u64);
    }
    else if (o.type == msgpack::type::NEGATIVE_INTEGER) {
        v = static_cast<float>(o.via.i64);
    }
    else {
        throw msgpack::type_error();
    }
    return o;
}

template <typename Stream>
inline msgpack::packer<Stream>& operator<< (msgpack::packer<Stream>& o, const float& v)
{
    o.pack_float(v);
    return o;
}


inline msgpack::object const& operator>> (msgpack::object const& o, double& v)
{
    if(o.type == msgpack::type::FLOAT) {
        v = o.via.f64;
    }
    else if (o.type == msgpack::type::POSITIVE_INTEGER) {
        v = static_cast<double>(o.via.u64);
    }
    else if (o.type == msgpack::type::NEGATIVE_INTEGER) {
        v = static_cast<double>(o.via.i64);
    }
    else {
        throw msgpack::type_error();
    }
    return o;
}

template <typename Stream>
inline msgpack::packer<Stream>& operator<< (msgpack::packer<Stream>& o, const double& v)
{
    o.pack_double(v);
    return o;
}


inline void operator<< (msgpack::object& o, float v)
{
    o.type = msgpack::type::FLOAT;
    o.via.f64 = static_cast<double>(v);
}

inline void operator<< (msgpack::object& o, double v)
{
    o.type = msgpack::type::FLOAT;
    o.via.f64 = v;
}

inline void operator<< (msgpack::object::with_zone& o, float v)
    { static_cast<msgpack::object&>(o) << v; }

inline void operator<< (msgpack::object::with_zone& o, double v)
    { static_cast<msgpack::object&>(o) << v; }


}  // MSGPACK_API_VERSION_NAMESPACE(v1)

}  // namespace msgpack

#endif // MSGPACK_TYPE_FLOAT_HPP
