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

#include <sstream>
#include <iostream>
#include <cassert>

#include <msgpack.hpp>

enum my_enum {
    elem1,
    elem2,
    elem3
};

MSGPACK_ADD_ENUM(my_enum);

int main(void)
{
    {   // pack, unpack
        std::stringstream sbuf;
        msgpack::pack(sbuf, elem1);
        msgpack::pack(sbuf, elem2);
        my_enum e3 = elem3;
        msgpack::pack(sbuf, e3);

        msgpack::unpacked result;
        std::size_t off = 0;

        msgpack::unpack(result, sbuf.str().data(), sbuf.str().size(), off);
        std::cout << result.get().as<my_enum>() << std::endl;
        assert(result.get().as<my_enum>() == elem1);

        msgpack::unpack(result, sbuf.str().data(), sbuf.str().size(), off);
        std::cout << result.get().as<my_enum>() << std::endl;
        assert(result.get().as<my_enum>() == elem2);

        msgpack::unpack(result, sbuf.str().data(), sbuf.str().size(), off);
        std::cout << result.get().as<my_enum>() << std::endl;
        assert(result.get().as<my_enum>() == elem3);
    }
    {   // create object without zone
        msgpack::object obj(elem2);
        std::cout << obj.as<my_enum>() << std::endl;
        assert(obj.as<my_enum>() == elem2);
    }
    {   // create object with zone
        msgpack::zone z;
        msgpack::object objz(elem3, z);
        std::cout << objz.as<my_enum>() << std::endl;
        assert(objz.as<my_enum>() == elem3);
    }
}
