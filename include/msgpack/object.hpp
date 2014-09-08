//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2008-2010 FURUHASHI Sadayuki
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
#ifndef MSGPACK_OBJECT_HPP
#define MSGPACK_OBJECT_HPP

#include "object.h"
#include "pack.hpp"
#include "zone.hpp"
#include <string.h>
#include <stdexcept>
#include <typeinfo>
#include <limits>
#include <ostream>

namespace msgpack {


class type_error : public std::bad_cast { };


namespace type {
    enum object_type {
        NIL                 = MSGPACK_OBJECT_NIL,
        BOOLEAN             = MSGPACK_OBJECT_BOOLEAN,
        POSITIVE_INTEGER    = MSGPACK_OBJECT_POSITIVE_INTEGER,
        NEGATIVE_INTEGER    = MSGPACK_OBJECT_NEGATIVE_INTEGER,
        DOUBLE              = MSGPACK_OBJECT_DOUBLE,
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
    object* ptr;
};

struct object_map {
    uint32_t size;
    object_kv* ptr;
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
        double   dec;
        object_array array;
        object_map map;
        object_str str;
        object_bin bin;
        object_ext ext;
    };

    type::object_type type;
    union_type via;

    bool is_nil() const { return type == type::NIL; }

    template <typename T>
    T as() const;

    template <typename T>
    void convert(T& v) const;
    template <typename T>
    void convert(T* v) const;

    object();

    object(msgpack_object o);

    template <typename T>
    explicit object(const T& v);

    template <typename T>
    object(const T& v, zone& z);

    // obsolete
    template <typename T>
    object(const T& v, zone* z);

    template <typename T>
    object& operator=(const T& v);

    operator msgpack_object() const;

    struct with_zone;

private:
    struct implicit_type;

public:
    implicit_type convert() const;
};

struct object_kv {
    object key;
    object val;
};

struct object::with_zone : object {
    with_zone(msgpack::zone& zone) : zone(zone) { }
    msgpack::zone& zone;
private:
    with_zone();
};

struct object::implicit_type {
    implicit_type(object const& o) : obj(o) { }
    ~implicit_type() { }

    template <typename T>
    operator T() { return obj.as<T>(); }

private:
    object const& obj;
};


// obsolete
template <typename Type>
class define : public Type {
public:
    typedef Type msgpack_type;
    typedef define<Type> define_type;

    define() {}
    define(const msgpack_type& v) : msgpack_type(v) {}

    template <typename Packer>
    void msgpack_pack(Packer& o) const
    {
        o << static_cast<const msgpack_type&>(*this);
    }

    void msgpack_unpack(object const& o)
    {
        o >> static_cast<msgpack_type&>(*this);
    }
};


template <typename Stream>
template <typename T>
inline packer<Stream>& packer<Stream>::pack(const T& v)
{
    *this << v;
    return *this;
}

inline object const& operator>> (object const& o, object& v)
{
    v = o;
    return o;
}

// convert operator
template <typename T>
inline object const& operator>> (object const& o, T& v)
{
    v.msgpack_unpack(o.convert());
    return o;
}

namespace detail {
template <typename Stream, typename T>
struct packer_serializer {
    static packer<Stream>& pack(packer<Stream>& o, const T& v) {
        v.msgpack_pack(o);
        return o;
    }
};
}

// serialize operator
template <typename Stream, typename T>
inline packer<Stream>& operator<< (packer<Stream>& o, const T& v)
{
    return detail::packer_serializer<Stream, T>::pack(o, v);
}

// deconvert operator
template <typename T>
inline void operator<< (object::with_zone& o, const T& v)
{
    v.msgpack_object(static_cast<object*>(&o), o.zone);
}


inline bool operator==(const object& x, const object& y)
{
    if(x.type != y.type) { return false; }

    switch(x.type) {
    case type::NIL:
        return true;

    case type::BOOLEAN:
        return x.via.boolean == y.via.boolean;

    case type::POSITIVE_INTEGER:
        return x.via.u64 == y.via.u64;

    case type::NEGATIVE_INTEGER:
        return x.via.i64 == y.via.i64;

    case type::DOUBLE:
        return x.via.dec == y.via.dec;

    case type::STR:
        return x.via.str.size == y.via.str.size &&
            memcmp(x.via.str.ptr, y.via.str.ptr, x.via.str.size) == 0;

    case type::BIN:
        return x.via.bin.size == y.via.bin.size &&
            memcmp(x.via.bin.ptr, y.via.bin.ptr, x.via.bin.size) == 0;

    case type::EXT:
        return x.via.ext.size == y.via.ext.size &&
            memcmp(x.via.ext.ptr, y.via.ext.ptr, x.via.ext.size) == 0;

    case type::ARRAY:
        if(x.via.array.size != y.via.array.size) {
            return false;
        } else if(x.via.array.size == 0) {
            return true;
        } else {
            object* px = x.via.array.ptr;
            object* const pxend = x.via.array.ptr + x.via.array.size;
            object* py = y.via.array.ptr;
            do {
                if(!(*px == *py)) {
                    return false;
                }
                ++px;
                ++py;
            } while(px < pxend);
            return true;
        }

    case type::MAP:
        if(x.via.map.size != y.via.map.size) {
            return false;
        } else if(x.via.map.size == 0) {
            return true;
        } else {
            object_kv* px = x.via.map.ptr;
            object_kv* const pxend = x.via.map.ptr + x.via.map.size;
            object_kv* py = y.via.map.ptr;
            do {
                if(!(px->key == py->key) || !(px->val == py->val)) {
                    return false;
                }
                ++px;
                ++py;
            } while(px < pxend);
            return true;
        }

    default:
        return false;
    }
}

template <typename T>
inline bool operator==(const object& x, const T& y)
try {
    return x == object(y);
} catch (msgpack::type_error&) {
    return false;
}

inline bool operator!=(const object& x, const object& y)
{ return !(x == y); }

template <typename T>
inline bool operator==(const T& y, const object x)
{ return x == y; }

template <typename T>
inline bool operator!=(const object& x, const T& y)
{ return !(x == y); }

template <typename T>
inline bool operator!=(const T& y, const object& x)
{ return x != y; }


inline object::implicit_type object::convert() const
{
    return implicit_type(*this);
}

template <typename T>
inline void object::convert(T& v) const
{
    *this >> v;
}

template <typename T>
inline void object::convert(T* v) const
{
    convert(*v);
}

template <typename T>
inline T object::as() const
{
    T v;
    convert(v);
    return v;
}


inline object::object()
{
    type = type::NIL;
}

template <typename T>
inline object::object(const T& v)
{
    *this << v;
}

template <typename T>
inline object& object::operator=(const T& v)
{
    *this = object(v);
    return *this;
}

template <typename T>
object::object(const T& v, zone& z)
{
    with_zone oz(z);
    oz << v;
    type = oz.type;
    via = oz.via;
}

template <typename T>
object::object(const T& v, zone* z)
{
    with_zone oz(*z);
    oz << v;
    type = oz.type;
    via = oz.via;
}


inline object::object(msgpack_object o)
{
    // FIXME beter way?
    ::memcpy(this, &o, sizeof(o));
}

inline void operator<< (object& o, msgpack_object v)
{
    // FIXME beter way?
    ::memcpy(&o, &v, sizeof(v));
}

inline object::operator msgpack_object() const
{
    // FIXME beter way?
    msgpack_object obj;
    ::memcpy(&obj, this, sizeof(obj));
    return obj;
}


// obsolete
template <typename T>
inline void convert(T& v, object const& o)
{
    o.convert(v);
}

// obsolete
template <typename Stream, typename T>
inline void pack(packer<Stream>& o, const T& v)
{
    o.pack(v);
}

// obsolete
template <typename Stream, typename T>
inline void pack_copy(packer<Stream>& o, T v)
{
    pack(o, v);
}


template <typename Stream>
packer<Stream>& operator<< (packer<Stream>& o, const object& v)
{
    switch(v.type) {
    case type::NIL:
        o.pack_nil();
        return o;

    case type::BOOLEAN:
        if(v.via.boolean) {
            o.pack_true();
        } else {
            o.pack_false();
        }
        return o;

    case type::POSITIVE_INTEGER:
        o.pack_uint64(v.via.u64);
        return o;

    case type::NEGATIVE_INTEGER:
        o.pack_int64(v.via.i64);
        return o;

    case type::DOUBLE:
        o.pack_double(v.via.dec);
        return o;

    case type::STR:
        o.pack_str(v.via.str.size);
        o.pack_str_body(v.via.str.ptr, v.via.str.size);
        return o;

    case type::BIN:
        o.pack_bin(v.via.bin.size);
        o.pack_bin_body(v.via.bin.ptr, v.via.bin.size);
        return o;

    case type::EXT:
        o.pack_ext(v.via.ext.size, v.via.ext.type());
        o.pack_ext_body(v.via.ext.data(), v.via.ext.size);
        return o;

    case type::ARRAY:
        o.pack_array(v.via.array.size);
        for(object* p(v.via.array.ptr),
                * const pend(v.via.array.ptr + v.via.array.size);
                p < pend; ++p) {
            o << *p;
        }
        return o;

    case type::MAP:
        o.pack_map(v.via.map.size);
        for(object_kv* p(v.via.map.ptr),
                * const pend(v.via.map.ptr + v.via.map.size);
                p < pend; ++p) {
            o << p->key;
            o << p->val;
        }
        return o;

    default:
        throw type_error();
    }
}

inline std::ostream& operator<< (std::ostream& s, const object& o)
{
    switch(o.type) {
    case type::NIL:
        s << "nil";
        break;

    case type::BOOLEAN:
        s << (o.via.boolean ? "true" : "false");
        break;

    case type::POSITIVE_INTEGER:
        s << o.via.u64;
        break;

    case type::NEGATIVE_INTEGER:
        s << o.via.i64;
        break;

    case type::DOUBLE:
        s << o.via.dec;
        break;

    case type::STR:
        (s << '"').write(o.via.str.ptr, o.via.str.size) << '"';
        break;

    case type::BIN:
        (s << '"').write(o.via.bin.ptr, o.via.bin.size) << '"';
        break;

    case type::EXT:
        s << "EXT";

    case type::ARRAY:
        s << "[";
        if(o.via.array.size != 0) {
            object* p(o.via.array.ptr);
            s << *p;
            ++p;
            for(object* const pend(o.via.array.ptr + o.via.array.size);
                    p < pend; ++p) {
                s << ", " << *p;
            }
        }
        s << "]";
        break;

    case type::MAP:
        s << "{";
        if(o.via.map.size != 0) {
            object_kv* p(o.via.map.ptr);
            s << p->key << "=>" << p->val;
            ++p;
            for(object_kv* const pend(o.via.map.ptr + o.via.map.size);
                    p < pend; ++p) {
                s << ", " << p->key << "=>" << p->val;
            }
        }
        s << "}";
        break;

    default:
        // FIXME
        s << "#<UNKNOWN " << static_cast<uint16_t>(o.type) << ">";
    }
    return s;
}

}  // namespace msgpack

#include "msgpack/type.hpp"

#endif /* msgpack/object.hpp */
