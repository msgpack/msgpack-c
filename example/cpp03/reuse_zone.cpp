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

#include <string>
#include <vector>

#include <msgpack.hpp>


int main() {
    std::vector<int> v;
    v.push_back(1);
    v.push_back(42);
    std::string s("ABC");

    std::stringstream ss;
    msgpack::pack(ss, v);
    msgpack::pack(ss, s);

    msgpack::zone z;
    std::size_t offset = 0;

    // msgpack array is constructed on z.
    msgpack::object obj = msgpack::unpack(z, ss.str().data(), ss.str().size(), offset);
    std::cout << obj << std::endl;
    assert(obj.as<std::vector<int> >() == v);

    // msgpack str is constructed on z.
    std::string const& str = msgpack::unpack(z, ss.str().data(), ss.str().size(), offset).as<std::string>();
    std::cout << str << std::endl;
    assert(str == s);
}
