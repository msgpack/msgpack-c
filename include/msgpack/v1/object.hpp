//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2008-2014 FURUHASHI Sadayuki and KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_V1_OBJECT_HPP
#define MSGPACK_V1_OBJECT_HPP

#include "msgpack/object_decl.hpp"
#include "msgpack/adaptor/check_container_size.hpp"

#include <cstring>
#include <stdexcept>
#include <typeinfo>
#include <limits>
#include <ostream>
#include <typeinfo>
#include <iomanip>

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond

struct object_kv {
    msgpack::object key;
    msgpack::object val;
};

struct object::with_zone : msgpack::object {
    with_zone(msgpack::zone& z) : zone(z) { }
    msgpack::zone& zone;
private:
    with_zone();
};


/// The class holds object and zone
class object_handle {
public:
    /// Constructor that creates nil object and null zone.
    object_handle() {}

    /// Constructor that creates an object_handle holding object `obj` and zone `z`.
    /**
     * @param obj object
     * @param z zone
     */
    object_handle(
        msgpack::object const& obj,
#if defined(MSGPACK_USE_CPP03)
        msgpack::unique_ptr<msgpack::zone> z
#else  // defined(MSGPACK_USE_CPP03)
        msgpack::unique_ptr<msgpack::zone>&& z
#endif // defined(MSGPACK_USE_CPP03)
    ) :
        m_obj(obj), m_zone(msgpack::move(z)) { }

    void set(msgpack::object const& obj)
        { m_obj = obj; }

    /// Get object reference
    /**
     * @return object
     */
    const msgpack::object& get() const
        { return m_obj; }

    /**
     * @return object (to mimic smart pointers).
     */
    const msgpack::object& operator*() const
        { return get(); }

    /**
     * @return the address of the object (to mimic smart pointers).
     */
    const msgpack::object* operator->() const
        { return &get(); }

    /// Get unique_ptr reference of zone.
    /**
     * @return unique_ptr reference of zone
     */
    msgpack::unique_ptr<msgpack::zone>& zone()
        { return m_zone; }

    /// Get unique_ptr const reference of zone.
    /**
     * @return unique_ptr const reference of zone
     */
    const msgpack::unique_ptr<msgpack::zone>& zone() const
        { return m_zone; }

#if defined(MSGPACK_USE_CPP03)
    struct object_handle_ref {
        object_handle_ref(object_handle* oh):m_oh(oh) {}
        object_handle* m_oh;
    };

    object_handle(object_handle& other):
        m_obj(other.m_obj),
        m_zone(msgpack::move(other.m_zone)) {
    }

    object_handle(object_handle_ref ref):
        m_obj(ref.m_oh->m_obj),
        m_zone(msgpack::move(ref.m_oh->m_zone)) {
    }

    object_handle& operator=(object_handle& other) {
        m_obj = other.m_obj;
        m_zone = msgpack::move(other.m_zone);
        return *this;
    }

    object_handle& operator=(object_handle_ref ref) {
        m_obj = ref.m_oh->m_obj;
        m_zone = msgpack::move(ref.m_oh->m_zone);
        return *this;
    }

    operator object_handle_ref() {
        return object_handle_ref(this);
    }
#endif // defined(MSGPACK_USE_CPP03)

private:
    msgpack::object m_obj;
    msgpack::unique_ptr<msgpack::zone> m_zone;
};

namespace detail {

template <std::size_t N>
inline std::size_t add_ext_type_size(std::size_t size) {
    return size + 1;
}

template <>
inline std::size_t add_ext_type_size<4>(std::size_t size) {
    return size == 0xffffffff ? size : size + 1;
}

} // namespace detail
class object_parser {
private:
    struct elem {
        elem(msgpack::object const* p, std::size_t r)
            : rest(r), is_map(false), is_key(false) {
            as.obj_ptr = p;
        }

        elem(msgpack::object_kv const* p, std::size_t r)
            : rest(r), is_map(true), is_key(true) {
            as.kv_ptr = p;
        }

        msgpack::object const& get() const {
            if (is_map) {
                if (is_key) {
                    return as.kv_ptr->key;
                }
                else {
                    return as.kv_ptr->val;
                }
            }
            else {
                return *as.obj_ptr;
            }
        }

        template <typename Visitor>
        bool next(Visitor& v) {
            if (rest == 0) {
                if (is_map) {
                    v.end_map();
                }
                else {
                    v.end_array();
                }
                return false;
            }
            else {
                if (is_map) {
                    if (is_key) {
                        v.end_map_key();
                        v.start_map_value();
                        is_key = false;
                    }
                    else {
                        v.end_map_value();
                        --rest;
                        if (rest == 0) {
                            v.end_map();
                            return false;
                        }
                        v.start_map_key();
                        ++as.kv_ptr;
                        is_key = true;
                    }
                }
                else {
                    v.end_array_item();
                    --rest;
                    if (rest == 0) {
                        v.end_array();
                        return false;
                    }
                    ++as.obj_ptr;
                }
                return true;
            }
        }

        union {
            msgpack::object const* obj_ptr;
            msgpack::object_kv const* kv_ptr;
        } as;
        std::size_t rest;
        bool is_map;
        bool is_key;
    };
public:
    object_parser(msgpack::object const& obj):m_current(&obj) {}
    template <typename Visitor>
    void parse(Visitor& v) {
        while (true) {
            bool start_collection = false;
            switch(m_current->type) {
            case msgpack::type::NIL:
                v.visit_nil();
                break;
            case msgpack::type::BOOLEAN:
                v.visit_boolean(m_current->via.boolean);
                break;
            case msgpack::type::POSITIVE_INTEGER:
                v.visit_positive_integer(m_current->via.u64);
                break;
            case msgpack::type::NEGATIVE_INTEGER:
                v.visit_negative_integer(m_current->via.i64);
                break;
            case msgpack::type::FLOAT32:
                v.visit_float32(static_cast<float>(m_current->via.f64));
                break;
            case msgpack::type::FLOAT64:
                v.visit_float64(m_current->via.f64);
                break;
            case msgpack::type::STR:
                v.visit_str(m_current->via.str.ptr, m_current->via.str.size);
                break;
            case msgpack::type::BIN:
                v.visit_bin(m_current->via.bin.ptr, m_current->via.bin.size);
                break;
            case msgpack::type::EXT:
                msgpack::detail::check_container_size<sizeof(std::size_t)>(m_current->via.ext.size);
                v.visit_ext(m_current->via.ext.ptr, m_current->via.ext.size + 1);
                break;
            case msgpack::type::ARRAY:
                v.start_array(m_current->via.array.size);
                m_ctx.push_back(elem(m_current->via.array.ptr, m_current->via.array.size));
                start_collection = m_current->via.array.size != 0;
                break;
            case msgpack::type::MAP:
                v.start_map(m_current->via.map.size);
                m_ctx.push_back(elem(m_current->via.map.ptr, m_current->via.map.size));
                start_collection = m_current->via.map.size != 0;
                break;
            default:
                throw msgpack::type_error();
                break;
            }
            if (m_ctx.empty()) return;
            if (!start_collection) {
                while (!m_ctx.back().next(v)) {
                    m_ctx.pop_back();
                    if (m_ctx.empty()) return;
                }
            }
            m_current = &m_ctx.back().get();
        }
    }
private:
    msgpack::object const* m_current;
    std::vector<elem> m_ctx;
};

template <typename Stream>
struct object_pack_visitor {
    object_pack_visitor(msgpack::packer<Stream>& pk)
        :m_packer(pk) {}
    bool visit_nil() {
        m_packer.pack_nil();
        return true;
    }
    bool visit_boolean(bool v) {
        if (v) m_packer.pack_true();
        else m_packer.pack_false();
        return true;
    }
    bool visit_positive_integer(uint64_t v) {
        m_packer.pack_uint64(v);
        return true;
    }
    bool visit_negative_integer(int64_t v) {
        m_packer.pack_int64(v);
        return true;
    }
    bool visit_float32(float v) {
        m_packer.pack_float(v);
        return true;
    }
    bool visit_float64(double v) {
        m_packer.pack_double(v);
        return true;
    }
    bool visit_str(const char* v, uint32_t size) {
        m_packer.pack_str(size);
        m_packer.pack_str_body(v, size);
        return true;
    }
    bool visit_bin(const char* v, uint32_t size) {
        m_packer.pack_bin(size);
        m_packer.pack_bin_body(v, size);
        return true;
    }
    bool visit_ext(const char* v, uint32_t size) {
        m_packer.pack_ext(size, *v);
        m_packer.pack_ext_body(v, size);
        return true;
    }
    bool start_array(uint32_t num_elements) {
        m_packer.pack_array(num_elements);
        return true;
    }
    bool start_array_item() {
        return true;
    }
    bool end_array_item() {
        return true;
    }
    bool end_array() {
        return true;
    }
    bool start_map(uint32_t num_kv_pairs) {
        m_packer.pack_array(num_kv_pairs);
        return true;
    }
    bool start_map_key() {
        return true;
    }
    bool end_map_key() {
        return true;
    }
    bool start_map_value() {
        return true;
    }
    bool end_map_value() {
        return true;
    }
    bool end_map() {
        return true;
    }
private:
    msgpack::packer<Stream>& m_packer;
};


struct object_stringize_visitor {
    object_stringize_visitor(std::ostream& os)
        :m_os(os) {}
    bool visit_nil() {
        m_os << "null";
        return true;
    }
    bool visit_boolean(bool v) {
        if (v) m_os << "true";
        else m_os << "false";
        return true;
    }
    bool visit_positive_integer(uint64_t v) {
        m_os << v;
        return true;
    }
    bool visit_negative_integer(int64_t v) {
        m_os << v;
        return true;
    }
    bool visit_float32(float v) {
        m_os << v;
        return true;
    }
    bool visit_float64(double v) {
        m_os << v;
        return true;
    }
    bool visit_str(const char* v, uint32_t size) {
        m_os << '"';
        for (uint32_t i = 0; i < size; ++i) {
            char c = v[i];
            switch (c) {
            case '\\':
                m_os << "\\\\";
                break;
            case '"':
                m_os << "\\\"";
                break;
            case '/':
                m_os << "\\/";
                break;
            case '\b':
                m_os << "\\b";
                break;
            case '\f':
                m_os << "\\f";
                break;
            case '\n':
                m_os << "\\n";
                break;
            case '\r':
                m_os << "\\r";
                break;
            case '\t':
                m_os << "\\t";
                break;
            default: {
                unsigned int code = static_cast<unsigned int>(c);
                if (code < 0x20 || code == 0x7f) {
                    std::ios::fmtflags flags(m_os.flags());
                    m_os << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (code & 0xff);
                    m_os.flags(flags);
                }
                else {
                    m_os << c;
                }
            } break;
            }
        }
        m_os << '"';
        return true;
    }
    bool visit_bin(const char* v, uint32_t size) {
        (m_os << '"').write(v, size) << '"';
        return true;
    }
    bool visit_ext(const char* /*v*/, uint32_t /*size*/) {
        m_os << "EXT";
        return true;
    }
    bool start_array(uint32_t num_elements) {
        m_current_size.push_back(num_elements);
        m_os << "[";
        return true;
    }
    bool start_array_item() {
        return true;
    }
    bool end_array_item() {
        --m_current_size.back();
        if (m_current_size.back() != 0) {
            m_os << ",";
        }
        return true;
    }
    bool end_array() {
        m_current_size.pop_back();
        m_os << "]";
        return true;
    }
    bool start_map(uint32_t num_kv_pairs) {
        m_current_size.push_back(num_kv_pairs);
        m_os << "{";
        return true;
    }
    bool start_map_key() {
        return true;
    }
    bool end_map_key() {
        m_os << ":";
        return true;
    }
    bool start_map_value() {
        return true;
    }
    bool end_map_value() {
        --m_current_size.back();
        if (m_current_size.back() != 0) {
            m_os << ",";
        }
        return true;
    }
    bool end_map() {
        m_current_size.pop_back();
        m_os << "}";
        return true;
    }
private:
    std::ostream& m_os;
    std::vector<uint32_t> m_current_size;
};

inline std::size_t aligned_zone_size(msgpack::object const& obj) {
    std::size_t s = 0;
    switch (obj.type) {
    case msgpack::type::ARRAY:
        s += msgpack::aligned_size(
            sizeof(msgpack::object) * obj.via.array.size,
            MSGPACK_ZONE_ALIGNOF(msgpack::object));
        for (uint32_t i = 0; i < obj.via.array.size; ++i) {
            s += msgpack::aligned_zone_size(obj.via.array.ptr[i]);
        }
        break;
    case msgpack::type::MAP:
        s += msgpack::aligned_size(
            sizeof(msgpack::object_kv) * obj.via.map.size,
            MSGPACK_ZONE_ALIGNOF(msgpack::object_kv));
        for (uint32_t i = 0; i < obj.via.map.size; ++i) {
            s += msgpack::aligned_zone_size(obj.via.map.ptr[i].key);
            s += msgpack::aligned_zone_size(obj.via.map.ptr[i].val);
        }
        break;
    case msgpack::type::EXT:
        s += msgpack::aligned_size(
            detail::add_ext_type_size<sizeof(std::size_t)>(obj.via.ext.size),
            MSGPACK_ZONE_ALIGNOF(char));
        break;
    case msgpack::type::STR:
        s += msgpack::aligned_size(obj.via.str.size, MSGPACK_ZONE_ALIGNOF(char));
        break;
    case msgpack::type::BIN:
        s += msgpack::aligned_size(obj.via.bin.size, MSGPACK_ZONE_ALIGNOF(char));
        break;
    default:
        break;
    }
    return s;
}

/// clone object
/**
 * Clone (deep copy) object.
 * The copied object is located on newly allocated zone.
 * @param obj copy source object
 *
 * @return object_handle that holds deep copied object and zone.
 */
inline object_handle clone(msgpack::object const& obj) {
    std::size_t size = msgpack::aligned_zone_size(obj);
    msgpack::unique_ptr<msgpack::zone> z(size == 0 ? MSGPACK_NULLPTR : new msgpack::zone(size));
    msgpack::object newobj = z.get() ? msgpack::object(obj, *z) : obj;
    return object_handle(newobj, msgpack::move(z));
}

template <typename T>
inline object::implicit_type::operator T() { return obj.as<T>(); }

namespace detail {
template <typename Stream, typename T>
struct packer_serializer {
    static msgpack::packer<Stream>& pack(msgpack::packer<Stream>& o, const T& v) {
        v.msgpack_pack(o);
        return o;
    }
};
} // namespace detail

// Adaptor functors' member functions definitions.
template <typename T, typename Enabler>
inline
msgpack::object const&
adaptor::convert<T, Enabler>::operator()(msgpack::object const& o, T& v) const {
    v.msgpack_unpack(o.convert());
    return o;
}

template <typename T, typename Enabler>
template <typename Stream>
inline
msgpack::packer<Stream>&
adaptor::pack<T, Enabler>::operator()(msgpack::packer<Stream>& o, T const& v) const {
    return msgpack::detail::packer_serializer<Stream, T>::pack(o, v);
}

template <typename T, typename Enabler>
inline
void
adaptor::object_with_zone<T, Enabler>::operator()(msgpack::object::with_zone& o, T const& v) const {
    v.msgpack_object(static_cast<msgpack::object*>(&o), o.zone);
}

// Adaptor functor specialization to object
namespace adaptor {

template <>
struct convert<msgpack::object> {
    msgpack::object const& operator()(msgpack::object const& o, msgpack::object& v) const {
        v = o;
        return o;
    }
};

template <>
struct pack<msgpack::object> {
    template <typename Stream>
    msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, msgpack::object const& v) const {
        object_pack_visitor<Stream> vis(o);
        msgpack::object_parser(v).parse(vis);
        return o;
#if 0
        switch(v.type) {
        case msgpack::type::NIL:
            o.pack_nil();
            return o;

        case msgpack::type::BOOLEAN:
            if(v.via.boolean) {
                o.pack_true();
            } else {
                o.pack_false();
            }
            return o;

        case msgpack::type::POSITIVE_INTEGER:
            o.pack_uint64(v.via.u64);
            return o;

        case msgpack::type::NEGATIVE_INTEGER:
            o.pack_int64(v.via.i64);
            return o;

        case msgpack::type::FLOAT32:
            o.pack_float(static_cast<float>(v.via.f64));
            return o;

        case msgpack::type::FLOAT64:
            o.pack_double(v.via.f64);
            return o;

        case msgpack::type::STR:
            o.pack_str(v.via.str.size);
            o.pack_str_body(v.via.str.ptr, v.via.str.size);
            return o;

        case msgpack::type::BIN:
            o.pack_bin(v.via.bin.size);
            o.pack_bin_body(v.via.bin.ptr, v.via.bin.size);
            return o;

        case msgpack::type::EXT:
            o.pack_ext(v.via.ext.size, v.via.ext.type());
            o.pack_ext_body(v.via.ext.data(), v.via.ext.size);
            return o;

        case msgpack::type::ARRAY:
            o.pack_array(v.via.array.size);
            for(msgpack::object* p(v.via.array.ptr),
                    * const pend(v.via.array.ptr + v.via.array.size);
                p < pend; ++p) {
                msgpack::operator<<(o, *p);
            }
            return o;

        case msgpack::type::MAP:
            o.pack_map(v.via.map.size);
            for(msgpack::object_kv* p(v.via.map.ptr),
                    * const pend(v.via.map.ptr + v.via.map.size);
                p < pend; ++p) {
                msgpack::operator<<(o, p->key);
                msgpack::operator<<(o, p->val);
            }
            return o;

        default:
            throw msgpack::type_error();
        }
#endif
    }
};

template <>
struct object_with_zone<msgpack::object> {
    void operator()(msgpack::object::with_zone& o, msgpack::object const& v) const {
        o.type = v.type;

        switch(v.type) {
        case msgpack::type::NIL:
        case msgpack::type::BOOLEAN:
        case msgpack::type::POSITIVE_INTEGER:
        case msgpack::type::NEGATIVE_INTEGER:
        case msgpack::type::FLOAT32:
        case msgpack::type::FLOAT64:
            std::memcpy(&o.via, &v.via, sizeof(v.via));
            return;

        case msgpack::type::STR: {
            char* ptr = static_cast<char*>(o.zone.allocate_align(v.via.str.size, MSGPACK_ZONE_ALIGNOF(char)));
            o.via.str.ptr = ptr;
            o.via.str.size = v.via.str.size;
            std::memcpy(ptr, v.via.str.ptr, v.via.str.size);
            return;
        }

        case msgpack::type::BIN: {
            char* ptr = static_cast<char*>(o.zone.allocate_align(v.via.bin.size, MSGPACK_ZONE_ALIGNOF(char)));
            o.via.bin.ptr = ptr;
            o.via.bin.size = v.via.bin.size;
            std::memcpy(ptr, v.via.bin.ptr, v.via.bin.size);
            return;
        }

        case msgpack::type::EXT: {
            char* ptr = static_cast<char*>(o.zone.allocate_align(v.via.ext.size + 1, MSGPACK_ZONE_ALIGNOF(char)));
            o.via.ext.ptr = ptr;
            o.via.ext.size = v.via.ext.size;
            std::memcpy(ptr, v.via.ext.ptr, v.via.ext.size + 1);
            return;
        }

        case msgpack::type::ARRAY:
            o.via.array.ptr = static_cast<msgpack::object*>(o.zone.allocate_align(sizeof(msgpack::object) * v.via.array.size, MSGPACK_ZONE_ALIGNOF(msgpack::object)));
            o.via.array.size = v.via.array.size;
            for (msgpack::object
                     * po(o.via.array.ptr),
                     * pv(v.via.array.ptr),
                     * const pvend(v.via.array.ptr + v.via.array.size);
                 pv < pvend;
                 ++po, ++pv) {
                new (po) msgpack::object(*pv, o.zone);
            }
            return;

        case msgpack::type::MAP:
            o.via.map.ptr = (msgpack::object_kv*)o.zone.allocate_align(sizeof(msgpack::object_kv) * v.via.map.size, MSGPACK_ZONE_ALIGNOF(msgpack::object_kv));
            o.via.map.size = v.via.map.size;
            for(msgpack::object_kv
                    * po(o.via.map.ptr),
                    * pv(v.via.map.ptr),
                    * const pvend(v.via.map.ptr + v.via.map.size);
                pv < pvend;
                ++po, ++pv) {
                msgpack::object_kv* kv = new (po) msgpack::object_kv;
                new (&kv->key) msgpack::object(pv->key, o.zone);
                new (&kv->val) msgpack::object(pv->val, o.zone);
            }
            return;

        default:
            throw msgpack::type_error();
        }

    }
};

// Adaptor functor specialization to object::with_zone

template <>
struct object_with_zone<msgpack::object::with_zone> {
    void operator()(
        msgpack::object::with_zone& o,
        msgpack::object::with_zone const& v) const {
        o << static_cast<msgpack::object const&>(v);
    }
};


} // namespace adaptor


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
        msgpack::operator<<(o, static_cast<const msgpack_type&>(*this));
    }

    void msgpack_unpack(object const& o)
    {
        msgpack::operator>>(o, static_cast<msgpack_type&>(*this));
    }
};

// deconvert operator

template <typename Stream>
template <typename T>
inline msgpack::packer<Stream>& packer<Stream>::pack(const T& v)
{
    msgpack::operator<<(*this, v);
    return *this;
}

inline bool operator==(const msgpack::object& x, const msgpack::object& y)
{
    if(x.type != y.type) { return false; }

    switch(x.type) {
    case msgpack::type::NIL:
        return true;

    case msgpack::type::BOOLEAN:
        return x.via.boolean == y.via.boolean;

    case msgpack::type::POSITIVE_INTEGER:
        return x.via.u64 == y.via.u64;

    case msgpack::type::NEGATIVE_INTEGER:
        return x.via.i64 == y.via.i64;

    case msgpack::type::FLOAT32:
    case msgpack::type::FLOAT64:
        return x.via.f64 == y.via.f64;

    case msgpack::type::STR:
        return x.via.str.size == y.via.str.size &&
            std::memcmp(x.via.str.ptr, y.via.str.ptr, x.via.str.size) == 0;

    case msgpack::type::BIN:
        return x.via.bin.size == y.via.bin.size &&
            std::memcmp(x.via.bin.ptr, y.via.bin.ptr, x.via.bin.size) == 0;

    case msgpack::type::EXT:
        return x.via.ext.size == y.via.ext.size &&
            std::memcmp(x.via.ext.ptr, y.via.ext.ptr, x.via.ext.size) == 0;

    case msgpack::type::ARRAY:
        if(x.via.array.size != y.via.array.size) {
            return false;
        } else if(x.via.array.size == 0) {
            return true;
        } else {
            msgpack::object* px = x.via.array.ptr;
            msgpack::object* const pxend = x.via.array.ptr + x.via.array.size;
            msgpack::object* py = y.via.array.ptr;
            do {
                if(!(*px == *py)) {
                    return false;
                }
                ++px;
                ++py;
            } while(px < pxend);
            return true;
        }

    case msgpack::type::MAP:
        if(x.via.map.size != y.via.map.size) {
            return false;
        } else if(x.via.map.size == 0) {
            return true;
        } else {
            msgpack::object_kv* px = x.via.map.ptr;
            msgpack::object_kv* const pxend = x.via.map.ptr + x.via.map.size;
            msgpack::object_kv* py = y.via.map.ptr;
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
inline bool operator==(const msgpack::object& x, const T& y)
try {
    return x == msgpack::object(y);
} catch (msgpack::type_error&) {
    return false;
}

inline bool operator!=(const msgpack::object& x, const msgpack::object& y)
{ return !(x == y); }

template <typename T>
inline bool operator==(const T& y, const msgpack::object& x)
{ return x == y; }

template <typename T>
inline bool operator!=(const msgpack::object& x, const T& y)
{ return !(x == y); }

template <typename T>
inline bool operator!=(const T& y, const msgpack::object& x)
{ return x != y; }


inline object::implicit_type object::convert() const
{
    return object::implicit_type(*this);
}

template <typename T>
inline
typename msgpack::enable_if<
    !msgpack::is_array<T>::value && !msgpack::is_pointer<T>::value,
    T&
>::type
object::convert(T& v) const
{
    msgpack::operator>>(*this, v);
    return v;
}

template <typename T, std::size_t N>
inline T(&object::convert(T(&v)[N]) const)[N]
{
    msgpack::operator>>(*this, v);
    return v;
}

#if !defined(MSGPACK_DISABLE_LEGACY_CONVERT)
template <typename T>
inline
typename msgpack::enable_if<
    msgpack::is_pointer<T>::value,
    T
>::type
object::convert(T v) const
{
    convert(*v);
    return v;
}
#endif // !defined(MSGPACK_DISABLE_LEGACY_CONVERT)

template <typename T>
inline bool object::convert_if_not_nil(T& v) const
{
    if (is_nil()) {
        return false;
    }
    convert(v);
    return true;
}

#if defined(MSGPACK_USE_CPP03)

template <typename T>
inline T object::as() const
{
    T v;
    convert(v);
    return v;
}

#else  // defined(MSGPACK_USE_CPP03)

template <typename T>
inline typename std::enable_if<msgpack::has_as<T>::value, T>::type object::as() const {
    return msgpack::adaptor::as<T>()(*this);
}

template <typename T>
inline typename std::enable_if<!msgpack::has_as<T>::value, T>::type object::as() const {
    T v;
    convert(v);
    return v;
}

#endif // defined(MSGPACK_USE_CPP03)

inline object::object()
{
    type = msgpack::type::NIL;
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
inline object::object(const T& v, msgpack::zone& z)
{
    with_zone oz(z);
    msgpack::operator<<(oz, v);
    type = oz.type;
    via = oz.via;
}

template <typename T>
inline object::object(const T& v, msgpack::zone* z)
{
    with_zone oz(*z);
    msgpack::operator<<(oz, v);
    type = oz.type;
    via = oz.via;
}


inline object::object(const msgpack_object& o)
{
    // FIXME beter way?
    std::memcpy(this, &o, sizeof(o));
}

inline void operator<< (msgpack::object& o, const msgpack_object& v)
{
    // FIXME beter way?
    std::memcpy(static_cast<void*>(&o), &v, sizeof(v));
}

inline object::operator msgpack_object() const
{
    // FIXME beter way?
    msgpack_object obj;
    std::memcpy(&obj, this, sizeof(obj));
    return obj;
}


// obsolete
template <typename T>
inline void convert(T& v, msgpack::object const& o)
{
    o.convert(v);
}

// obsolete
template <typename Stream, typename T>
inline void pack(msgpack::packer<Stream>& o, const T& v)
{
    o.pack(v);
}

// obsolete
template <typename Stream, typename T>
inline void pack_copy(msgpack::packer<Stream>& o, T v)
{
    pack(o, v);
}

template <typename Stream>
inline msgpack::packer<Stream>& operator<< (msgpack::packer<Stream>& o, const msgpack::object& v)
{
    object_pack_visitor<Stream> vis(o);
    msgpack::object_parser(v).parse(vis);
    return o;
}

template <typename Stream>
inline msgpack::packer<Stream>& operator<< (msgpack::packer<Stream>& o, const msgpack::object::with_zone& v)
{
    return o << static_cast<msgpack::object>(v);
}

inline std::ostream& operator<< (std::ostream& s, const msgpack::object& v)
{
    object_stringize_visitor vis(s);
    msgpack::object_parser(v).parse(vis);
    return s;
}

/// @cond
}  // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

}  // namespace msgpack

#endif // MSGPACK_V1_OBJECT_HPP
