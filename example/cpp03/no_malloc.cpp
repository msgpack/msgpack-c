// MessagePack for C++ example#include <memory>

//
// Copyright (C) 2019 KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <new>

// You can define your own allocator here

#define MSGPACK_ALLOC_SIZE_MAX  1024*1024
#define MSGPACK_ALLOC_COUNT_MAX 1024

static char g_buf[MSGPACK_ALLOC_COUNT_MAX][MSGPACK_ALLOC_SIZE_MAX];
static size_t g_index = 0;

void* my_malloc(size_t size) {
    if (g_index == MSGPACK_ALLOC_COUNT_MAX) return NULL;
    if (size > MSGPACK_ALLOC_SIZE_MAX) return NULL;
    return &g_buf[g_index++][0];
}

void* my_realloc(void* p, size_t size) {
    if (p == NULL) return my_malloc(size);
    if (size > MSGPACK_ALLOC_SIZE_MAX) return NULL;
    return p;
}

void my_free(void* p) {
    (void)p;
}

void* operator new(size_t size)
#if __cplusplus < 201103L && !defined(_MSC_VER)
    throw(std::bad_alloc)
#endif // __cplusplus < 201103L
{
    void* p = my_malloc(size);
    if (!p) throw std::bad_alloc();
    return p;
}

void* operator new[](size_t size)
#if __cplusplus < 201103L && !defined(_MSC_VER)
    throw(std::bad_alloc)
#endif // __cplusplus < 201103L
{
    void* p = my_malloc(size);
    if (!p) throw std::bad_alloc();
    return p;
}

void operator delete(void* /*p*/)
#if __cplusplus < 201103L
    throw()
#else  // __cplusplus < 201103L
    noexcept
#endif // __cplusplus < 201103L
{
}

#if __cplusplus >= 201402L
void operator delete(void* /*p*/, size_t)
    noexcept
{
}
#endif // __cplusplus >= 201402L

// Define the following three macros before include msgpack.hpp

#define MSGPACK_MALLOC(size) my_malloc(size)
#define MSGPACK_REALLOC(p, size) my_realloc(p, size)
#define MSGPACK_FREE(p) my_free(p)


#include <msgpack.hpp>
#include <string>
#include <iostream>
#include <sstream>

int main(void)
{
    msgpack::type::tuple<int, bool, std::string> src(1, true, "example");

    // serialize the object into the buffer.
    // any classes that implements write(const char*,size_t) can be a buffer.
    std::stringstream buffer;
    msgpack::pack(buffer, src);

    // send the buffer ...
    buffer.seekg(0);

    // deserialize the buffer into msgpack::object instance.
    std::string str(buffer.str());

    msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());

    // deserialized object is valid during the msgpack::object_handle instance alive.
    msgpack::object deserialized = oh.get();

    // msgpack::object supports ostream.
    std::cout << deserialized << std::endl;

    // convert msgpack::object instance into the original type.
    // if the type is mismatched, it throws msgpack::type_error exception.
    msgpack::type::tuple<int, bool, std::string> dst;
    deserialized.convert(dst);

    return 0;
}
