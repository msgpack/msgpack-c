//
// MessagePack for C++ zero-copy buffer implementation
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
#ifndef MSGPACK_VREFBUFFER_HPP
#define MSGPACK_VREFBUFFER_HPP

#include <stdexcept>

#ifndef MSGPACK_VREFBUFFER_REF_SIZE
#define MSGPACK_VREFBUFFER_REF_SIZE 32
#endif

#ifndef MSGPACK_VREFBUFFER_CHUNK_SIZE
#define MSGPACK_VREFBUFFER_CHUNK_SIZE 8192
#endif

#ifndef _WIN32
#include <sys/uio.h>
#else
struct iovec {
	void  *iov_base;
	size_t iov_len;
};
#endif

namespace msgpack {

namespace detail {


} // detail

class vrefbuffer {
private:
	struct chunk {
		chunk* next;
	};
	struct inner_buffer {
		size_t free;
		char*  ptr;
		chunk* head;
	};
public:
	vrefbuffer(size_t ref_size = MSGPACK_VREFBUFFER_REF_SIZE,
			size_t chunk_size = MSGPACK_VREFBUFFER_CHUNK_SIZE)
		:ref_size_(ref_size), chunk_size_(chunk_size)
	{
		size_t nfirst = (sizeof(iovec) < 72/2) ?
			72 / sizeof(iovec) : 8;

		iovec* array = (iovec*)::malloc(
			sizeof(iovec) * nfirst);
		if(!array) {
			throw std::bad_alloc();
		}
		
		tail_  = array;
		end_   = array + nfirst;
		array_ = array;

		chunk* c = (chunk*)::malloc(sizeof(chunk) + chunk_size);
		if(!c) {
			::free(array);
			throw std::bad_alloc();
		}
		inner_buffer* const ib = &inner_buffer_;

		ib->free = chunk_size;
		ib->ptr	 = ((char*)c) + sizeof(chunk);
		ib->head = c;
		c->next = nullptr;
		
	}

	~vrefbuffer()
	{
		chunk* c = inner_buffer_.head;
		while(true) {
			chunk* n = c->next;
			::free(c);
			if(n != NULL) {
				c = n;
			} else {
				break;
			}
		}
		::free(array_);
	}

public:
	void write(const char* buf, size_t len)
	{
		if(len < ref_size_) {
			append_copy(buf, len);
		} else {
			append_ref(buf, len);
		}
	}

	void append_ref(const char* buf, size_t len)
	{
		if(tail_ == end_) {
			const size_t nused = tail_ - array_;
			const size_t nnext = nused * 2;

			iovec* nvec = (iovec*)::realloc(
				array_, sizeof(iovec)*nnext);
			if(!nvec) {
				throw std::bad_alloc();
			}
			
			array_ = nvec;
			end_   = nvec + nnext;
			tail_  = nvec + nused;
		}
		
		tail_->iov_base = (char*)buf;
		tail_->iov_len	= len;
		++tail_;
	}

	void append_copy(const char* buf, size_t len)
	{
		inner_buffer* const ib = &inner_buffer_;

		if(ib->free < len) {
			size_t sz = chunk_size_;
			if(sz < len) {
				sz = len;
			}

			chunk* c = (chunk*)::malloc(sizeof(chunk) + sz);
			if(!c) {
				throw std::bad_alloc();
			}
			
			c->next = ib->head;
			ib->head = c;
			ib->free = sz;
			ib->ptr	 = ((char*)c) + sizeof(chunk);
		}

		char* m = ib->ptr;
		::memcpy(m, buf, len);
		ib->free -= len;
		ib->ptr	 += len;
		
		if(tail_ != array_ && m ==
			(const char*)((tail_ - 1)->iov_base) + (tail_ - 1)->iov_len) {
			(tail_ - 1)->iov_len += len;
			return;
		} else {
			append_ref( m, len);
		}
	}

	const struct iovec* vector() const
	{
		return array_;
	}

	size_t vector_size() const
	{
		return tail_ - array_;
	}

	void migrate(vrefbuffer* to)
	{
		size_t sz = chunk_size_;

		chunk* empty = (chunk*)::malloc(sizeof(chunk) + sz);
		if(!empty) {
			throw std::bad_alloc();
		}

		empty->next = nullptr;

		const size_t nused = tail_ - array_;
		if(to->tail_ + nused < end_) {
			const size_t tosize = to->tail_ - to->array_;
			const size_t reqsize = nused + tosize;
			size_t nnext = (to->end_ - to->array_) * 2;
			while(nnext < reqsize) {
				nnext *= 2;
			}

			iovec* nvec = (iovec*)::realloc(
				to->array_, sizeof(iovec)*nnext);
			if(!nvec) {
				::free(empty);
				throw std::bad_alloc();
			}

			to->array_ = nvec;
			to->end_   = nvec + nnext;
			to->tail_  = nvec + tosize;
		}

		::memcpy(to->tail_, array_, sizeof(iovec)*nused);
			
		to->tail_ += nused;
		tail_ = array_;


		inner_buffer* const ib = &inner_buffer_;
		inner_buffer* const toib = &to->inner_buffer_;

		chunk* last = ib->head;
		while(last->next) {
			last = last->next;
		}
		last->next = toib->head;
		toib->head = ib->head;
			
		if(toib->free < ib->free) {
			toib->free = ib->free;
			toib->ptr  = ib->ptr;
		}
			
		ib->head = empty;
		ib->free = sz;
		ib->ptr	 = ((char*)empty) + sizeof(chunk);
			
	}
		
	void clear()
	{
		chunk* c = inner_buffer_.head->next;
		chunk* n;
		while(c) {
			n = c->next;
			::free(c);
			c = n;
		}

		inner_buffer* const ib = &inner_buffer_;
		c = ib->head;
		c->next = nullptr;
		ib->free = chunk_size_;
		ib->ptr	 = ((char*)c) + sizeof(chunk);

		tail_ = array_;
	}

private:
	vrefbuffer(const vrefbuffer&);

private:
	iovec* tail_;
	iovec* end_;
	iovec* array_;

	size_t ref_size_;
	size_t chunk_size_;

	inner_buffer inner_buffer_;

};


}  // namespace msgpack

#endif /* msgpack/vrefbuffer.hpp */

