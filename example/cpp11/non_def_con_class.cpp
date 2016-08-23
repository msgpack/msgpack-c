// MessagePack for C++ example
//
// Copyright (C) 2015 KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//

#include <cassert>
#include <memory>
#include <iostream>

#include <msgpack.hpp>

struct my {
    my() = delete;

    // target class should be either copyable or movable (or both).
    my(my const&) = delete;
    my(my&&) = default;

    my(int a):a(a) {}
    int a;
    MSGPACK_DEFINE(a);

    bool operator==(const my & other) const
    {
       return other.a == a;
    }

    bool operator<(const my & other) const
    {
       return other.a > a;
    }
};

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
namespace adaptor {

template<>
struct as<my> {
    my operator()(msgpack::object const& o) const {
        if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
        if (o.via.array.size != 1) throw msgpack::type_error();
        return my(o.via.array.ptr[0].as<int>());
    }
};

} // namespace adaptor
} // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // namespace msgpack

int main() {
    my m1(42);
    msgpack::zone z;
    msgpack::object obj(m1, z);
    std::cout << obj << std::endl;
    assert(m1.a == obj.as<my>().a);

    // useful keys for maps do not have as<>() method implemented:
    std::cout << "has_as<int>    " << msgpack::has_as<int>::value << std::endl;
    std::cout << "has_as<string> " << msgpack::has_as<std::string>::value << std::endl;

    // BEFORE PATCH: as a result as<map<K, V> >() is not available either.
    // AFTER PATCH:  as<map<K, V> >() is available if key OR value have an as<>() implementation
    std::cout << "has_as<std::map<int, my> > " << msgpack::has_as<std::map<int, my> >::value << std::endl;

    std::map<int, my> m;
    m.emplace(1, my(17));
    msgpack::object obj2(m, z);
    std::cout << obj2 << std::endl;

    // BEFORE PATCH: this makes the following break with a compiler error,
    // beacuse it uses the deleted my:my() constructor, as it falls back to the
    // convert() method.
    // AFTER PATCH: the following works and uses the customary as() implementation
    assert(m == obj2.as<decltype(m)>());

    std::map<my, int> m2;
    m2.emplace(17, 42);
    msgpack::object obj3(m2, z);
    std::cout << obj3 << std::endl;

    // BEFORE PATCH: this makes the following break with a compiler error:
    // beacuse it uses the deleted my:my() constructor, as it falls back to the
    // convert() method.
    // AFTER PATCH: the following works and uses the customary as() implementation
    assert(m2 == obj3.as<decltype(m2)>());
}
