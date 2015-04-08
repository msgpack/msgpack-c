//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2008-2009 FURUHASHI Sadayuki
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
#ifndef MSGPACK_TYPE_BOOST_OPTIONAL_HPP
#define MSGPACK_TYPE_BOOST_OPTIONAL_HPP

#include "msgpack/versioning.hpp"
#include "msgpack/adaptor/adaptor_base.hpp"
#include "msgpack/adaptor/check_container_size.hpp"

#include <boost/optional.hpp>

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v1) {
/// @endcond

namespace adaptor {

template <typename T>
struct convert<boost::optional<T> > {
    msgpack::object const& operator()(msgpack::object const& o, boost::optional<T>& v) const {
        if(o.is_nil()) v = boost::none;
        else {
            T t;
            msgpack::adaptor::convert<T>()(o, t);
            v = t;
        }
        return o;
    }
};

template <typename T>
struct pack<boost::optional<T> > {
    template <typename Stream>
    msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, const boost::optional<T>& v) const {
        if (v) o.pack(*v);
        else o.pack_nil();
        return o;
    }
};

template <typename T>
struct object<boost::optional<T> > {
    void operator()(msgpack::object& o, const boost::optional<T>& v) const {
        if (v) msgpack::adaptor::object<T>()(o, *v);
        else o.type = msgpack::type::NIL;
    }
};

template <typename T>
struct object_with_zone<boost::optional<T> > {
    void operator()(msgpack::object::with_zone& o, const boost::optional<T>& v) const {
        if (v) msgpack::adaptor::object_with_zone<T>()(o, *v);
        else o.type = msgpack::type::NIL;
    }
};

} // namespace adaptor

/// @cond
} // MSGPACK_API_VERSION_NAMESPACE(v1)
/// @endcond

} // namespace msgpack

#endif // MSGPACK_TYPE_BOOST_OPTIONAL_HPP
