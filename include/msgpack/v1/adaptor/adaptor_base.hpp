//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2015-2016 KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_V1_ADAPTOR_BASE_HPP
#define MSGPACK_V1_ADAPTOR_BASE_HPP

#include "msgpack/v1/adaptor/adaptor_base_decl.hpp"

#include "msgpack/v1/cpp_config.hpp"


namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond


namespace adaptor {

// Unpack detector

#ifndef MSGPACK_USE_CPP03

namespace impl {

template < typename T >
using msgpack_unpack_t
    = decltype( std::declval< T >().msgpack_unpack( std::declval< msgpack::object::implicit_type >() ) );

template < typename T, typename = void_t<> >
struct has_msgpack_unpack : std::false_type
{};

template < typename T >
struct has_msgpack_unpack< T, void_t< impl::msgpack_unpack_t< T > > > : std::true_type
{};

} // namespace impl

#endif

// Adaptor functors

#ifdef MSGPACK_USE_CPP03

template <typename T, typename Enabler>
struct convert {
    msgpack::object const& operator()(msgpack::object const& o, T& v) const;
};

#else

template <typename T, typename Enabler>
struct convert;

template<typename T>
struct convert<T, typename std::enable_if<adaptor::impl::has_msgpack_unpack<T>::value>::type>
{
    msgpack::object const &operator()(msgpack::object const &o, T &v) const;
};

#endif

template <typename T, typename Enabler>
struct pack {
    template <typename Stream>
    msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, T const& v) const;
};

template <typename T, typename Enabler>
struct object {
    void operator()(msgpack::object& o, T const& v) const;
};

template <typename T, typename Enabler>
struct object_with_zone {
    void operator()(msgpack::object::with_zone& o, T const& v) const;
};

} // namespace adaptor

// operators

template <typename T>
inline
typename msgpack::enable_if<
    !is_array<T>::value,
    msgpack::object const&
>::type
operator>> (msgpack::object const& o, T& v) {
    return msgpack::adaptor::convert<T>()(o, v);
}
template <typename T, std::size_t N>
inline
msgpack::object const& operator>> (msgpack::object const& o, T(&v)[N]) {
    return msgpack::adaptor::convert<T[N]>()(o, v);
}

template <typename Stream, typename T>
inline
typename msgpack::enable_if<
    !is_array<T>::value,
    msgpack::packer<Stream>&
>::type
operator<< (msgpack::packer<Stream>& o, T const& v) {
    return msgpack::adaptor::pack<T>()(o, v);
}
template <typename Stream, typename T, std::size_t N>
inline
msgpack::packer<Stream>& operator<< (msgpack::packer<Stream>& o, const T(&v)[N]) {
    return msgpack::adaptor::pack<T[N]>()(o, v);
}

template <typename T>
inline
typename msgpack::enable_if<
    !is_array<T>::value
>::type
operator<< (msgpack::object& o, T const& v) {
    msgpack::adaptor::object<T>()(o, v);
}
template <typename T, std::size_t N>
inline
void operator<< (msgpack::v1::object& o, const T(&v)[N]) {
    msgpack::v1::adaptor::object<T[N]>()(o, v);
}

template <typename T>
inline
typename msgpack::enable_if<
    !is_array<T>::value
>::type
operator<< (msgpack::object::with_zone& o, T const& v) {
    msgpack::adaptor::object_with_zone<T>()(o, v);
}
template <typename T, std::size_t N>
inline
void operator<< (msgpack::object::with_zone& o, const T(&v)[N]) {
    msgpack::adaptor::object_with_zone<T[N]>()(o, v);
}

/// @cond
} // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

} // namespace msgpack


#endif // MSGPACK_V1_ADAPTOR_BASE_HPP
