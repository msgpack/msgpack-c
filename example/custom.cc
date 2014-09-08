#include <msgpack.hpp>
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

        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, oc);

        msgpack::unpacked result;
        msgpack::unpack(result, sbuf.data(), sbuf.size());
        msgpack::object obj = result.get();

        obj.convert(&nc);

        std::cout << obj << " value=" << nc.value << " flag=" << nc.flag << std::endl;
    }

    {
        new_class nc;
        old_class oc;

        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, nc);

        msgpack::unpacked result;
        msgpack::unpack(result, sbuf.data(), sbuf.size());
        msgpack::object obj = result.get();

        obj.convert(&oc);

        std::cout << obj << " value=" << oc.value << std::endl;
    }
}

