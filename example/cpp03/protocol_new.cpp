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

#include <msgpack.hpp>
#include <string>
#include <iostream>
#include <sstream>

// This example uses obsolete APIs
// See protocol_new.cpp
namespace myprotocol {
    struct Get {
        Get() {}
        Get(uint32_t f, const std::string& k) : flags(f), key(k) {}
        uint32_t flags;
        std::string key;
        MSGPACK_DEFINE(flags, key);
    };

    typedef std::vector<Get> MultiGet;
}


int main(void)
{
    // send Get request
    std::stringstream stream;
    {
        myprotocol::Get req;
        req.flags = 0;
        req.key   = "key0";
        msgpack::pack(stream, req);
    }

    stream.seekg(0);

    // receive Get request
    {
        std::string buffer(stream.str());

        msgpack::unpacked result;
        msgpack::unpack(result, buffer.data(), buffer.size());
        msgpack::object o = result.get();

        myprotocol::Get req;
        o.convert(req);
        std::cout << "received: " << o << std::endl;
    }


    stream.str("");


    // send MultiGet request
    {
        myprotocol::MultiGet req;
        req.push_back( myprotocol::Get(1, "key1") );
        req.push_back( myprotocol::Get(2, "key2") );
        req.push_back( myprotocol::Get(3, "key3") );
        msgpack::pack(stream, req);
    }

    stream.seekg(0);

    // receive MultiGet request
    {
        std::string buffer(stream.str());

        msgpack::unpacked result;
        msgpack::unpack(result, buffer.data(), buffer.size());
        msgpack::object o = result.get();


        myprotocol::MultiGet req;
        o.convert(req);
        std::cout << "received: " << o << std::endl;
    }
}
