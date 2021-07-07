#include <cmath>
#include <msgpack.hpp>
#include <sstream>
#include <iterator>

#define BOOST_TEST_MODULE MSGPACK_BOOST
#include <boost/test/unit_test.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

const double kEPS = 1e-10;

// nil

BOOST_AUTO_TEST_CASE(pack_convert_variant_nil)
{
    std::stringstream ss;
    msgpack::type::variant val1 = msgpack::type::nil_t();
    BOOST_CHECK(val1.is_nil());
    msgpack::pack(ss, val1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    msgpack::type::variant val2 = oh.get().as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_nil());
    BOOST_CHECK_NO_THROW(boost::get<msgpack::type::nil_t>(val2));
}

BOOST_AUTO_TEST_CASE(object_variant_nil)
{
    msgpack::type::variant val1 = msgpack::type::nil_t();
    BOOST_CHECK(val1.is_nil());
    msgpack::object obj(val1);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_nil());
    BOOST_CHECK_NO_THROW(boost::get<msgpack::type::nil_t>(val2));
}

BOOST_AUTO_TEST_CASE(object_with_zone_variant_nil)
{
    msgpack::zone z;
    msgpack::type::variant val1 = msgpack::type::nil_t();
    BOOST_CHECK(val1.is_nil());
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_nil());
    BOOST_CHECK_NO_THROW(boost::get<msgpack::type::nil_t>(val2));
}

// nil (default constructor)

BOOST_AUTO_TEST_CASE(pack_convert_variant_nil_default)
{
    std::stringstream ss;
    msgpack::type::variant val1;
    BOOST_CHECK(val1.is_nil());

    msgpack::pack(ss, val1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    msgpack::type::variant val2 = oh.get().as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_nil());
    BOOST_CHECK_NO_THROW(boost::get<msgpack::type::nil_t>(val2));
}

BOOST_AUTO_TEST_CASE(object_variant_nil_default)
{
    msgpack::type::variant val1;
    BOOST_CHECK(val1.is_nil());
    msgpack::object obj(val1);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_nil());
    BOOST_CHECK_NO_THROW(boost::get<msgpack::type::nil_t>(val2));
}

BOOST_AUTO_TEST_CASE(object_with_zone_variant_nil_default)
{
    msgpack::zone z;
    msgpack::type::variant val1;
    BOOST_CHECK(val1.is_nil());
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_nil());
    BOOST_CHECK_NO_THROW(boost::get<msgpack::type::nil_t>(val2));
}

// bool

BOOST_AUTO_TEST_CASE(pack_convert_variant_bool)
{
    std::stringstream ss;
    msgpack::type::variant val1 = true;
    BOOST_CHECK(val1.is_bool());
    BOOST_CHECK(val1.as_bool());

    msgpack::pack(ss, val1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    msgpack::type::variant val2 = oh.get().as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_bool());
    BOOST_CHECK(val2.as_bool());
    BOOST_CHECK_NO_THROW(boost::get<bool>(val2));
    BOOST_CHECK(val1 == val2);
    // Tests for totally ordered
    BOOST_CHECK(!(val1 != val2));
    BOOST_CHECK(!(val1 < val2));
    BOOST_CHECK(!(val1 > val2));
    BOOST_CHECK(val1 <= val2);
    BOOST_CHECK(val1 >= val2);
}

BOOST_AUTO_TEST_CASE(object_variant_bool)
{
    msgpack::type::variant val1 = true;
    BOOST_CHECK(val1.is_bool());
    BOOST_CHECK(val1.as_bool());
    msgpack::object obj(val1);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_bool());
    BOOST_CHECK(val2.as_bool());
    BOOST_CHECK_NO_THROW(boost::get<bool>(val2));
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_variant_bool)
{
    msgpack::zone z;
    msgpack::type::variant val1 = true;
    BOOST_CHECK(val1.is_bool());
    BOOST_CHECK(val1.as_bool());
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_bool());
    BOOST_CHECK(val2.as_bool());
    BOOST_CHECK_NO_THROW(boost::get<bool>(val2));
    BOOST_CHECK(val1 == val2);
}

// positive integer

BOOST_AUTO_TEST_CASE(pack_convert_variant_positive_integer)
{
    std::stringstream ss;
    msgpack::type::variant val1 = 123;
    BOOST_CHECK(val1.is_uint64_t());
    BOOST_CHECK_EQUAL(val1.as_uint64_t(), 123U);

    msgpack::pack(ss, val1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    msgpack::type::variant val2 = oh.get().as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_uint64_t());
    BOOST_CHECK_EQUAL(val2.as_uint64_t(), 123U);
    BOOST_CHECK_NO_THROW(boost::get<uint64_t>(val2));
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(object_variant_positive_integer)
{
    msgpack::type::variant val1 = 123;
    BOOST_CHECK(val1.is_uint64_t());
    BOOST_CHECK_EQUAL(val1.as_uint64_t(), 123U);
    msgpack::object obj(val1);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_uint64_t());
    BOOST_CHECK_EQUAL(val2.as_uint64_t(), 123U);
    BOOST_CHECK_NO_THROW(boost::get<uint64_t>(val2));
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_variant_positive_integer)
{
    msgpack::zone z;
    msgpack::type::variant val1 = 123;
    BOOST_CHECK(val1.is_uint64_t());
    BOOST_CHECK_EQUAL(val1.as_uint64_t(), 123U);
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_uint64_t());
    BOOST_CHECK_EQUAL(val2.as_uint64_t(), 123U);
    BOOST_CHECK_NO_THROW(boost::get<uint64_t>(val2));
    BOOST_CHECK(val1 == val2);
}

// negative integer

BOOST_AUTO_TEST_CASE(pack_convert_variant_negative_integer)
{
    std::stringstream ss;
    msgpack::type::variant val1 = -123;
    BOOST_CHECK(val1.is_int64_t());
    BOOST_CHECK_EQUAL(val1.as_int64_t(), -123);

    msgpack::pack(ss, val1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    msgpack::type::variant val2 = oh.get().as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_int64_t());
    BOOST_CHECK_EQUAL(val2.as_int64_t(), -123);
    BOOST_CHECK_NO_THROW(boost::get<int64_t>(val2));
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(object_variant_negative_integer)
{
    msgpack::type::variant val1 = -123;
    BOOST_CHECK(val1.is_int64_t());
    BOOST_CHECK_EQUAL(val1.as_int64_t(), -123);
    msgpack::object obj(val1);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_int64_t());
    BOOST_CHECK_EQUAL(val2.as_int64_t(), -123);
    BOOST_CHECK_NO_THROW(boost::get<int64_t>(val2));
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_variant_negative_integer)
{
    msgpack::zone z;
    msgpack::type::variant val1 = -123;
    BOOST_CHECK(val1.is_int64_t());
    BOOST_CHECK_EQUAL(val1.as_int64_t(), -123);
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_int64_t());
    BOOST_CHECK_EQUAL(val2.as_int64_t(), -123);
    BOOST_CHECK_NO_THROW(boost::get<int64_t>(val2));
    BOOST_CHECK(val1 == val2);
}

// float

BOOST_AUTO_TEST_CASE(pack_convert_variant_float)
{
    std::stringstream ss;
    msgpack::type::variant val1 = 12.34;
    BOOST_CHECK(val1.is_double());
    BOOST_CHECK(fabs(12.34 - val1.as_double()) <= kEPS);

    msgpack::pack(ss, val1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    msgpack::type::variant val2 = oh.get().as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_double());
    BOOST_CHECK(fabs(12.34 - val2.as_double()) <= kEPS);
    BOOST_CHECK_NO_THROW(boost::get<double>(val2));
    BOOST_CHECK(fabs(val2.as_double() - val2.as_double()) <= kEPS);
}

BOOST_AUTO_TEST_CASE(object_variant_float)
{
    msgpack::type::variant val1 = 12.34;
    BOOST_CHECK(val1.is_double());
    BOOST_CHECK(fabs(12.34 - val1.as_double()) <= kEPS);
    msgpack::object obj(val1);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_double());
    BOOST_CHECK(fabs(12.34 - val2.as_double()) <= kEPS);
    BOOST_CHECK_NO_THROW(boost::get<double>(val2));
    BOOST_CHECK(fabs(val2.as_double() - val2.as_double()) <= kEPS);
}

BOOST_AUTO_TEST_CASE(object_with_zone_variant_float)
{
    msgpack::zone z;
    msgpack::type::variant val1 = 12.34;
    BOOST_CHECK(val1.is_double());
    BOOST_CHECK(fabs(12.34 - val1.as_double()) <= kEPS);
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_double());
    BOOST_CHECK(fabs(12.34 - val2.as_double()) <= kEPS);
    BOOST_CHECK_NO_THROW(boost::get<double>(val2));
    BOOST_CHECK(fabs(val2.as_double() - val2.as_double()) <= kEPS);
}

// str

BOOST_AUTO_TEST_CASE(pack_convert_variant_str)
{
    std::stringstream ss;
    msgpack::type::variant val1 = "ABC";
    BOOST_CHECK(val1.is_string());
    BOOST_CHECK_EQUAL(val1.as_string(), "ABC");

    msgpack::pack(ss, val1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    msgpack::type::variant val2 = oh.get().as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_string());
    BOOST_CHECK_EQUAL(val2.as_string(), "ABC");
    BOOST_CHECK_NO_THROW(boost::get<std::string>(val2));
    BOOST_CHECK(val1 == val2);
}


BOOST_AUTO_TEST_CASE(object_with_zone_variant_str)
{
    msgpack::zone z;
    msgpack::type::variant val1 = "ABC";
    BOOST_CHECK(val1.is_string());
    BOOST_CHECK_EQUAL(val1.as_string(), "ABC");
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_string());
    BOOST_CHECK_EQUAL(val2.as_string(), "ABC");
    BOOST_CHECK_NO_THROW(boost::get<std::string>(val2));
    BOOST_CHECK(val1 == val2);
}

#if (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53

BOOST_AUTO_TEST_CASE(object_with_zone_variant_str_ref)
{
    // You can use boost::string_ref with msgpack::type::variant.
    msgpack::zone z;
    std::string s = "ABC";
    boost::string_ref sr(s);
    msgpack::type::variant val1(sr);
    BOOST_CHECK(val1.is_boost_string_ref());
    BOOST_CHECK_EQUAL(val1.as_boost_string_ref(), "ABC");
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    // Converted as std::string.
    BOOST_CHECK(val2.is_string());
    BOOST_CHECK_EQUAL(val2.as_string(), "ABC");
    BOOST_CHECK_NO_THROW(boost::get<std::string>(val2));
    // boost::string_ref and std::string are different.
    BOOST_CHECK(!(val1 == val2));
}

#endif // (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53

// bin

BOOST_AUTO_TEST_CASE(pack_convert_variant_bin)
{
    std::stringstream ss;
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    msgpack::type::variant val1 = v;
    BOOST_CHECK(val1.is_vector_char());
    BOOST_CHECK(val1.as_vector_char() == v);

    msgpack::pack(ss, val1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    msgpack::type::variant val2 = oh.get().as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_vector_char());
    BOOST_CHECK(val2.as_vector_char() == v);
    BOOST_CHECK_NO_THROW(boost::get<std::vector<char> >(val2));
    BOOST_CHECK(val1 == val2);
}



BOOST_AUTO_TEST_CASE(object_with_zone_variant_bin)
{
    msgpack::zone z;
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    msgpack::type::variant val1 = v;
    BOOST_CHECK(val1.is_vector_char());
    BOOST_CHECK(val1.as_vector_char() == v);
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_vector_char());
    BOOST_CHECK(val2.as_vector_char() == v);
    BOOST_CHECK_NO_THROW(boost::get<std::vector<char> >(val2));
    BOOST_CHECK(val1 == val2);
}

#if (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53

BOOST_AUTO_TEST_CASE(object_with_zone_variant_raw_ref)
{
    // You can use boost::string_ref with msgpack::type::variant.
    msgpack::zone z;
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    msgpack::type::raw_ref rr(&v.front(), static_cast<uint32_t>(v.size()));
    msgpack::type::variant val1 = rr;
    BOOST_CHECK(val1.is_raw_ref());
    BOOST_CHECK(val1.as_raw_ref() == msgpack::type::raw_ref(&v.front(), static_cast<uint32_t>(v.size())));
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    // Converted as std::vector<char>.
    BOOST_CHECK(val2.is_vector_char());
    BOOST_CHECK(val2.as_vector_char() == v);
    BOOST_CHECK_NO_THROW(boost::get<std::vector<char> >(val2));
     // msgpack::type::raw_ref and std::vector<char> are different.
    BOOST_CHECK(!(val1 == val2));
}

#endif // (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53

// ext

BOOST_AUTO_TEST_CASE(pack_convert_variant_ext)
{
    std::stringstream ss;
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    msgpack::type::ext e(42, v.data(), static_cast<uint32_t>(v.size()));
    msgpack::type::variant val1(e);
    BOOST_CHECK(val1.is_ext());
    BOOST_CHECK(val1.as_ext() == e);

    msgpack::pack(ss, val1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    msgpack::type::variant val2 = oh.get().as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_ext());
    BOOST_CHECK(val2.as_ext() == e);
    BOOST_CHECK_NO_THROW(boost::get<msgpack::type::ext>(val2));
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_variant_ext)
{
    msgpack::zone z;
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    msgpack::type::ext e(42, v.data(), static_cast<uint32_t>(v.size()));
    msgpack::type::variant val1(e);
    BOOST_CHECK(val1.is_ext());
    BOOST_CHECK(val1.as_ext() == e);
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_ext());
    BOOST_CHECK(val2.as_ext() == e);
    BOOST_CHECK_NO_THROW(boost::get<msgpack::type::ext>(val2));
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_variant_ext_ref)
{
    // You can use msgpack::type::ext_ref with msgpack::type::variant.
    msgpack::zone z;
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    msgpack::type::ext_ref e(v.data(), static_cast<uint32_t>(v.size()));
    msgpack::type::variant val1(e);
    BOOST_CHECK(val1.is_ext_ref());
    BOOST_CHECK(val1.as_ext_ref() == e);
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    // Converted as msgpack::type::ext.
    BOOST_CHECK(val2.is_ext());
    BOOST_CHECK(val2.as_ext() == msgpack::type::ext(e));
    BOOST_CHECK_NO_THROW(boost::get<msgpack::type::ext>(val2));
     // msgpack::type::ext_ref and msgpack::type::ext are different.
    BOOST_CHECK(!(val1 == val2));
}

// array

BOOST_AUTO_TEST_CASE(pack_convert_variant_array)
{
    std::stringstream ss;
    std::vector<msgpack::type::variant> v;
    v.push_back(msgpack::type::variant(1));
    v.push_back(msgpack::type::variant(-1));
    v.push_back(msgpack::type::variant("ABC"));
    msgpack::type::variant val1 = v;
    BOOST_CHECK(val1.is_vector());
    BOOST_CHECK(val1.as_vector() == v);

    msgpack::pack(ss, val1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    msgpack::type::variant val2 = oh.get().as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_vector());
    BOOST_CHECK(val2.as_vector() == v);
    BOOST_CHECK_NO_THROW(boost::get<std::vector<msgpack::type::variant> >(val2));
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_variant_array)
{
    msgpack::zone z;
    std::vector<msgpack::type::variant> v;
    v.push_back(msgpack::type::variant(1));
    v.push_back(msgpack::type::variant(-1));
    v.push_back(msgpack::type::variant("ABC"));
    msgpack::type::variant val1 = v;
    BOOST_CHECK(val1.is_vector());
    BOOST_CHECK(val1.as_vector() == v);
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_vector());
    BOOST_CHECK(val2.as_vector() == v);
    BOOST_CHECK_NO_THROW(boost::get<std::vector<msgpack::type::variant> >(val2));
    BOOST_CHECK(val1 == val2);
}

// multimap

BOOST_AUTO_TEST_CASE(pack_convert_variant_map)
{
    std::stringstream ss;
    typedef std::multimap<msgpack::type::variant, msgpack::type::variant> multimap_t;
    multimap_t v;
    v.insert(multimap_t::value_type(msgpack::type::variant(1), msgpack::type::variant(-1)));
    v.insert(multimap_t::value_type(msgpack::type::variant("ABC"), msgpack::type::variant("DEF")));
    msgpack::type::variant val1 = v;
    BOOST_CHECK(val1.is_multimap());
    BOOST_CHECK(val1.as_multimap() == v);

    msgpack::pack(ss, val1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    msgpack::type::variant val2 = oh.get().as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_multimap());
    BOOST_CHECK(val2.as_multimap() == v);
    BOOST_CHECK_NO_THROW(boost::get<multimap_t>(val2));
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_variant_map)
{
    msgpack::zone z;
    typedef std::multimap<msgpack::type::variant, msgpack::type::variant> multimap_t;
    multimap_t v;
    v.insert(multimap_t::value_type(msgpack::type::variant(1), msgpack::type::variant(-1)));
    v.insert(multimap_t::value_type(msgpack::type::variant("ABC"), msgpack::type::variant("DEF")));
    msgpack::type::variant val1 = v;
    BOOST_CHECK(val1.is_multimap());
    BOOST_CHECK(val1.as_multimap() == v);
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    BOOST_CHECK(val2.is_multimap());
    BOOST_CHECK(val2.as_multimap() == v);
    BOOST_CHECK_NO_THROW(boost::get<multimap_t>(val2));
    BOOST_CHECK(val1 == val2);
}

// variant_ref

// str

#if (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53

BOOST_AUTO_TEST_CASE(pack_convert_variant_ref_str)
{
    std::stringstream ss;
    std::string s("ABC");
    boost::string_ref sr(s);
    msgpack::type::variant_ref val1 = sr;
    BOOST_CHECK(val1.is_boost_string_ref());
    BOOST_CHECK_EQUAL(val1.as_boost_string_ref(), sr);

    msgpack::pack(ss, val1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    msgpack::type::variant_ref val2 = oh.get().as<msgpack::type::variant_ref>();
    BOOST_CHECK(val2.is_boost_string_ref());
    BOOST_CHECK_EQUAL(val2.as_boost_string_ref(), sr);
    BOOST_CHECK_NO_THROW(boost::get<boost::string_ref>(val2));
    BOOST_CHECK(val1 == val2);
}



BOOST_AUTO_TEST_CASE(object_with_zone_variant_ref_str)
{
    msgpack::zone z;
    std::string s("ABC");
    boost::string_ref sr(s);
    msgpack::type::variant_ref val1 = sr;
    BOOST_CHECK(val1.is_boost_string_ref());
    BOOST_CHECK_EQUAL(val1.as_boost_string_ref(), sr);
    msgpack::object obj(val1, z);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    BOOST_CHECK(val2.is_boost_string_ref());
    BOOST_CHECK_EQUAL(val2.as_boost_string_ref(), sr);
    BOOST_CHECK_NO_THROW(boost::get<boost::string_ref>(val2));
    BOOST_CHECK(val1 == val2);
}

#endif // (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53

// bin

BOOST_AUTO_TEST_CASE(pack_convert_variant_ref_bin)
{
    std::stringstream ss;
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    msgpack::type::raw_ref rr(v.data(), static_cast<uint32_t>(v.size()));
    msgpack::type::variant_ref val1 = rr;
    BOOST_CHECK(val1.is_raw_ref());
    BOOST_CHECK(val1.as_raw_ref() == rr);

    msgpack::pack(ss, val1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    msgpack::type::variant_ref val2 = oh.get().as<msgpack::type::variant_ref>();
    BOOST_CHECK(val2.is_raw_ref());
    BOOST_CHECK(val2.as_raw_ref() == rr);
    BOOST_CHECK_NO_THROW(boost::get<msgpack::type::raw_ref>(val2));
    BOOST_CHECK(val1 == val2);
}



BOOST_AUTO_TEST_CASE(object_with_zone_variant_ref_bin)
{
    msgpack::zone z;
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    msgpack::type::raw_ref rr(v.data(), static_cast<uint32_t>(v.size()));
    msgpack::type::variant_ref val1 = rr;
    BOOST_CHECK(val1.is_raw_ref());
    BOOST_CHECK(val1.as_raw_ref() == rr);
    msgpack::object obj(val1, z);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    BOOST_CHECK(val2.is_raw_ref());
    BOOST_CHECK(val2.as_raw_ref() == rr);
    BOOST_CHECK_NO_THROW(boost::get<msgpack::type::raw_ref>(val2));
    BOOST_CHECK(val1 == val2);
}

// ext

BOOST_AUTO_TEST_CASE(pack_convert_variant_ref_ext)
{
    std::stringstream ss;
    std::vector<char> v;
    v.push_back(static_cast<char>(42));
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    msgpack::type::ext_ref er(v.data(), static_cast<uint32_t>(v.size()));
    msgpack::type::variant_ref val1(er);
    BOOST_CHECK(val1.is_ext_ref());
    BOOST_CHECK(val1.as_ext_ref() == er);

    msgpack::pack(ss, val1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    msgpack::type::variant_ref val2 = oh.get().as<msgpack::type::variant_ref>();
    BOOST_CHECK_NO_THROW(boost::get<msgpack::type::ext_ref>(val2));
    BOOST_CHECK(val2.is_ext_ref());
    BOOST_CHECK(val2.as_ext_ref() == er);
    BOOST_CHECK(val1 == val2);
}


BOOST_AUTO_TEST_CASE(object_with_zone_variant_ref_ext)
{
    msgpack::zone z;
    std::vector<char> v;
    v.push_back(static_cast<char>(42));
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    msgpack::type::ext_ref er(v.data(), static_cast<uint32_t>(v.size()));
    msgpack::type::variant_ref val1(er);
    BOOST_CHECK(val1.is_ext_ref());
    BOOST_CHECK(val1.as_ext_ref() == er);
    msgpack::object obj(val1, z);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    BOOST_CHECK(val2.is_ext_ref());
    BOOST_CHECK(val2.as_ext_ref() == er);
    BOOST_CHECK_NO_THROW(boost::get<msgpack::type::ext_ref>(val2));
    BOOST_CHECK(val1 == val2);
}

// array

BOOST_AUTO_TEST_CASE(pack_convert_variant_ref_array)
{
    std::stringstream ss;
    std::vector<msgpack::type::variant_ref> v;
    v.push_back(msgpack::type::variant_ref(1));
    v.push_back(msgpack::type::variant_ref(-1));
    std::string s("ABC");
#if (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
    v.push_back(msgpack::type::variant_ref(boost::string_ref(s)));
#else  // (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
    v.push_back(msgpack::type::variant_ref(s));
#endif // (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
    msgpack::type::variant_ref val1 = v;
    BOOST_CHECK(val1.is_vector());
    BOOST_CHECK(val1.as_vector() == v);

    msgpack::pack(ss, val1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    msgpack::type::variant_ref val2 = oh.get().as<msgpack::type::variant_ref>();
    BOOST_CHECK(val2.is_vector());
    BOOST_CHECK(val2.as_vector() == v);
    BOOST_CHECK_NO_THROW(boost::get<std::vector<msgpack::type::variant_ref> >(val2));
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_variant_ref_array)
{
    msgpack::zone z;
    std::vector<msgpack::type::variant_ref> v;
    v.push_back(msgpack::type::variant_ref(1));
    v.push_back(msgpack::type::variant_ref(-1));
    std::string s("ABC");
#if (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
    v.push_back(msgpack::type::variant_ref(boost::string_ref(s)));
#else  // (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
    v.push_back(msgpack::type::variant_ref(s));
#endif // (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
    msgpack::type::variant_ref val1 = v;
    BOOST_CHECK(val1.is_vector());
    BOOST_CHECK(val1.as_vector() == v);
    msgpack::object obj(val1, z);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    BOOST_CHECK(val2.is_vector());
    BOOST_CHECK(val2.as_vector() == v);
    BOOST_CHECK_NO_THROW(boost::get<std::vector<msgpack::type::variant_ref> >(val2));
    BOOST_CHECK(val1 == val2);
}

// multimap

BOOST_AUTO_TEST_CASE(pack_convert_variant_ref_map)
{
    std::stringstream ss;
    typedef std::multimap<msgpack::type::variant_ref, msgpack::type::variant_ref> multimap_t;
    multimap_t v;
    v.insert(multimap_t::value_type(msgpack::type::variant_ref(1), msgpack::type::variant_ref(-1)));
    std::string s1("ABC");
    std::string s2("DEF");
#if (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
    v.insert(multimap_t::value_type(msgpack::type::variant_ref(boost::string_ref(s1)), msgpack::type::variant_ref(boost::string_ref(s2))));
#else  // (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
    v.insert(multimap_t::value_type(msgpack::type::variant_ref(s1), msgpack::type::variant_ref(s2)));
#endif // (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
    msgpack::type::variant_ref val1 = v;
    BOOST_CHECK(val1.is_multimap());
    BOOST_CHECK(val1.as_multimap() == v);

    msgpack::pack(ss, val1);

    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    msgpack::type::variant_ref val2 = oh.get().as<msgpack::type::variant_ref>();
    BOOST_CHECK(val2.is_multimap());
    BOOST_CHECK(val2.as_multimap() == v);
    BOOST_CHECK_NO_THROW(boost::get<multimap_t>(val2));
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(object_with_zone_variant_ref_map)
{
    msgpack::zone z;
    typedef std::multimap<msgpack::type::variant_ref, msgpack::type::variant_ref> multimap_t;
    multimap_t v;
    v.insert(multimap_t::value_type(msgpack::type::variant_ref(1), msgpack::type::variant_ref(-1)));
    std::string s1("ABC");
    std::string s2("DEF");
#if (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
    v.insert(multimap_t::value_type(msgpack::type::variant_ref(boost::string_ref(s1)), msgpack::type::variant_ref(boost::string_ref(s2))));
#else  // (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
    v.insert(multimap_t::value_type(msgpack::type::variant_ref(s1), msgpack::type::variant_ref(s2)));
#endif // (BOOST_VERSION / 100000) >= 1 && ((BOOST_VERSION / 100) % 1000) >= 53
    msgpack::type::variant_ref val1 = v;
    BOOST_CHECK(val1.is_multimap());
    BOOST_CHECK(val1.as_multimap() == v);
    msgpack::object obj(val1, z);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    BOOST_CHECK(val2.is_multimap());
    BOOST_CHECK(val2.as_multimap() == v);
    BOOST_CHECK_NO_THROW(boost::get<multimap_t>(val2));
    BOOST_CHECK(val1 == val2);
}
