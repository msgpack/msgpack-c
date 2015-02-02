// MessagePack for C++ example
//
// Copyright (C) 2008-2015 FURUHASHI Sadayuki and KONDO Takatoshi
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

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>

// msgpack.hpp should be included at after user declarations
#include <msgpack_fwd.hpp>


// declarations
class my_class;

namespace msgpack {

MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {

object const& operator>> (msgpack::object const& o, my_class& v);

template <typename Stream>
packer<Stream>& operator<< (msgpack::packer<Stream>& o, my_class const& v);

void operator<< (msgpack::object::with_zone& o, my_class const& v);

} // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // namespace msgpack

#include <msgpack.hpp>

class my_class {
public:
    my_class() {} // When you want to convert from msgpack::object to my_class,
                  // my_class should be default constractible.
    my_class(std::string const& name, int age):name_(name), age_(age) {}

    // my_class should provide getters for the data members you want to pack.
    std::string const& get_name() const { return name_; }
    int get_age() const { return age_; }

    friend bool operator==(my_class const& lhs, my_class const& rhs) {
        return lhs.name_ == rhs.name_ && lhs.age_ == rhs.age_;
    }

private:
    std::string name_;
    int age_;
};


// definitions

namespace msgpack {

MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {

inline object const& operator>> (msgpack::object const& o, my_class& v) {
    if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
    if (o.via.array.size != 2) throw msgpack::type_error();
    v = my_class(
        o.via.array.ptr[0].as<std::string>(),
        o.via.array.ptr[1].as<int>());
    return o;
}


template <typename Stream>
inline packer<Stream>& operator<< (msgpack::packer<Stream>& o, my_class const& v) {
    // packing member variables as an array.
    o.pack_array(2);
    o.pack(v.get_name());
    o.pack(v.get_age());
    return o;
}

inline void operator<< (msgpack::object::with_zone& o, my_class const& v) {
    o.type = type::ARRAY;
    o.via.array.size = 2;
    o.via.array.ptr = static_cast<object*>(o.zone.allocate_align(sizeof(object) * o.via.array.size));
    o.via.array.ptr[0] = msgpack::object(v.get_name(), o.zone);
    o.via.array.ptr[1] = msgpack::object(v.get_age(), o.zone);
}

} // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // namespace msgpack


void print(std::string const& buf) {
    for (std::string::const_iterator it = buf.begin(), end = buf.end();
         it != end;
         ++it) {
        std::cout
            << std::setw(2)
            << std::hex
            << std::setfill('0')
            << (static_cast<int>(*it) & 0xff)
            << ' ';
    }
    std::cout << std::dec << std::endl;
}

int main() {
    {   // pack, unpack
        my_class my("John Smith", 42);
        std::stringstream ss;
        msgpack::pack(ss, my);

        print(ss.str());

        msgpack::unpacked unp;
        msgpack::unpack(unp, ss.str().data(), ss.str().size());
        msgpack::object obj = unp.get();
        std::cout << obj << std::endl;
        assert(obj.as<my_class>() == my);
    }
    {   // create object with zone
        my_class my("John Smith", 42);
        msgpack::zone z;
        msgpack::object obj(my, z);
        std::cout << obj << std::endl;
        assert(obj.as<my_class>() == my);
    }
}
