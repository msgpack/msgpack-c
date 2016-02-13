//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2008-2016 FURUHASHI Sadayuki and KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MSGPACK_V2_OBJECT_FWD_HPP
#define MSGPACK_V2_OBJECT_FWD_HPP

#include "msgpack/v2/object_fwd_decl.hpp"

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v2) {
/// @endcond

#if !defined(MSGPACK_USE_CPP03)

namespace adaptor {

// If v1 has as specialization for T, then dispatch v1::adaptor::as<T>.
// So I call v1::has_as<T> meta function intentionally.
template <typename T>
struct as<T, typename std::enable_if<v1::has_as<T>::value>::type> : v1::adaptor::as<T> {
};

} // namespace adaptor

template <typename T>
struct has_as {
private:
    template <typename U>
    static auto check(U*) ->
        typename std::enable_if<
            // check v2 specialization
            std::is_same<
                decltype(adaptor::as<U>()(std::declval<msgpack::object>())),
                U
            >::value
            ||
            // check v1 specialization
            v1::has_as<U>::value,
            std::true_type
        >::type;
    template <typename>
    static std::false_type check(...);
public:
    using type = decltype(check<T>(nullptr));
    static constexpr bool value = type::value;
};

#endif // !defined(MSGPACK_USE_CPP03)

/// @cond
} // MSGPACK_API_VERSION_NAMESPACE(v2)
/// @endcond

} // namespace msgpack

#endif // MSGPACK_V2_OBJECT_FWD_HPP
