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

#ifndef MSGPACK_CPP11_REFERENCE_WRAPPER_HPP
#define MSGPACK_CPP11_REFERENCE_WRAPPER_HPP

#include "msgpack/versioning.hpp"
#include "msgpack/adaptor/adaptor_base.hpp"
#include "msgpack/adaptor/check_container_size.hpp"

#include <memory>
#include <type_traits>

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond

namespace adaptor {

template <typename T>
struct convert<std::reference_wrapper<T>> {
    msgpack::object const& operator()(msgpack::object const& o, std::reference_wrapper<T>& v) const {
        msgpack::adaptor::convert<T>()(o, v.get());
        return o;
    }
};

template <typename T>
struct pack<std::reference_wrapper<T>> {
    template <typename Stream>
    msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, const std::reference_wrapper<T>& v) const {
        o.pack(v.get());
        return o;
    }
};

template <typename T>
struct object<std::reference_wrapper<T> > {
    void operator()(msgpack::object& o, const std::reference_wrapper<T>& v) const {
        msgpack::adaptor::object<typename std::remove_const<T>::type>()(o, v.get());
    }
};

template <typename T>
struct object_with_zone<std::reference_wrapper<T>> {
    void operator()(msgpack::object::with_zone& o, const std::reference_wrapper<T>& v) const {
        msgpack::adaptor::object_with_zone<typename std::remove_const<T>::type>()(o, v.get());
    }
};

} // namespace adaptor

/// @cond
} // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

} // namespace msgpack

#endif // MSGPACK_CPP11_REFERENCE_WRAPPER_HPP
