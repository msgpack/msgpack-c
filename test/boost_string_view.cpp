#include <msgpack.hpp>
#include <sstream>
#include <iterator>

#define BOOST_TEST_MODULE MSGPACK_BOOST
#include <boost/test/unit_test.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if defined(MSGPACK_NO_BOOST)

BOOST_AUTO_TEST_CASE(empty)
{
}
#else  // defined(MSGPACK_NO_BOOST)

// Avoid empty test tree on boost 1.60 and lower
BOOST_AUTO_TEST_CASE(dummy)
{
}

#if (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 61

BOOST_AUTO_TEST_CASE(pack_convert_string_view)
{
    std::stringstream ss;
    std::string s = "ABC";
    boost::string_view val1(s);

    msgpack::pack(ss, val1);

    msgpack::object_handle oh;
    std::string const& str = ss.str();
    msgpack::unpack(oh, str.data(), str.size());
    boost::string_view val2 = oh.get().as<boost::string_view>();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(object_strinf_view)
{
    std::string s = "ABC";
    boost::string_view val1(s);
    msgpack::object obj(val1);
    boost::string_view val2 = obj.as<boost::string_view>();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_string_view)
{
    msgpack::zone z;
    std::string s = "ABC";
    boost::string_view val1(s);
    msgpack::object obj(val1, z);
    boost::string_view val2 = obj.as<boost::string_view>();
    BOOST_CHECK(val1 == val2);
}

#endif // (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 61

#endif // defined(MSGPACK_NO_BOOST)
