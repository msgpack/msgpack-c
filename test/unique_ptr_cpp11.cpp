#include <msgpack.hpp>
#include <sstream>
#include <iterator>

#define BOOST_TEST_MODULE UNIQUE_PTR
#include <boost/test/unit_test.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if !defined(MSGPACK_USE_CPP03)

BOOST_AUTO_TEST_CASE(pack_convert_nil)
{
    std::stringstream ss;
    std::unique_ptr<int> val1;
    msgpack::pack(ss, val1);
    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    std::unique_ptr<int> val2 = oh.get().as<std::unique_ptr<int>>();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(pack_convert_int)
{
    std::stringstream ss;
    std::unique_ptr<int> val1(new int(1));
    msgpack::pack(ss, val1);
    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    std::unique_ptr<int> val2 = oh.get().as<std::unique_ptr<int>>();
    BOOST_CHECK(*val1 == *val2);
}

BOOST_AUTO_TEST_CASE(object_nil)
{
    std::unique_ptr<int> val1;
    msgpack::object obj(val1);
    std::unique_ptr<int> val2 = obj.as<std::unique_ptr<int>>();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(object_int)
{
    std::unique_ptr<int> val1(new int(1));
    msgpack::object obj(val1);
    std::unique_ptr<int> val2 = obj.as<std::unique_ptr<int>>();
    BOOST_CHECK(*val1 == *val2);
}

// Compile error as expected
// object::with_zone is required not object
/*
BOOST_AUTO_TEST_CASE(object_vector)
{
    typedef std::unique_ptr<std::vector<int>> ovi_t;
    ovi_t val1(new std::vector<int>());
    msgpack::object obj(val1);
    ovi_t  val2 = obj.as<ovi_t>();
    BOOST_CHECK(val1 == val2);
}
*/

BOOST_AUTO_TEST_CASE(object_with_zone_nil)
{
    msgpack::zone z;
    std::unique_ptr<int> val1;
    msgpack::object obj(val1, z);
    std::unique_ptr<int> val2 = obj.as<std::unique_ptr<int>>();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_int)
{
    msgpack::zone z;
    std::unique_ptr<int> val1(new int(1));
    msgpack::object obj(val1, z);
    std::unique_ptr<int> val2 = obj.as<std::unique_ptr<int>>();
    BOOST_CHECK(*val1 == *val2);
}

struct no_def_con {
    no_def_con() = delete;
    no_def_con(int i):i(i) {}
    int i;
    MSGPACK_DEFINE(i);
};

inline bool operator==(no_def_con const& lhs, no_def_con const& rhs) {
    return lhs.i == rhs.i;
}

inline bool operator!=(no_def_con const& lhs, no_def_con const& rhs) {
    return !(lhs == rhs);
}

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
namespace adaptor {
template <>
struct as<no_def_con> {
    no_def_con operator()(msgpack::object const& o) const {
        if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
        if (o.via.array.size != 1) throw msgpack::type_error();
        return no_def_con(o.via.array.ptr[0].as<int>());
    }
};
} // adaptor
} // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // msgpack

BOOST_AUTO_TEST_CASE(pack_convert_nil_no_def_con)
{
    std::stringstream ss;
    std::unique_ptr<no_def_con> val1(new no_def_con(1));
    msgpack::pack(ss, val1);
    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    std::unique_ptr<no_def_con> val2 = oh.get().as<std::unique_ptr<no_def_con>>();
    BOOST_CHECK(*val1 == *val2);
}


#endif // !defined(MSGPACK_USE_CPP03)
