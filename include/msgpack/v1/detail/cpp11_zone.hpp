//
// MessagePack for C++ memory pool
//
// Copyright (C) 2008-2013 FURUHASHI Sadayuki and KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_CPP11_ZONE_HPP
#define MSGPACK_CPP11_ZONE_HPP

#include "msgpack/versioning.hpp"
#include "msgpack/cpp_config.hpp"
#include "msgpack/zone_decl.hpp"
#include "msgpack/assert.hpp"

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <vector>

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond

class zone {
private:
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
            auto n = m_head->m_next;
            delete m_head;
            m_head = n;
        }

        finalizer_array(finalizer_array&& other) noexcept: m_head(other.m_head) {
            other.m_head = MSGPACK_NULLPTR;
        }

        finalizer_array& operator=(finalizer_array&& other) noexcept {
            m_head = other.m_head;
            other.m_head = MSGPACK_NULLPTR;
            return *this;
        }

    private:
        finalizer* m_head;
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
        chunk_list(chunk_list&& other) noexcept = delete;
        chunk_list& operator=(chunk_list&& other) noexcept = delete;
        chunk_list(const chunk_list&);
        chunk_list& operator=(const chunk_list&);
    };

    size_t m_chunk_size;
    chunk_list* m_chunk_list{};
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

    static void operator delete(void *p) noexcept {
        ::free(p);
    }

    static void* operator new(std::size_t /*size*/, void* mem) noexcept {
        return mem;
    }

    static void operator delete(void * /*p*/, void* /*mem*/) noexcept {}

    template <typename T, typename... Args>
    T* allocate(Args... args);

    zone(zone&&) = default;
    zone& operator=(zone&&) = default;
    zone(const zone&) = delete;
    zone& operator=(const zone&) = delete;

private:
    void undo_allocate(size_t size);

    template <typename T>
    static void object_destruct(void* obj);

    template <typename T>
    static void object_delete(void* obj);

    static char* get_aligned(char* ptr, size_t align);

    chunk_list& get_chank_lst();

    char* allocate_expand(size_t size);
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
        auto ptr = ::malloc(sizeof(chunk_list) + m_chunk_size);
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
    std::swap(*this, o);
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

template <typename T, typename... Args>
T* zone::allocate(Args... args) {
    void* x = allocate_align(sizeof(T), MSGPACK_ZONE_ALIGNOF(T));
    try {
        m_finalizer_array.push(&zone::object_destruct<T>, x);
    } catch (...) {
        undo_allocate(sizeof(T));
        throw;
    }
    try {
        return new (x) T(args...);
    } catch (...) {
        m_finalizer_array.pop();
        undo_allocate(sizeof(T));
        throw;
    }
}

/// @cond
}  // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

}  // namespace msgpack

#endif // MSGPACK_CPP11_ZONE_HPP
