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

#include <iostream>
#include <sstream>
#include <cassert>

#include <array>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <forward_list>
#include <string>

#include <msgpack.hpp>

void array() {
    std::array<int, 5> a { { 1, 2, 3, 4, 5 } };
    std::stringstream ss;
    msgpack::pack(ss, a);

    msgpack::unpacked und = msgpack::unpack(ss.str().data(), ss.str().size());
    msgpack::object obj = und.get();

    std::cout << obj << std::endl;
    assert((obj.as<std::array<int, 5>>()) == a);
}

void tuple() {
    std::tuple<bool, std::string, int> t(true, "ABC", 42);
    std::stringstream ss;
    msgpack::pack(ss, t);

    auto und = msgpack::unpack(ss.str().data(), ss.str().size());
    auto obj = und.get();

    std::cout << obj << std::endl;
    assert(obj.as<decltype(t)>() == t);
}

void unordered_map() {
    std::unordered_map<std::string, int> m { {"ABC", 1}, {"DEF", 3} };
    std::stringstream ss;
    msgpack::pack(ss, m);

    auto und = msgpack::unpack(ss.str().data(), ss.str().size());
    msgpack::object obj = und.get();

    std::cout << obj << std::endl;
    assert(obj.as<decltype(m)>() == m);
}

void unordered_set() {
    std::unordered_set<std::string> s { "ABC", "DEF" };
    std::stringstream ss;
    msgpack::pack(ss, s);

    auto und = msgpack::unpack(ss.str().data(), ss.str().size());
    auto obj = und.get();

    std::cout << obj << std::endl;
    assert(obj.as<decltype(s)>() == s);
}

void forward_list() {
    using type = std::forward_list<std::string>;
    type f { "ABC", "DEF" };
    std::stringstream ss;
    msgpack::pack(ss, f);

    auto und = msgpack::unpack(ss.str().data(), ss.str().size());
    auto obj = und.get();

    std::cout << obj << std::endl;
    assert(obj.as<type>() == f);
}

void combi() {
    std::array<int, 5>                   a { { 1, 2, 3, 4, 5 } };
    std::tuple<bool, std::string, int>   t {true, "ABC", 42};
    std::unordered_map<std::string, int> m { {"ABC", 1}, {"DEF", 3} };
    std::unordered_set<std::string>      s { "ABC", "DEF" };
    std::forward_list<std::string>       f { "ABC", "DEF" };

    std::stringstream ss;
    msgpack::pack(ss, a);
    msgpack::pack(ss, t);
    msgpack::pack(ss, m);
    msgpack::pack(ss, s);
    msgpack::pack(ss, f);

    std::size_t offset = 0;
    std::cout << "offset: " << offset << std::endl;
    {
        auto und = msgpack::unpack(ss.str().data(), ss.str().size(), offset);
        auto obj = und.get();

        std::cout << obj << std::endl;
        assert(obj.as<decltype(a)>() == a);
    }
    std::cout << "offset: " << offset << std::endl;
    {
        auto und = msgpack::unpack(ss.str().data(), ss.str().size(), offset);
        auto obj = und.get();

        std::cout << obj << std::endl;
        assert(obj.as<decltype(t)>() == t);
    }
    std::cout << "offset: " << offset << std::endl;
    {
        auto und = msgpack::unpack(ss.str().data(), ss.str().size(), offset);
        auto obj = und.get();

        std::cout << obj << std::endl;
        assert(obj.as<decltype(m)>() == m);
    }
    std::cout << "offset: " << offset << std::endl;
    {
        auto und = msgpack::unpack(ss.str().data(), ss.str().size(), offset);
        auto obj = und.get();

        std::cout << obj << std::endl;
        assert(obj.as<decltype(s)>() == s);
    }
    std::cout << "offset: " << offset << std::endl;
    {
        auto und = msgpack::unpack(ss.str().data(), ss.str().size(), offset);
        auto obj = und.get();

        std::cout << obj << std::endl;
        assert(obj.as<decltype(f)>() == f);
    }
    std::cout << "offset: " << offset << std::endl;
}

int main() {
    array();
    tuple();
    unordered_map();
    unordered_set();
    forward_list();
    combi();
}
