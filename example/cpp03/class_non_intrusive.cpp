#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>

#include <msgpack_fwd.hpp>


// declarations
class my_class;

namespace msgpack {

MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {

object const& operator>> (object const& o, my_class& v);

template <typename Stream>
packer<Stream>& operator<< (packer<Stream>& o, my_class const& v);

} // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // namespace msgpack

#include <msgpack.hpp>

class my_class {
public:
    my_class() {} // When you want to convert from msgpack::object to my_class,
                  // my_class should be default constractible.
    my_class(std::string const& name, int age):name_(name), age_(age) {}

    // When you want to define a non-intrusive adaptor functions,
    // member variables should be public.
    std::string name_;
    int age_;
};

inline bool operator==(my_class const& lhs, my_class const& rhs) {
    return lhs.name_ == rhs.name_ && lhs.age_ == rhs.age_;
}

// definitions

namespace msgpack {

MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {

inline object const& operator>> (object const& o, my_class& v) {
    if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
    if (o.via.array.size != 2) throw msgpack::type_error();
    o.via.array.ptr[0].convert(v.name_);
    o.via.array.ptr[1].convert(v.age_);
    return o;
}


template <typename Stream>
inline packer<Stream>& operator<< (packer<Stream>& o, my_class const& v) {
    // packing member variables as an array.
    o.pack_array(2);
    o.pack(v.name_);
    o.pack(v.age_);
    return o;
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
    std::cout << std::endl;
}

int main() {
    my_class my("John Smith", 42);
    std::stringstream ss;
    msgpack::pack(ss, my);

    print(ss.str());

    msgpack::unpacked unp;
    msgpack::unpack(unp, ss.str().data(), ss.str().size());
    msgpack::object obj = unp.get();
    assert(obj.as<my_class>() == my);
}
