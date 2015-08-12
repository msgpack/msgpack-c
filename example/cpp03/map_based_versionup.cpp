// MessagePack for C++ example
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

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>

#include <msgpack.hpp>

struct base1 {
    base1():a("default") {}
    std::string a;
    MSGPACK_DEFINE_MAP(a);
};

struct v1 : base1 {
    v1():name("default"), age(0) {}
    std::string name;
    int age;
    MSGPACK_DEFINE_MAP(MSGPACK_BASE_MAP(base1), name, age);
};

struct base2 {
    base2():a("default") {}
    std::string a;
    MSGPACK_DEFINE_MAP(a);
};

// Removed: base1, name
// Added  : base2, address
struct v2 : base2 {
    v2(): age(0), address("default") {}
    int age;
    std::string address;
    MSGPACK_DEFINE_MAP(MSGPACK_BASE_MAP(base2), age, address);
};

// The member variable "age" is in common between v1 and v2.

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
    { // pack v1, unpack, convert to v2
        v1 v;
        v.a = "ABC";
        v.name = "John Smith";
        v.age = 35;

        std::stringstream ss;
        msgpack::pack(ss, v);

        print(ss.str());

        msgpack::unpacked unp = msgpack::unpack(ss.str().data(), ss.str().size());

        msgpack::object obj = unp.get();
        std::cout << obj << std::endl;

        v2 newv = obj.as<v2>();

        std::cout << "v2::a       " << newv.a << std::endl;
        std::cout << "v2::age     " << newv.age << std::endl;
        std::cout << "v2::address " << newv.address << std::endl;

        // "age" is set from v1
        assert(newv.a == "default");
        assert(newv.age == 35);
        assert(newv.address == "default");
    }
    { // create v2 object with zone, convert to v1
        v2 v;
        v.a = "DEF";
        v.age = 42;
        v.address = "Tokyo";

        msgpack::zone z;
        msgpack::object obj(v, z);
        std::cout << obj << std::endl;

        v1 newv = obj.as<v1>();

        std::cout << "v1::a       " << newv.a << std::endl;
        std::cout << "v1::name    " << newv.name << std::endl;
        std::cout << "v1::age     " << newv.age << std::endl;

        // "age" is set from v2
        assert(newv.a == "default");
        assert(newv.name == "default");
        assert(newv.age == 42);
    }
}
