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
#ifndef MSGPACK_TYPE_UNORDERED_MAP_HPP
#define MSGPACK_TYPE_UNORDERED_MAP_HPP

#include "msgpack/versioning.hpp"
#include "msgpack/object_fwd.hpp"
#include "msgpack/adaptor/check_container_size.hpp"

#include <unordered_map>

namespace msgpack {

MSGPACK_API_VERSION_NAMESPACE(v1) {

template <typename K, typename V>
inline msgpack::object const& operator>> (msgpack::object const& o, std::unordered_map<K, V>& v)
{
    if(o.type != msgpack::type::MAP) { throw msgpack::type_error(); }
    msgpack::object_kv* p(o.via.map.ptr);
    msgpack::object_kv* const pend(o.via.map.ptr + o.via.map.size);
    std::unordered_map<K, V> tmp;
    for(; p != pend; ++p) {
        K key;
        p->key.convert(key);
        p->val.convert(tmp[key]);
    }
    tmp.swap(v);
    return o;
}

template <typename Stream, typename K, typename V>
inline msgpack::packer<Stream>& operator<< (msgpack::packer<Stream>& o, const std::unordered_map<K,V>& v)
{
    uint32_t size = checked_get_container_size(v.size());
    o.pack_map(size);
    for(typename std::unordered_map<K,V>::const_iterator it(v.begin()), it_end(v.end());
        it != it_end; ++it) {
        o.pack(it->first);
        o.pack(it->second);
    }
    return o;
}

template <typename K, typename V>
inline void operator<< (msgpack::object::with_zone& o, const std::unordered_map<K,V>& v)
{
    o.type = msgpack::type::MAP;
    if(v.empty()) {
        o.via.map.ptr  = nullptr;
        o.via.map.size = 0;
    } else {
        uint32_t size = checked_get_container_size(v.size());
        msgpack::object_kv* p = static_cast<msgpack::object_kv*>(o.zone.allocate_align(sizeof(msgpack::object_kv)*size));
        msgpack::object_kv* const pend = p + size;
        o.via.map.ptr  = p;
        o.via.map.size = size;
        typename std::unordered_map<K,V>::const_iterator it(v.begin());
        do {
            p->key = msgpack::object(it->first, o.zone);
            p->val = msgpack::object(it->second, o.zone);
            ++p;
            ++it;
        } while(p < pend);
    }
}


template <typename K, typename V>
inline msgpack::object const& operator>> (msgpack::object const& o, std::unordered_multimap<K, V>& v)
{
    if(o.type != msgpack::type::MAP) { throw msgpack::type_error(); }
    msgpack::object_kv* p(o.via.map.ptr);
    msgpack::object_kv* const pend(o.via.map.ptr + o.via.map.size);
    std::unordered_multimap<K, V> tmp;
    for(; p != pend; ++p) {
        std::pair<K, V> value;
        p->key.convert(value.first);
        p->val.convert(value.second);
        tmp.insert(value);
    }
    tmp.swap(v);
    return o;
}

template <typename Stream, typename K, typename V>
inline msgpack::packer<Stream>& operator<< (msgpack::packer<Stream>& o, const std::unordered_multimap<K,V>& v)
{
    uint32_t size = checked_get_container_size(v.size());
    o.pack_map(size);
    for(typename std::unordered_multimap<K,V>::const_iterator it(v.begin()), it_end(v.end());
        it != it_end; ++it) {
        o.pack(it->first);
        o.pack(it->second);
    }
    return o;
}

template <typename K, typename V>
inline void operator<< (msgpack::object::with_zone& o, const std::unordered_multimap<K,V>& v)
{
    o.type = msgpack::type::MAP;
    if(v.empty()) {
        o.via.map.ptr  = nullptr;
        o.via.map.size = 0;
    } else {
        uint32_t size = checked_get_container_size(v.size());
        msgpack::object_kv* p = static_cast<msgpack::object_kv*>(o.zone.allocate_align(sizeof(msgpack::object_kv)*size));
        msgpack::object_kv* const pend = p + size;
        o.via.map.ptr  = p;
        o.via.map.size = size;
        typename std::unordered_multimap<K,V>::const_iterator it(v.begin());
        do {
            p->key = msgpack::object(it->first, o.zone);
            p->val = msgpack::object(it->second, o.zone);
            ++p;
            ++it;
        } while(p < pend);
    }
}

}  // MSGPACK_API_VERSION_NAMESPACE(v1)

}  // namespace msgpack


#endif // MSGPACK_TYPE_UNORDERED_MAP_HPP
