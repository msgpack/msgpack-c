//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2008-2014 FURUHASHI Sadayuki and KONDO Takatoshi
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

#ifndef MSGPACK_OBJECT_FWD_HPP
#define MSGPACK_OBJECT_FWD_HPP

#include "msgpack/versioning.hpp"
#include "msgpack/zone.hpp"
#include "msgpack/object.h"

#include <typeinfo>

namespace msgpack {

MSGPACK_API_VERSION_NAMESPACE(v1) {


namespace type {
    enum object_type {
        NIL                 = MSGPACK_OBJECT_NIL,
        BOOLEAN             = MSGPACK_OBJECT_BOOLEAN,
        POSITIVE_INTEGER    = MSGPACK_OBJECT_POSITIVE_INTEGER,
        NEGATIVE_INTEGER    = MSGPACK_OBJECT_NEGATIVE_INTEGER,
        FLOAT               = MSGPACK_OBJECT_FLOAT,
#if defined(MSGPACK_USE_LEGACY_NAME_AS_FLOAT)
        DOUBLE              = MSGPACK_OBJECT_DOUBLE, // obsolete
#endif // MSGPACK_USE_LEGACY_NAME_AS_FLOAT
        STR                 = MSGPACK_OBJECT_STR,
        BIN                 = MSGPACK_OBJECT_BIN,
        ARRAY               = MSGPACK_OBJECT_ARRAY,
        MAP                 = MSGPACK_OBJECT_MAP,
        EXT                 = MSGPACK_OBJECT_EXT
    };
}


struct object;
struct object_kv;

struct object_array {
    uint32_t size;
    msgpack::object* ptr;
};

struct object_map {
    uint32_t size;
    msgpack::object_kv* ptr;
};

struct object_str {
    uint32_t size;
    const char* ptr;
};

struct object_bin {
    uint32_t size;
    const char* ptr;
};

struct object_ext {
    int8_t type() const { return ptr[0]; }
    const char* data() const { return &ptr[1]; }
    uint32_t size;
    const char* ptr;
};

struct object {
    union union_type {
        bool boolean;
        uint64_t u64;
        int64_t  i64;
#if defined(MSGPACK_USE_LEGACY_NAME_AS_FLOAT)
        double   dec; // obsolete
#endif // MSGPACK_USE_LEGACY_NAME_AS_FLOAT
        double   f64;
        msgpack::object_array array;
        msgpack::object_map map;
        msgpack::object_str str;
        msgpack::object_bin bin;
        msgpack::object_ext ext;
    };

    msgpack::type::object_type type;
    union_type via;

    bool is_nil() const { return type == msgpack::type::NIL; }

    template <typename T>
    T as() const;

    template <typename T>
    void convert(T& v) const;
    template <typename T>
    void convert(T* v) const;

    object();

    object(const msgpack_object& o);

    template <typename T>
    explicit object(const T& v);

    template <typename T>
    object(const T& v, msgpack::zone& z);

    // obsolete
    template <typename T>
    object(const T& v, msgpack::zone* z);

    template <typename T>
    object& operator=(const T& v);

    operator msgpack_object() const;

    struct with_zone;

private:
    struct implicit_type;

public:
    implicit_type convert() const;
};

class type_error : public std::bad_cast { };

struct object_kv {
    msgpack::object key;
    msgpack::object val;
};

namespace detail {
template <typename Stream, typename T>
struct packer_serializer;
} // namespace detail

msgpack::object const& operator>> (msgpack::object const& o, msgpack::object& v);

template <typename T>
msgpack::object const& operator>> (msgpack::object const& o, T& v);

template <typename T>
void operator<< (msgpack::object::with_zone& o, const T& v);

void operator<< (msgpack::object::with_zone& o, const msgpack::object& v);

void operator<< (msgpack::object::with_zone& o, const msgpack::object::with_zone& v);

template <typename Stream>
class packer;

template <typename Stream>
msgpack::packer<Stream>& operator<< (msgpack::packer<Stream>& o, const msgpack::object& v);

template <typename Stream>
msgpack::packer<Stream>& operator<< (msgpack::packer<Stream>& o, const msgpack::object::with_zone& v);

template <typename Stream, typename T>
msgpack::packer<Stream>& operator<< (msgpack::packer<Stream>& o, const T& v);

template <typename T>
void operator<< (msgpack::object::with_zone& o, const T& v);

} // MSGPACK_API_VERSION_NAMESPACE(v1)

} // namespace msgpack

#endif // MSGPACK_OBJECT_FWD_HPP
