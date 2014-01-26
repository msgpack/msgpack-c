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
#ifndef MSGPACK_TYPE_STRING_HPP
#define MSGPACK_TYPE_STRING_HPP

#include "msgpack/object.hpp"
#include <string>

namespace msgpack {


inline std::string& operator>> (object const& o, std::string& v)
{
	if(o.type != type::BIN) { throw type_error(); }
	v.assign(o.via.bin.ptr, o.via.bin.size);
	return v;
}

template <typename Stream>
inline packer<Stream>& operator<< (packer<Stream>& o, const std::string& v)
{
	o.pack_bin(v.size());
	o.pack_bin_body(v.data(), v.size());
	return o;
}

inline void operator<< (object::with_zone& o, const std::string& v)
{
	o.type = type::BIN;
	char* ptr = (char*)o.zone->malloc(v.size());
	o.via.bin.ptr = ptr;
	o.via.bin.size = (uint32_t)v.size();
	memcpy(ptr, v.data(), v.size());
}

inline void operator<< (object& o, const std::string& v)
{
	o.type = type::BIN;
	o.via.bin.ptr = v.data();
	o.via.bin.size = (uint32_t)v.size();
}


}  // namespace msgpack

#endif /* msgpack/type/string.hpp */

