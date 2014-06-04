//
// MessagePack for C++ simple buffer implementation
//
// Copyright (C) 2008-2013 FURUHASHI Sadayuki and KONDO Takatoshi
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
#ifndef MSGPACK_SBUFFER_HPP
#define MSGPACK_SBUFFER_HPP

#include <stdexcept>

#ifndef MSGPACK_SBUFFER_INIT_SIZE
#define MSGPACK_SBUFFER_INIT_SIZE 8192
#endif

namespace msgpack {


class sbuffer {
public:
	sbuffer(size_t initsz = MSGPACK_SBUFFER_INIT_SIZE):size_(0), alloc_(initsz)
	{
		if(initsz == 0) {
			data_ = nullptr;
		} else {
			data_ = (char*)::malloc(initsz);
			if(!data_) {
				throw std::bad_alloc();
			}
		}
	}

	~sbuffer()
	{
		::free(data_);
	}

public:
	void write(const char* buf, size_t len)
	{
		if(alloc_ - size_ < len) {
			expand_buffer(len);
		}
		::memcpy(data_ + size_, buf, len);
		size_ += len;
	}

	char* data()
	{
		return data_;
	}

	const char* data() const
	{
		return data_;
	}

	size_t size() const
	{
		return size_;
	}

	char* release()
	{
		char* tmp = data_;
		size_ = 0;
		data_ = nullptr;
		alloc_ = 0;
		return tmp;
	}

	void clear()
	{
		size_ = 0;
	}

private:
	void expand_buffer(size_t len)
	{
		size_t nsize = (alloc_ > 0) ?
				alloc_ * 2 : MSGPACK_SBUFFER_INIT_SIZE;
	
		while(nsize < size_ + len) { nsize *= 2; }
	
		void* tmp = ::realloc(data_, nsize);
		if(!tmp) {
			throw std::bad_alloc();
		}
	
		data_ = static_cast<char*>(tmp);
		alloc_ = nsize;
	}

private:
	sbuffer(const sbuffer&);
private:
	size_t size_;
	char* data_;
	size_t alloc_;
};


}  // namespace msgpack

#endif /* msgpack/sbuffer.hpp */

