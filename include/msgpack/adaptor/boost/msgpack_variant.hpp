//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2015 KONDO Takatoshi
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
#ifndef MSGPACK_TYPE_BOOST_MSGPACK_VARIANT_HPP
#define MSGPACK_TYPE_BOOST_MSGPACK_VARIANT_HPP

#if defined(MSGPACK_USE_BOOST)

#include "msgpack/versioning.hpp"
#include "msgpack/adaptor/adaptor_base.hpp"
#include "msgpack/adaptor/check_container_size.hpp"
#include "msgpack/adaptor/boost/string_ref.hpp"

#include "msgpack/adaptor/nil.hpp"
#include "msgpack/adaptor/bool.hpp"
#include "msgpack/adaptor/int.hpp"
#include "msgpack/adaptor/float.hpp"
#include "msgpack/adaptor/string.hpp"
#include "msgpack/adaptor/vector_char.hpp"
#include "msgpack/adaptor/raw.hpp"
#include "msgpack/adaptor/ext.hpp"
#include "msgpack/adaptor/vector.hpp"
#include "msgpack/adaptor/map.hpp"

#include <boost/variant.hpp>

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond

namespace type {

typedef boost::make_recursive_variant<
    nil,               // NIL
    bool,              // BOOL
    int64_t,           // NEGATIVE_INTEGER
    uint64_t,          // POSITIVE_INTEGER
    double,            // FLOAT
    std::string,       // STR
#if (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
    boost::string_ref, // STR
#endif // (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
    std::vector<char>, // BIN
    msgpack::type::raw_ref, // BIN
    ext,               // EXT
    ext_ref,           // EXT
    std::vector<boost::recursive_variant_>, // ARRAY
    std::map<boost::recursive_variant_, boost::recursive_variant_>, // MAP
    std::multimap<boost::recursive_variant_, boost::recursive_variant_> // MAP
>::type variant;

namespace detail {
struct ref_tag {
#if defined(MSGPACK_USE_CPP03)
private:
    ref_tag();
#else  // defined(MSGPACK_USE_CPP03)
    ref_tag() = delete;
#endif // defined(MSGPACK_USE_CPP03)
public:
    MSGPACK_DEFINE();
};

inline bool operator<(ref_tag const& lhs, ref_tag const& rhs) {
    return &lhs < &rhs;
}
inline bool operator==(ref_tag const& lhs, ref_tag const& rhs) {
    return &lhs == &rhs;
}
} // namespace detail

typedef boost::make_recursive_variant<
    nil,               // NIL
    bool,              // BOOL
    int64_t,           // NEGATIVE_INTEGER
    uint64_t,          // POSITIVE_INTEGER
    double,            // FLOAT
    std::string,       // STR
#if (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
    boost::string_ref, // STR
#endif // (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
    std::vector<char>, // BIN
    msgpack::type::raw_ref, // BIN
    ext,               // EXT
    ext_ref,           // EXT
    std::vector<boost::recursive_variant_>, // ARRAY
    std::map<boost::recursive_variant_, boost::recursive_variant_>, // MAP
    std::multimap<boost::recursive_variant_, boost::recursive_variant_>, // MAP
    detail::ref_tag
>::type variant_ref;

} // namespace type




namespace adaptor {

#if !defined (MSGPACK_USE_CPP03)

template <>
struct as<msgpack::type::variant> {
    msgpack::type::variant operator()(msgpack::object const& o) const {
        switch(o.type) {
        case type::NIL:
            return o.as<msgpack::type::nil>();
        case type::BOOLEAN:
            return o.as<bool>();
        case type::POSITIVE_INTEGER:
            return o.as<uint64_t>();
        case type::NEGATIVE_INTEGER:
            return o.as<int64_t>();
        case type::FLOAT:
            return o.as<double>();
        case type::STR:
            return o.as<std::string>();
        case type::BIN:
            return o.as<std::vector<char> >();
        case type::EXT:
            return o.as<msgpack::type::ext>();
        case type::ARRAY:
            return o.as<std::vector<msgpack::type::variant> >();
        case type::MAP:
            return o.as<std::multimap<msgpack::type::variant, msgpack::type::variant> >();
        default:
            break;
        }
        return msgpack::type::variant();
    }
};

#endif // !defined (MSGPACK_USE_CPP03)


template <>
struct convert<msgpack::type::variant> {
    msgpack::object const& operator()(
        msgpack::object const& o,
        msgpack::type::variant& v) const {
        switch(o.type) {
        case type::NIL:
            v = o.as<msgpack::type::nil>();
            break;
        case type::BOOLEAN:
            v = o.as<bool>();
            break;
        case type::POSITIVE_INTEGER:
            v = o.as<uint64_t>();
            break;
        case type::NEGATIVE_INTEGER:
            v = o.as<int64_t>();
            break;
        case type::FLOAT:
            v = o.as<double>();
            break;
        case type::STR:
            v = o.as<std::string>();
            break;
        case type::BIN:
            v = o.as<std::vector<char> >();
            break;
        case type::EXT:
            v = o.as<msgpack::type::ext>();
            break;
        case type::ARRAY:
            v = o.as<std::vector<msgpack::type::variant> >();
            break;
        case type::MAP:
            v = o.as<std::multimap<msgpack::type::variant, msgpack::type::variant> >();
            break;
        default:
            break;
        }
        return o;
    }
};

namespace detail {

template <typename Stream>
struct pack_imp:boost::static_visitor<void> {
    template <typename T>
    void operator()(T const& value) const {
        pack<T>()(o_, value);
    }
    pack_imp(packer<Stream>& o):o_(o) {}
    packer<Stream>& o_;
};

} // namespace detail

template <>
struct pack<msgpack::type::variant> {
    template <typename Stream>
    msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, const msgpack::type::variant& v) const {
        boost::apply_visitor(detail::pack_imp<Stream>(o), v);
        return o;
    }
};

namespace detail {

struct object_imp:boost::static_visitor<void> {
    void operator()(msgpack::type::nil const& v) const {
        object<msgpack::type::nil>()(o_, v);
    }
    void operator()(bool const& v) const {
        object<bool>()(o_, v);
    }
    void operator()(uint64_t const& v) const {
        object<uint64_t>()(o_, v);
    }
    void operator()(int64_t const& v) const {
        object<int64_t>()(o_, v);
    }
    void operator()(double const& v) const {
        object<double>()(o_, v);
    }
    template <typename T>
    void operator()(T const&) const {
        throw msgpack::type_error();
    }
    object_imp(msgpack::object& o):o_(o) {}
    msgpack::object& o_;
};

} // namespace detail

template <>
struct object<msgpack::type::variant> {
    void operator()(msgpack::object& o, const msgpack::type::variant& v) const {
        boost::apply_visitor(detail::object_imp(o), v);
    }
};

namespace detail {

struct object_with_zone_imp:boost::static_visitor<void> {
    template <typename T>
    void operator()(T const& v) const {
        object_with_zone<T>()(o_, v);
    }
    object_with_zone_imp(msgpack::object::with_zone& o):o_(o) {}
    msgpack::object::with_zone& o_;
};

} // namespace detail

template <>
struct object_with_zone<msgpack::type::variant> {
    void operator()(msgpack::object::with_zone& o, const msgpack::type::variant& v) const {
        boost::apply_visitor(detail::object_with_zone_imp(o), v);
    }
};

#if !defined (MSGPACK_USE_CPP03)

template <>
struct as<msgpack::type::variant_ref> {
    msgpack::type::variant_ref operator()(msgpack::object const& o) const {
        switch(o.type) {
        case type::NIL:
            return o.as<msgpack::type::nil>();
        case type::BOOLEAN:
            return o.as<bool>();
        case type::POSITIVE_INTEGER:
            return o.as<uint64_t>();
        case type::NEGATIVE_INTEGER:
            return o.as<int64_t>();
        case type::FLOAT:
            return o.as<double>();
        case type::STR:
#if (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
            return o.as<boost::string_ref>();
#else  // (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
            return o.as<std::string>();
#endif // (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
        case type::BIN:
            return o.as<msgpack::type::raw_ref>();
        case type::EXT:
            return o.as<msgpack::type::ext_ref>();
        case type::ARRAY:
            return o.as<std::vector<msgpack::type::variant_ref> >();
        case type::MAP:
            return o.as<std::multimap<msgpack::type::variant_ref, msgpack::type::variant_ref> >();
        default:
            break;
        }
        return msgpack::type::variant_ref();
    }
};

#endif // !defined (MSGPACK_USE_CPP03)


template <>
struct convert<msgpack::type::variant_ref> {
    msgpack::object const& operator()(
        msgpack::object const& o,
        msgpack::type::variant_ref& v) const {
        switch(o.type) {
        case type::NIL:
            v = o.as<msgpack::type::nil>();
            break;
        case type::BOOLEAN:
            v = o.as<bool>();
            break;
        case type::POSITIVE_INTEGER:
            v = o.as<uint64_t>();
            break;
        case type::NEGATIVE_INTEGER:
            v = o.as<int64_t>();
            break;
        case type::FLOAT:
            v = o.as<double>();
            break;
        case type::STR:
#if (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
            v = o.as<boost::string_ref>();
#else  // (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
            v = o.as<std::string>();
#endif // (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
            break;
        case type::BIN:
            v = o.as<msgpack::type::raw_ref>();
            break;
        case type::EXT:
            v = o.as<msgpack::type::ext_ref>();
            break;
        case type::ARRAY:
            v = o.as<std::vector<msgpack::type::variant_ref> >();
            break;
        case type::MAP:
            v = o.as<std::multimap<msgpack::type::variant_ref, msgpack::type::variant_ref> >();
            break;
        default:
            break;
        }
        return o;
    }
};

template <>
struct pack<msgpack::type::variant_ref> {
    template <typename Stream>
    msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, const msgpack::type::variant_ref& v) const {
        boost::apply_visitor(detail::pack_imp<Stream>(o), v);
        return o;
    }
};

template <>
struct object<msgpack::type::variant_ref> {
    void operator()(msgpack::object& o, const msgpack::type::variant_ref& v) const {
        boost::apply_visitor(detail::object_imp(o), v);
    }
};

template <>
struct object_with_zone<msgpack::type::variant_ref> {
    void operator()(msgpack::object::with_zone& o, const msgpack::type::variant_ref& v) const {
        boost::apply_visitor(detail::object_with_zone_imp(o), v);
    }
};

} // namespace adaptor

/// @cond
} // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

} // namespace msgpack

#endif // MSGPACK_USE_BOOST
#endif // MSGPACK_TYPE_BOOST_MSGPACK_VARIANT_HPP
