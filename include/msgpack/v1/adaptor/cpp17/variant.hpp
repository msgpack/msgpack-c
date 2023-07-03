//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2023 Uy Ha
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MSGPACK_V1_TYPE_VARIANT_HPP
#define MSGPACK_V1_TYPE_VARIANT_HPP

#define MSGPACK_USE_STD_VARIANT_ADAPTOR

#if defined(MSGPACK_USE_STD_VARIANT_ADAPTOR)

#include "msgpack/cpp_version.hpp"

#if MSGPACK_CPP_VERSION >= 201703

#include "msgpack/adaptor/adaptor_base.hpp"
#include "msgpack/object.hpp"
#include "msgpack/versioning.hpp"

#include <variant>

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(v1) {
  namespace adaptor {
  namespace detail {
  template <typename Variant, typename T, typename... Ts, std::size_t current_index, std::size_t... indices>
  Variant construct_variant(std::size_t index,
                            msgpack::object &object,
                            std::index_sequence<current_index, indices...>) {
    if constexpr(sizeof...(Ts) == 0) {
      return object.as<T>();
    } else {
      if (index == current_index) {
        return object.as<T>();
      }
      return construct_variant<Variant, Ts...>(index, object, std::index_sequence<indices...>());
    }
  }

  } // namespace detail

  template <typename... Ts>
  struct as<std::variant<Ts...>, std::enable_if_t<(msgpack::has_as<Ts>::value && ...)>> {
    std::variant<Ts...> operator()(msgpack::object const &o) const {
      if (o.type != msgpack::type::ARRAY) {
        throw msgpack::type_error{};
      }
      if (o.via.array.size != 2) {
        throw msgpack::type_error{};
      }
      if (o.via.array.ptr[0].type != msgpack::type::POSITIVE_INTEGER) {
        throw msgpack::type_error{};
      }
      return detail::construct_variant<std::variant<Ts...>, Ts...>(
        o.via.array.ptr[0].as<std::size_t>(),
        o.via.array.ptr[1],
        std::make_index_sequence<sizeof...(Ts)>()
      );
    }
  };

  template<typename... Ts>
  struct convert<std::variant<Ts...>> {
    msgpack::object const &operator()(msgpack::object const &o, std::variant<Ts...> &v) const {
      if (o.type != msgpack::type::ARRAY) {
        throw msgpack::type_error{};
      }
      if (o.via.array.size != 2) {
        throw msgpack::type_error{};
      }
      if (o.via.array.ptr[0].type != msgpack::type::POSITIVE_INTEGER) {
        throw msgpack::type_error{};
      }
      v =  detail::construct_variant<std::variant<Ts...>, Ts...>(
        o.via.array.ptr[0].as<std::size_t>(),
        o.via.array.ptr[1],
        std::make_index_sequence<sizeof...(Ts)>()
      );
      return o;
    }
  };

  template <typename... Ts>
  struct pack<std::variant<Ts...>>{
    template<typename Stream>
    msgpack::packer<Stream>& operator()(msgpack::packer<Stream> &o, std::variant<Ts...> const &v) const {
      o.pack_array(2);
      o.pack_uint64(v.index());
      std::visit([&o](auto const &real_value){o.pack(real_value);}, v);
      return o;
    }
  };

  // template <typename... Ts>
  // struct object<std::variant<Ts...>> {
  //   void operator()(msgpack::object &o, std::variant<Ts...> const &v) const {
  //     o.type = msgpack::type::ARRAY;
  //     o.via.array.size = 2;
  //     msgpack::adaptor::object<std::size_t>(o.via.array.ptr[0], v.index());
  //     std::visit([&o](auto const &value) {
  //       msgpack::adaptor::object(o.via.array.ptr[1], value);
  //     }, v);
  //   }
  // };
  //
  // template<typename... Ts>
  // struct object_with_zone<std::variant<Ts...>> {
  //   void operator()(msgpack::object::with_zone &o, std::variant<Ts...> const &v) const {
  //     o.type = msgpack::type::ARRAY;
  //
  //     msgpack::object *p = static_cast<msgpack::object *>(o.zone.allocate_align(sizeof(msgpack::object) * 2, MSGPACK_ZONE_ALIGNOF(msgpack::object)));
  //
  //     o.via.array.size = 2;
  //     o.via.array.ptr = p;
  //     msgpack::adaptor::object_with_zone<std::size_t>()(o.via.array.ptr[0], v.index(), o.zone);
  //     std::visit([&o](auto const &real_value){
  //       o.via.array.ptr[1] = msgpack::adaptor::object()(real_value, o.zone);
  //     }, v);
  //   }
  // };
  } // namespace adaptor
}
} // namespace msgpack

#endif // MSGPACK_CPP_VERSION >= 201703
#endif // defined(MSGPACK_USE_STD_VARIANT_ADAPTOR)
#endif // MSGPACK_V1_TYPE_VARIANT_HPP
