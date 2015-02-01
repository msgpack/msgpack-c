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

#include <msgpack.hpp>
#include <sstream>
#include <string>
#include <iostream>

class old_class {
public:
    old_class() : value("default") { }

    std::string value;

    MSGPACK_DEFINE(value);
};

class new_class {
public:
    new_class() : value("default"), flag(-1) { }

    std::string value;
    int flag;

    MSGPACK_DEFINE(value, flag);
};

int main(void)
{
    {
        old_class oc;
        new_class nc;

        std::stringstream sbuf;
        msgpack::pack(sbuf, oc);

        msgpack::unpacked result;
        msgpack::unpack(result, sbuf.str().data(), sbuf.str().size());
        msgpack::object obj = result.get();

        obj.convert(&nc);

        std::cout << obj << " value=" << nc.value << " flag=" << nc.flag << std::endl;
    }

    {
        new_class nc;
        old_class oc;

        std::stringstream sbuf;
        msgpack::pack(sbuf, nc);

        msgpack::unpacked result;
        msgpack::unpack(result, sbuf.str().data(), sbuf.str().size());
        msgpack::object obj = result.get();

        obj.convert(&oc);

        std::cout << obj << " value=" << oc.value << std::endl;
    }
}
