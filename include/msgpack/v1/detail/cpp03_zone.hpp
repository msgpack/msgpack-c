//
// MessagePack for C++ memory pool
//
// Copyright (C) 2008-2016 FURUHASHI Sadayuki and KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_V1_CPP03_ZONE_HPP
#define MSGPACK_V1_CPP03_ZONE_HPP

#include "msgpack/versioning.hpp"
#include "msgpack/cpp_config.hpp"
#include "msgpack/zone_decl.hpp"
#include "msgpack/assert.hpp"

#include <stdint.h>
#include <cstdlib>
#include <memory>
#include <vector>


namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond

class zone {
    struct finalizer {
        finalizer(void (*func)(void*), void* data, finalizer* next): m_func(func), m_data(data), m_next(next) {}
        void operator()() { m_func(m_data); }
        void (*m_func)(void*);
        void* m_data;
        finalizer* m_next;
    };

    struct finalizer_array {
        finalizer_array(): m_head(MSGPACK_NULLPTR) {}

        ~finalizer_array() {
            clear();
        }

        void clear() {
            finalizer* fin = m_head;
            finalizer* tmp = MSGPACK_NULLPTR;
            while(fin) {
                (*fin)();
                tmp = fin;
                fin = fin->m_next;
                delete tmp;
            }
            m_head = MSGPACK_NULLPTR;
        }

        void push(void (*func)(void* data), void* data) {
            m_head = new finalizer(func, data, m_head);
        }

        void pop() {
            finalizer* n = m_head->m_next;
            delete m_head;
            m_head = n;
        }

        finalizer* m_head;
    private:
        finalizer_array(const finalizer_array&);
        finalizer_array& operator=(const finalizer_array&);
    };

    struct chunk {
        chunk* m_next;
    };

    struct chunk_list {
        chunk_list(size_t chunk_size, char* ptr): m_free(chunk_size), m_ptr(ptr), m_head(MSGPACK_NULLPTR) {}
        ~chunk_list() {
            chunk* c = m_head;
            while(c) {
                chunk* n = c->m_next;
                ::free(c);
                c = n;
            }
            m_head = MSGPACK_NULLPTR;
        }

        void clear(size_t chunk_size, char* ptr) {
            chunk* c = m_head;
            while(c) {
                chunk* n = c->m_next;
                ::free(c);
                c = n;
            }
            m_head = MSGPACK_NULLPTR;
            m_free = chunk_size;
            m_ptr  = ptr;
        }

        size_t m_free;
        char* m_ptr;
        chunk* m_head;

    private:
        chunk_list(const chunk_list&);
        chunk_list& operator=(const chunk_list&);
    };

    size_t m_chunk_size;
    chunk_list* m_chunk_list;
    finalizer_array m_finalizer_array;

public:
    zone(size_t chunk_size = MSGPACK_ZONE_CHUNK_SIZE);
    ~zone();

    void* allocate_align(size_t size, size_t align = MSGPACK_ZONE_ALIGN);

    void* allocate_no_align(size_t size);

    bool allocated() { 
        return m_chunk_list != MSGPACK_NULLPTR; 
    }

    void push_finalizer(void (*func)(void*), void* data);

    template <typename T>
    void push_finalizer(msgpack::unique_ptr<T> obj);

    void clear();

    void swap(zone& o);

    static void* operator new(std::size_t size) {
        void* p = ::malloc(size);
        if (!p) throw std::bad_alloc();
        return p;
    }

    static void operator delete(void *p) /* throw() */ {
        ::free(p);
    }

    static void* operator new(std::size_t /*size*/, void* mem) /* throw() */ {
        return mem;
    }

    static void operator delete(void * /*p*/, void* /*mem*/) /* throw() */ {}

    /// @cond
    
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
    
    /// @endcond

private:
    void undo_allocate(size_t size);

    template <typename T>
    static void object_destruct(void* obj);

    template <typename T>
    static void object_delete(void* obj);

    static char* get_aligned(char* ptr, size_t align);

    chunk_list& get_chank_lst();

    char* allocate_expand(size_t size);
private:
    zone(const zone&);
    zone& operator=(const zone&);
};

inline zone::zone(size_t chunk_size):m_chunk_size(chunk_size), m_chunk_list(MSGPACK_NULLPTR) {}

inline zone::~zone() {
    m_finalizer_array.~finalizer_array();
    if(m_chunk_list) {
        m_chunk_list->~chunk_list();
        ::free(m_chunk_list);
        m_chunk_list = MSGPACK_NULLPTR;
    }
}

inline char* zone::get_aligned(char* ptr, size_t align) {
    MSGPACK_ASSERT(align != 0 && (align & (align - 1)) == 0); // align must be 2^n (n >= 0)
    return
        reinterpret_cast<char*>(
            reinterpret_cast<uintptr_t>(ptr + (align - 1)) & ~static_cast<uintptr_t>(align - 1)
        );
}

inline zone::chunk_list& zone::get_chank_lst() {
    if (!m_chunk_list) {
        void* ptr = ::malloc(sizeof(chunk_list) + m_chunk_size);
        if (!ptr)
            throw std::bad_alloc();
        m_chunk_list = new (ptr) chunk_list(m_chunk_size, reinterpret_cast<char*>(ptr) + sizeof(chunk_list));
    }
    return *m_chunk_list;
}

inline void* zone::allocate_align(size_t size, size_t align) {
    chunk_list& chank_lst = get_chank_lst();
    char* aligned = get_aligned(chank_lst.m_ptr, align);
    size_t adjusted_size = size + static_cast<size_t>(aligned - chank_lst.m_ptr);
    if (chank_lst.m_free < adjusted_size) {
        size_t enough_size = size + align - 1;
        char* ptr = allocate_expand(enough_size);
        aligned = get_aligned(ptr, align);
        adjusted_size = size + static_cast<size_t>(aligned - chank_lst.m_ptr);
    }
    chank_lst.m_free -= adjusted_size;
    chank_lst.m_ptr += adjusted_size;
    return aligned;
}

inline void* zone::allocate_no_align(size_t size) {
    chunk_list& chank_lst = get_chank_lst();
    char* ptr = chank_lst.m_ptr;
    if(chank_lst.m_free < size) {
        ptr = allocate_expand(size);
    }
    chank_lst.m_free -= size;
    chank_lst.m_ptr  += size;

    return ptr;
}

inline char* zone::allocate_expand(size_t size) {
    chunk_list& cl = get_chank_lst();
    size_t sz = m_chunk_size;

    while(sz < size) {
        size_t tmp_sz = sz * 2;
        if (tmp_sz <= sz) {
            sz = size;
            break;
        }
        sz = tmp_sz;
    }

    chunk* c = static_cast<chunk*>(::malloc(sizeof(chunk) + sz));
    if (!c) throw std::bad_alloc();

    char* ptr = reinterpret_cast<char*>(c) + sizeof(chunk);

    c->m_next = cl.m_head;
    cl.m_head = c;
    cl.m_free = sz;
    cl.m_ptr  = ptr;

    return ptr;
}

inline void zone::push_finalizer(void (*func)(void*), void* data) {
    m_finalizer_array.push(func, data);
}

template <typename T>
inline void zone::push_finalizer(msgpack::unique_ptr<T> obj) {
    m_finalizer_array.push(&zone::object_delete<T>, obj.release());
}

inline void zone::clear() {
    m_finalizer_array.clear();
    if (m_chunk_list) {
        m_chunk_list->clear(m_chunk_size, reinterpret_cast<char*>(m_chunk_list) + sizeof(chunk_list));
    }
}

inline void zone::swap(zone& o) {
    using std::swap;
    swap(m_chunk_size, o.m_chunk_size);
    swap(m_chunk_list, o.m_chunk_list);
    swap(m_finalizer_array.m_head, o.m_finalizer_array.m_head);
}

template <typename T>
void zone::object_delete(void* obj) {
    delete static_cast<T*>(obj);
}

template <typename T>
void zone::object_destruct(void* obj) {
    static_cast<T*>(obj)->~T();
}

inline void zone::undo_allocate(size_t size) {
    chunk_list& cl = get_chank_lst();
    cl.m_ptr  -= size;
    cl.m_free += size;
}

inline std::size_t aligned_size(std::size_t size, std::size_t align) {
    return (size + align - 1) / align * align;
}

/// @cond

template <typename T>
T* zone::allocate()
{
    void* x = allocate_align(sizeof(T), MSGPACK_ZONE_ALIGNOF(T));
    try {
        m_finalizer_array.push(&zone::object_destruct<T>, x);
    } catch (...) {
        undo_allocate(sizeof(T));
        throw;
    }
    try {
        return new (x) T();
    } catch (...) {
        m_finalizer_array.pop();
        undo_allocate(sizeof(T));
        throw;
    }
}

template <typename T, typename A1>
T* zone::allocate(A1 a1)
{
    void* x = allocate_align(sizeof(T), MSGPACK_ZONE_ALIGNOF(T));
    try {
        m_finalizer_array.push(&zone::object_destruct<T>, x);
    } catch (...) {
        undo_allocate(sizeof(T));
        throw;
    }
    try {
        return new (x) T(a1);
    } catch (...) {
        m_finalizer_array.pop();
        undo_allocate(sizeof(T));
        throw;
    }
}

template <typename T, typename A1, typename A2>
T* zone::allocate(A1 a1, A2 a2)
{
    void* x = allocate_align(sizeof(T), MSGPACK_ZONE_ALIGNOF(T));
    try {
        m_finalizer_array.push(&zone::object_destruct<T>, x);
    } catch (...) {
        undo_allocate(sizeof(T));
        throw;
    }
    try {
        return new (x) T(a1, a2);
    } catch (...) {
        m_finalizer_array.pop();
        undo_allocate(sizeof(T));
        throw;
    }
}

template <typename T, typename A1, typename A2, typename A3>
T* zone::allocate(A1 a1, A2 a2, A3 a3)
{
    void* x = allocate_align(sizeof(T), MSGPACK_ZONE_ALIGNOF(T));
    try {
        m_finalizer_array.push(&zone::object_destruct<T>, x);
    } catch (...) {
        undo_allocate(sizeof(T));
        throw;
    }
    try {
        return new (x) T(a1, a2, a3);
    } catch (...) {
        m_finalizer_array.pop();
        undo_allocate(sizeof(T));
        throw;
    }
}

template <typename T, typename A1, typename A2, typename A3, typename A4>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4)
{
    void* x = allocate_align(sizeof(T), MSGPACK_ZONE_ALIGNOF(T));
    try {
        m_finalizer_array.push(&zone::object_destruct<T>, x);
    } catch (...) {
        undo_allocate(sizeof(T));
        throw;
    }
    try {
        return new (x) T(a1, a2, a3, a4);
    } catch (...) {
        m_finalizer_array.pop();
        undo_allocate(sizeof(T));
        throw;
    }
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
    void* x = allocate_align(sizeof(T), MSGPACK_ZONE_ALIGNOF(T));
    try {
        m_finalizer_array.push(&zone::object_destruct<T>, x);
    } catch (...) {
        undo_allocate(sizeof(T));
        throw;
    }
    try {
        return new (x) T(a1, a2, a3, a4, a5);
    } catch (...) {
        m_finalizer_array.pop();
        undo_allocate(sizeof(T));
        throw;
    }
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
{
    void* x = allocate_align(sizeof(T), MSGPACK_ZONE_ALIGNOF(T));
    try {
        m_finalizer_array.push(&zone::object_destruct<T>, x);
    } catch (...) {
        undo_allocate(sizeof(T));
        throw;
    }
    try {
        return new (x) T(a1, a2, a3, a4, a5, a6);
    } catch (...) {
        m_finalizer_array.pop();
        undo_allocate(sizeof(T));
        throw;
    }
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
{
    void* x = allocate_align(sizeof(T), MSGPACK_ZONE_ALIGNOF(T));
    try {
        m_finalizer_array.push(&zone::object_destruct<T>, x);
    } catch (...) {
        undo_allocate(sizeof(T));
        throw;
    }
    try {
        return new (x) T(a1, a2, a3, a4, a5, a6, a7);
    } catch (...) {
        m_finalizer_array.pop();
        undo_allocate(sizeof(T));
        throw;
    }
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
{
    void* x = allocate_align(sizeof(T), MSGPACK_ZONE_ALIGNOF(T));
    try {
        m_finalizer_array.push(&zone::object_destruct<T>, x);
    } catch (...) {
        undo_allocate(sizeof(T));
        throw;
    }
    try {
        return new (x) T(a1, a2, a3, a4, a5, a6, a7, a8);
    } catch (...) {
        m_finalizer_array.pop();
        undo_allocate(sizeof(T));
        throw;
    }
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
{
    void* x = allocate_align(sizeof(T), MSGPACK_ZONE_ALIGNOF(T));
    try {
        m_finalizer_array.push(&zone::object_destruct<T>, x);
    } catch (...) {
        undo_allocate(sizeof(T));
        throw;
    }
    try {
        return new (x) T(a1, a2, a3, a4, a5, a6, a7, a8, a9);
    } catch (...) {
        m_finalizer_array.pop();
        undo_allocate(sizeof(T));
        throw;
    }
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10)
{
    void* x = allocate_align(sizeof(T), MSGPACK_ZONE_ALIGNOF(T));
    try {
        m_finalizer_array.push(&zone::object_destruct<T>, x);
    } catch (...) {
        undo_allocate(sizeof(T));
        throw;
    }
    try {
        return new (x) T(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
    } catch (...) {
        m_finalizer_array.pop();
        undo_allocate(sizeof(T));
        throw;
    }
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11)
{
    void* x = allocate_align(sizeof(T), MSGPACK_ZONE_ALIGNOF(T));
    try {
        m_finalizer_array.push(&zone::object_destruct<T>, x);
    } catch (...) {
        undo_allocate(sizeof(T));
        throw;
    }
    try {
        return new (x) T(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
    } catch (...) {
        m_finalizer_array.pop();
        undo_allocate(sizeof(T));
        throw;
    }
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12)
{
    void* x = allocate_align(sizeof(T), MSGPACK_ZONE_ALIGNOF(T));
    try {
        m_finalizer_array.push(&zone::object_destruct<T>, x);
    } catch (...) {
        undo_allocate(sizeof(T));
        throw;
    }
    try {
        return new (x) T(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
    } catch (...) {
        m_finalizer_array.pop();
        undo_allocate(sizeof(T));
        throw;
    }
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13)
{
    void* x = allocate_align(sizeof(T), MSGPACK_ZONE_ALIGNOF(T));
    try {
        m_finalizer_array.push(&zone::object_destruct<T>, x);
    } catch (...) {
        undo_allocate(sizeof(T));
        throw;
    }
    try {
        return new (x) T(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
    } catch (...) {
        m_finalizer_array.pop();
        undo_allocate(sizeof(T));
        throw;
    }
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14)
{
    void* x = allocate_align(sizeof(T), MSGPACK_ZONE_ALIGNOF(T));
    try {
        m_finalizer_array.push(&zone::object_destruct<T>, x);
    } catch (...) {
        undo_allocate(sizeof(T));
        throw;
    }
    try {
        return new (x) T(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
    } catch (...) {
        m_finalizer_array.pop();
        undo_allocate(sizeof(T));
        throw;
    }
}

template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
T* zone::allocate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15)
{
    void* x = allocate_align(sizeof(T), MSGPACK_ZONE_ALIGNOF(T));
    try {
        m_finalizer_array.push(&zone::object_destruct<T>, x);
    } catch (...) {
        undo_allocate(sizeof(T));
        throw;
    }
    try {
        return new (x) T(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
    } catch (...) {
        m_finalizer_array.pop();
        undo_allocate(sizeof(T));
        throw;
    }
}

/// @endcond

/// @cond
}  // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

}  // namespace msgpack

#endif // MSGPACK_V1_CPP03_ZONE_HPP
