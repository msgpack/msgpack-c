//
// MessagePack for C++ memory pool
//
// Copyright (C) 2008-2010 FURUHASHI Sadayuki
//
//	  Licensed under the Apache License, Version 2.0 (the "License");
//	  you may not use this file except in compliance with the License.
//	  You may obtain a copy of the License at
//
//		  http://www.apache.org/licenses/LICENSE-2.0
//
//	  Unless required by applicable law or agreed to in writing, software
//	  distributed under the License is distributed on an "AS IS" BASIS,
//	  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//	  See the License for the specific language governing permissions and
//	  limitations under the License.
//
#ifndef MSGPACK_CPP03_ZONE_HPP
#define MSGPACK_CPP03_ZONE_HPP

#include <cstdlib>
#include <memory>
#include <vector>

#include "msgpack/cpp_config.hpp"

#ifndef MSGPACK_ZONE_CHUNK_SIZE
#define MSGPACK_ZONE_CHUNK_SIZE 8192
#endif

#ifndef MSGPACK_ZONE_ALIGN
#define MSGPACK_ZONE_ALIGN sizeof(int)
#endif


namespace msgpack {

class zone {
	struct finalizer {
		finalizer(void (*func)(void*), void* data):func_(func), data_(data) {}
		void operator()() { func_(data_); }
		void (*func_)(void*);
		void* data_;
	};
	struct finalizer_array {
		finalizer_array():tail_(nullptr), end_(nullptr), array_(nullptr) {}
		void call() {
			finalizer* fin = tail_;
			for(; fin != array_; --fin) (*(fin-1))();
		}
		~finalizer_array() {
			call();
			::free(array_);
		}
		void clear() {
			call();
			tail_ = array_;
		}
		void push(void (*func)(void* data), void* data)
		{
			finalizer* fin = tail_;

			if(fin == end_) {
				push_expand(func, data);
				return;
			}

			fin->func_ = func;
			fin->data_ = data;

			++tail_;
		}
		void push_expand(void (*func)(void*), void* data) {
			const size_t nused = end_ - array_;
			size_t nnext;
			if(nused == 0) {
				nnext = (sizeof(finalizer) < 72/2) ?
					72 / sizeof(finalizer) : 8;
			} else {
				nnext = nused * 2;
			}
			finalizer* tmp =
				static_cast<finalizer*>(::realloc(array_, sizeof(finalizer) * nnext));
			if(!tmp) {
				throw std::bad_alloc();
			}
			array_	= tmp;
			end_	= tmp + nnext;
			tail_	= tmp + nused;
			new (tail_) finalizer(func, data);

			++tail_;
		}
		finalizer* tail_;
		finalizer* end_;
		finalizer* array_;
	};
	struct chunk {
		chunk* next_;
	};
	struct chunk_list {
		chunk_list(size_t chunk_size)
		{
			chunk* c = static_cast<chunk*>(::malloc(sizeof(chunk) + chunk_size));
			if(!c) {
				throw std::bad_alloc();
			}

			head_ = c;
			free_ = chunk_size;
			ptr_  = reinterpret_cast<char*>(c) + sizeof(chunk);
			c->next_ = nullptr;
		}
		~chunk_list()
		{
			chunk* c = head_;
			while(true) {
				chunk* n = c->next_;
				::free(c);
				if(n) {
					c = n;
				} else {
					break;
				}
			}
		}
		void clear(size_t chunk_size)
		{
			chunk* c = head_;
			while(true) {
				chunk* n = c->next_;
				if(n) {
					::free(c);
					c = n;
				} else {
					break;
				}
			}
			head_->next_ = nullptr;
			free_ = chunk_size;
			ptr_  = reinterpret_cast<char*>(head_) + sizeof(chunk);
		}
		size_t free_;
		char* ptr_;
		chunk* head_;
	};
	size_t chunk_size_;
	chunk_list chunk_list_;
	finalizer_array finalizer_array_;

public:
	zone(size_t chunk_size = MSGPACK_ZONE_CHUNK_SIZE) /* throw() */;

public:
	static zone* create(size_t chunk_size);
	static void destroy(zone* zone);
	void* allocate_align(size_t size);
	void* allocate_no_align(size_t size);

	void push_finalizer(void (*func)(void*), void* data);

	template <typename T>
	void push_finalizer(msgpack::unique_ptr<T> obj);

	void clear();

	void swap(zone& o);
	static void* operator new(std::size_t size) throw(std::bad_alloc)
	{
		void* p = ::malloc(size);
		if (!p) throw std::bad_alloc();
		return p;
	}
	static void operator delete(void *p) throw()
	{
		::free(p);
	}
	static void* operator new(std::size_t size, void* place) throw()
	{
		return ::operator new(size, place);
	}
	static void operator delete(void* p, void* place) throw()
	{
		::operator delete(p, place);
	}
	
	template <typename T>
	T* allocate();
	
	template <typename T, typename A1>
	T* allocate(A1 a1);
	
	template <typename T, typename A1, typename A2>
	T* allocate(A1 a1, A2 a2);
	
	template <typename T, typename A1, typename A2, typename A3>
	T* allocate(A1 a1, A2 a2, A3 a3);
	
	template <typename T, typename A1, typename A2, typename A3, typename A4>
	T* allocate(A1 a1, A2 a2, A3 a3, A4 a4);
	
	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
	T* allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5);
	
	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	T* allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6);
	
	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	T* allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7);
	
	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
	T* allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8);
	
	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
	T* allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9);
	
	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
	T* allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10);
	
	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
	T* allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11);
	
	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
	T* allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12);
	
	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
	T* allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13);
	
	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
	T* allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14);
	
	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
	T* allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15);
	

private:
	void undo_allocate(size_t size);

	template <typename T>
	static void object_destructor(void* obj);

	void* allocate_expand(size_t size);
};


inline zone* zone::create(size_t chunk_size)
{
	zone* z = static_cast<zone*>(::malloc(sizeof(zone) + chunk_size));
	if (!z) {
		return nullptr;
	}
	new (z) zone(chunk_size);
	return z;
}

inline void zone::destroy(zone* z)
{
	z->~zone();
	::free(z);
}

inline zone::zone(size_t chunk_size) /* throw() */ :chunk_size_(chunk_size), chunk_list_(chunk_size_)
{
}

inline void* zone::allocate_align(size_t size)
{
	return allocate_no_align(
		((size)+((MSGPACK_ZONE_ALIGN)-1)) & ~((MSGPACK_ZONE_ALIGN)-1));
}

inline void* zone::allocate_no_align(size_t size)
{
	if(chunk_list_.free_ < size) {
		return allocate_expand(size);
	}

	char* ptr = chunk_list_.ptr_;
	chunk_list_.free_ -= size;
	chunk_list_.ptr_  += size;

	return ptr;
}

inline void* zone::allocate_expand(size_t size)
{
	chunk_list* const cl = &chunk_list_;

	size_t sz = chunk_size_;

	while(sz < size) {
		sz *= 2;
	}

	chunk* c = static_cast<chunk*>(::malloc(sizeof(chunk) + sz));

	char* ptr = reinterpret_cast<char*>(c) + sizeof(chunk);

	c->next_  = cl->head_;
	cl->head_ = c;
	cl->free_ = sz - size;
	cl->ptr_  = ptr + size;

	return ptr;
}

inline void zone::push_finalizer(void (*func)(void*), void* data)
{
	finalizer_array_.push(func, data);
}

template <typename T>
inline void zone::push_finalizer(msgpack::unique_ptr<T> obj)
{
	finalizer_array_.push(&zone::object_destructor<T>, obj.get());
	obj.release();
}

inline void zone::clear()
{
	finalizer_array_.clear();
	chunk_list_.clear(chunk_size_);
}

inline void zone::swap(zone& o)
{
	std::swap(*this, o);
}

template <typename T>
void zone::object_destructor(void* obj)
{
	reinterpret_cast<T*>(obj)->~T();
}

inline void zone::undo_allocate(size_t size)
{
	chunk_list_.ptr_  -= size;
	chunk_list_.free_ += size;
}


template <typename T>
T* zone::allocate()
{
	void* x = allocate_align(sizeof(T));
	try {
		finalizer_array_.push(&zone::object_destructor<T>, x);
	} catch (...) {
		undo_allocate(sizeof(T));
		throw;
	}
	try {
		return new (x) T();
	} catch (...) {
		--finalizer_array_.tail_;
		undo_allocate(sizeof(T));
		throw;
	}
}

template <typename T, typename A1>
T* zone::allocate(A1 a1)
{
	void* x = allocate_align(sizeof(T));
	try {
		finalizer_array_.push(&zone::object_destructor<T>, x);
	} catch (...) {
		undo_allocate(sizeof(T));
		throw;
	}
	try {
		return new (x) T(a1);
	} catch (...) {
		--finalizer_array_.tail_;
		undo_allocate(sizeof(T));
		throw;
	}
}

template <typename T, typename A1, typename A2>
T* zone::allocate(A1 a1, A2 a2)
{
	void* x = allocate_align(sizeof(T));
	try {
		finalizer_array_.push(&zone::object_destructor<T>, x);
	} catch (...) {
		undo_allocate(sizeof(T));
		throw;
	}
	try {
		return new (x) T(a1, a2);
	} catch (...) {
		--finalizer_array_.tail_;
		undo_allocate(sizeof(T));
		throw;
	}
}

template <typename T, typename A1, typename A2, typename A3>
T* zone::allocate(A1 a1, A2 a2, A3 a3)
{
	void* x = allocate_align(sizeof(T));
	try {
		finalizer_array_.push(&zone::object_destructor<T>, x);
	} catch (...) {
		undo_allocate(sizeof(T));
		throw;
	}
	try {
		return new (x) T(a1, a2, a3);
	} catch (...) {
		--finalizer_array_.tail_;
		undo_allocate(sizeof(T));
		throw;
	}
}

template <typename T, typename A1, typename A2, typename A3, typename A4>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4)
{
	void* x = allocate_align(sizeof(T));
	try {
		finalizer_array_.push(&zone::object_destructor<T>, x);
	} catch (...) {
		undo_allocate(sizeof(T));
		throw;
	}
	try {
		return new (x) T(a1, a2, a3, a4);
	} catch (...) {
		--finalizer_array_.tail_;
		undo_allocate(sizeof(T));
		throw;
	}
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
	void* x = allocate_align(sizeof(T));
	try {
		finalizer_array_.push(&zone::object_destructor<T>, x);
	} catch (...) {
		undo_allocate(sizeof(T));
		throw;
	}
	try {
		return new (x) T(a1, a2, a3, a4, a5);
	} catch (...) {
		--finalizer_array_.tail_;
		undo_allocate(sizeof(T));
		throw;
	}
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
{
	void* x = allocate_align(sizeof(T));
	try {
		finalizer_array_.push(&zone::object_destructor<T>, x);
	} catch (...) {
		undo_allocate(sizeof(T));
		throw;
	}
	try {
		return new (x) T(a1, a2, a3, a4, a5, a6);
	} catch (...) {
		--finalizer_array_.tail_;
		undo_allocate(sizeof(T));
		throw;
	}
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
{
	void* x = allocate_align(sizeof(T));
	try {
		finalizer_array_.push(&zone::object_destructor<T>, x);
	} catch (...) {
		undo_allocate(sizeof(T));
		throw;
	}
	try {
		return new (x) T(a1, a2, a3, a4, a5, a6, a7);
	} catch (...) {
		--finalizer_array_.tail_;
		undo_allocate(sizeof(T));
		throw;
	}
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
{
	void* x = allocate_align(sizeof(T));
	try {
		finalizer_array_.push(&zone::object_destructor<T>, x);
	} catch (...) {
		undo_allocate(sizeof(T));
		throw;
	}
	try {
		return new (x) T(a1, a2, a3, a4, a5, a6, a7, a8);
	} catch (...) {
		--finalizer_array_.tail_;
		undo_allocate(sizeof(T));
		throw;
	}
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
{
	void* x = allocate_align(sizeof(T));
	try {
		finalizer_array_.push(&zone::object_destructor<T>, x);
	} catch (...) {
		undo_allocate(sizeof(T));
		throw;
	}
	try {
		return new (x) T(a1, a2, a3, a4, a5, a6, a7, a8, a9);
	} catch (...) {
		--finalizer_array_.tail_;
		undo_allocate(sizeof(T));
		throw;
	}
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10)
{
	void* x = allocate_align(sizeof(T));
	try {
		finalizer_array_.push(&zone::object_destructor<T>, x);
	} catch (...) {
		undo_allocate(sizeof(T));
		throw;
	}
	try {
		return new (x) T(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
	} catch (...) {
		--finalizer_array_.tail_;
		undo_allocate(sizeof(T));
		throw;
	}
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11)
{
	void* x = allocate_align(sizeof(T));
	try {
		finalizer_array_.push(&zone::object_destructor<T>, x);
	} catch (...) {
		undo_allocate(sizeof(T));
		throw;
	}
	try {
		return new (x) T(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
	} catch (...) {
		--finalizer_array_.tail_;
		undo_allocate(sizeof(T));
		throw;
	}
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12)
{
	void* x = allocate_align(sizeof(T));
	try {
		finalizer_array_.push(&zone::object_destructor<T>, x);
	} catch (...) {
		undo_allocate(sizeof(T));
		throw;
	}
	try {
		return new (x) T(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
	} catch (...) {
		--finalizer_array_.tail_;
		undo_allocate(sizeof(T));
		throw;
	}
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13)
{
	void* x = allocate_align(sizeof(T));
	try {
		finalizer_array_.push(&zone::object_destructor<T>, x);
	} catch (...) {
		undo_allocate(sizeof(T));
		throw;
	}
	try {
		return new (x) T(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
	} catch (...) {
		--finalizer_array_.tail_;
		undo_allocate(sizeof(T));
		throw;
	}
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14)
{
	void* x = allocate_align(sizeof(T));
	try {
		finalizer_array_.push(&zone::object_destructor<T>, x);
	} catch (...) {
		undo_allocate(sizeof(T));
		throw;
	}
	try {
		return new (x) T(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
	} catch (...) {
		--finalizer_array_.tail_;
		undo_allocate(sizeof(T));
		throw;
	}
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15)
{
	void* x = allocate_align(sizeof(T));
	try {
		finalizer_array_.push(&zone::object_destructor<T>, x);
	} catch (...) {
		undo_allocate(sizeof(T));
		throw;
	}
	try {
		return new (x) T(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
	} catch (...) {
		--finalizer_array_.tail_;
		undo_allocate(sizeof(T));
		throw;
	}
}


}  // namespace msgpack

#endif // MSGPACK_CPP03_ZONE_HPP
