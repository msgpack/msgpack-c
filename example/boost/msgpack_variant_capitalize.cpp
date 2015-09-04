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
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

#include <msgpack.hpp>

struct user {
    std::string name;
    int age;
    std::string address;
    MSGPACK_DEFINE(name, age, address);
};

struct proc:boost::static_visitor<void> {
    void operator()(std::string& v) const {
        std::cout << "  match std::string& v" << std::endl;
        std::cout << "    v: " << v << std::endl;
        std::cout << "    capitalize" << std::endl;
        for (std::string::iterator it = v.begin(), end = v.end();
             it != end;
             ++it) {
            *it = std::toupper(*it);
        }
    }
    void operator()(std::vector<msgpack::type::variant>& v) const {
        std::cout << "match vector (msgpack::type::ARRAY)" << std::endl;
        std::vector<msgpack::type::variant>::iterator it = v.begin();
        std::vector<msgpack::type::variant>::const_iterator end = v.end();
        for (; it != end; ++it) {
            boost::apply_visitor(*this, *it);
        }
    }
    template <typename T>
    void operator()(T const&) const {
        std::cout << "  match others" << std::endl;
    }
};

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
    std::stringstream ss1;
    user u;
    u.name = "Takatoshi Kondo";
    u.age = 42;
    u.address = "Tokyo, JAPAN";

    std::cout << "Packing object." << std::endl;
    msgpack::pack(ss1, u);
    print(ss1.str());

    msgpack::unpacked unp1 = msgpack::unpack(ss1.str().data(), ss1.str().size());
    msgpack::object const& obj1 = unp1.get();
    std::cout << "Unpacked msgpack object." << std::endl;
    std::cout << obj1 << std::endl;

    msgpack::type::variant v = obj1.as<msgpack::type::variant>();
    std::cout << "Applying proc..." << std::endl;
    boost::apply_visitor(proc(), v);

    std::stringstream ss2;
    std::cout << "Packing modified object." << std::endl;
    msgpack::pack(ss2, v);
    print(ss2.str());

    msgpack::unpacked unp2 = msgpack::unpack(ss2.str().data(), ss2.str().size());
    msgpack::object const& obj2 = unp2.get();
    std::cout << "Modified msgpack object." << std::endl;
    std::cout << obj2 << std::endl;
}
