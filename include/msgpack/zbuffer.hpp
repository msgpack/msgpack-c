//
// MessagePack for C++ deflate buffer implementation
//
// Copyright (C) 2010-2013 FURUHASHI Sadayuki and KONDO Takatoshi
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
#ifndef MSGPACK_ZBUFFER_HPP
#define MSGPACK_ZBUFFER_HPP

#include <stdexcept>
#include <zlib.h>

#ifndef MSGPACK_ZBUFFER_RESERVE_SIZE
#define MSGPACK_ZBUFFER_RESERVE_SIZE 512
#endif

#ifndef MSGPACK_ZBUFFER_INIT_SIZE
#define MSGPACK_ZBUFFER_INIT_SIZE 8192
#endif

namespace msgpack {


class zbuffer {
public:
	zbuffer(int level = Z_DEFAULT_COMPRESSION,
			size_t init_size = MSGPACK_ZBUFFER_INIT_SIZE)
		: data_(nullptr), init_size_(init_size)
	{
		stream_.zalloc = Z_NULL;
		stream_.zfree = Z_NULL;
		stream_.opaque = Z_NULL;
		stream_.next_out = Z_NULL;
		stream_.avail_out = 0;
		if(deflateInit(&stream_, level) != Z_OK) {
			throw std::bad_alloc();
		}
	}

	~zbuffer()
	{
		deflateEnd(&stream_);
		::free(data_);
	}

public:
	void write(const char* buf, size_t len)
	{
		stream_.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(buf));
		stream_.avail_in = len;

		do {
			if(stream_.avail_out < MSGPACK_ZBUFFER_RESERVE_SIZE) {
				if(!expand()) {
					throw std::bad_alloc();
				}
			}

			if(deflate(&stream_, Z_NO_FLUSH) != Z_OK) {
				throw std::bad_alloc();
			}
		} while(stream_.avail_in > 0);
	}

	char* flush()
	{
		while(true) {
			switch(deflate(&stream_, Z_FINISH)) {
			case Z_STREAM_END:
				return data_;
			case Z_OK:
				if(!expand()) {
					throw std::bad_alloc();
				}
				break;
			default:
				throw std::bad_alloc();
			}
		}
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
		return reinterpret_cast<char*>(stream_.next_out) - data_;
	}

	void reset()
	{
		if(deflateReset(&stream_) != Z_OK) {
			throw std::bad_alloc();
		}
		reset_buffer();
	}

	void reset_buffer()
	{
		stream_.avail_out += reinterpret_cast<char*>(stream_.next_out) - data_;
		stream_.next_out = reinterpret_cast<Bytef*>(data_);
	}

	char* release_buffer()
	{
		char* tmp = data_;
		data_ = nullptr;
		stream_.next_out = nullptr;
		stream_.avail_out = 0;
		return tmp;
	}

private:
	bool expand()
	{
		size_t used = reinterpret_cast<char*>(stream_.next_out) - data_;
		size_t csize = used + stream_.avail_out;
		size_t nsize = (csize == 0) ? init_size_ : csize * 2;

		char* tmp = static_cast<char*>(::realloc(data_, nsize));
		if(tmp == nullptr) {
			return false;
		}

		data_ = tmp;
		stream_.next_out  = reinterpret_cast<Bytef*>(tmp + used);
		stream_.avail_out = nsize - used;

		return true;
	}
private:
	zbuffer(const zbuffer&);

private:
	z_stream stream_;
	char* data_;
	size_t init_size_;
};


}  // namespace msgpack

#endif /* msgpack/zbuffer.hpp */

