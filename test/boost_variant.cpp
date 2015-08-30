#include <msgpack.hpp>
#include <sstream>
#include <iterator>
#include <gtest/gtest.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if defined(MSGPACK_USE_BOOST)

// nil

TEST(MSGPACK_BOOST, pack_convert_variant_nil)
{
    std::stringstream ss;
    msgpack::type::variant val1 = msgpack::type::nil();

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant val2 = ret.get().as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<msgpack::type::nil>(val2));
}

TEST(MSGPACK_BOOST, object_variant_nil)
{
    msgpack::type::variant val1 = msgpack::type::nil();
    msgpack::object obj(val1);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<msgpack::type::nil>(val2));
}

TEST(MSGPACK_BOOST, object_with_zone_variant_nil)
{
    msgpack::zone z;
    msgpack::type::variant val1 = msgpack::type::nil();
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<msgpack::type::nil>(val2));
}

// nil (default constructor)

TEST(MSGPACK_BOOST, pack_convert_variant_nil_default)
{
    std::stringstream ss;
    msgpack::type::variant val1;

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant val2 = ret.get().as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<msgpack::type::nil>(val2));
}

TEST(MSGPACK_BOOST, object_variant_nil_default)
{
    msgpack::type::variant val1;
    msgpack::object obj(val1);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<msgpack::type::nil>(val2));
}

TEST(MSGPACK_BOOST, object_with_zone_variant_nil_default)
{
    msgpack::zone z;
    msgpack::type::variant val1;
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<msgpack::type::nil>(val2));
}

// bool

TEST(MSGPACK_BOOST, pack_convert_variant_bool)
{
    std::stringstream ss;
    msgpack::type::variant val1 = true;

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant val2 = ret.get().as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<bool>(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_variant_bool)
{
    msgpack::type::variant val1 = true;
    msgpack::object obj(val1);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<bool>(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_with_zone_variant_bool)
{
    msgpack::zone z;
    msgpack::type::variant val1 = true;
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<bool>(val2));
    EXPECT_TRUE(val1 == val2);
}

// positive integer

TEST(MSGPACK_BOOST, pack_convert_variant_positive_integer)
{
    std::stringstream ss;
    msgpack::type::variant val1 = 123;

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant val2 = ret.get().as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<uint64_t>(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_variant_positive_integer)
{
    msgpack::type::variant val1 = 123;
    msgpack::object obj(val1);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<uint64_t>(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_with_zone_variant_positive_integer)
{
    msgpack::zone z;
    msgpack::type::variant val1 = 123;
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<uint64_t>(val2));
    EXPECT_TRUE(val1 == val2);
}

// negative integer

TEST(MSGPACK_BOOST, pack_convert_variant_negative_integer)
{
    std::stringstream ss;
    msgpack::type::variant val1 = -123;

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant val2 = ret.get().as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<int64_t>(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_variant_negative_integer)
{
    msgpack::type::variant val1 = -123;
    msgpack::object obj(val1);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<int64_t>(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_with_zone_variant_negative_integer)
{
    msgpack::zone z;
    msgpack::type::variant val1 = -123;
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<int64_t>(val2));
    EXPECT_TRUE(val1 == val2);
}

// float

TEST(MSGPACK_BOOST, pack_convert_variant_float)
{
    std::stringstream ss;
    msgpack::type::variant val1 = 12.34;

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant val2 = ret.get().as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<double>(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_variant_float)
{
    msgpack::type::variant val1 = 12.34;
    msgpack::object obj(val1);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<double>(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_with_zone_variant_float)
{
    msgpack::zone z;
    msgpack::type::variant val1 = 12.34;
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<double>(val2));
    EXPECT_TRUE(val1 == val2);
}

// str

TEST(MSGPACK_BOOST, pack_convert_variant_str)
{
    std::stringstream ss;
    msgpack::type::variant val1 = "ABC";

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant val2 = ret.get().as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<std::string>(val2));
    EXPECT_TRUE(val1 == val2);
}



TEST(MSGPACK_BOOST, object_with_zone_variant_str)
{
    msgpack::zone z;
    msgpack::type::variant val1 = "ABC";
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<std::string>(val2));
    EXPECT_TRUE(val1 == val2);
}

// bin

TEST(MSGPACK_BOOST, pack_convert_variant_bin)
{
    std::stringstream ss;
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    msgpack::type::variant val1 = v;

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant val2 = ret.get().as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<std::vector<char> >(val2));
    EXPECT_TRUE(val1 == val2);
}



TEST(MSGPACK_BOOST, object_with_zone_variant_bin)
{
    msgpack::zone z;
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    msgpack::type::variant val1 = v;
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<std::vector<char> >(val2));
    EXPECT_TRUE(val1 == val2);
}

// ext

TEST(MSGPACK_BOOST, pack_convert_variant_ext)
{
    std::stringstream ss;
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    msgpack::type::variant val1(msgpack::type::ext(42, v.data(), v.size()));

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant val2 = ret.get().as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<msgpack::type::ext>(val2));
    EXPECT_TRUE(val1 == val2);
}



TEST(MSGPACK_BOOST, object_with_zone_variant_ext)
{
    msgpack::zone z;
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    msgpack::type::variant val1(msgpack::type::ext(42, v.data(), v.size()));
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<msgpack::type::ext>(val2));
    EXPECT_TRUE(val1 == val2);
}

// array

TEST(MSGPACK_BOOST, pack_convert_variant_array)
{
    std::stringstream ss;
    std::vector<msgpack::type::variant> v;
    v.push_back(msgpack::type::variant(1));
    v.push_back(msgpack::type::variant(-1));
    v.push_back(msgpack::type::variant(23.4));
    v.push_back(msgpack::type::variant("ABC"));
    msgpack::type::variant val1 = v;

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant val2 = ret.get().as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<std::vector<msgpack::type::variant> >(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_with_zone_variant_array)
{
    msgpack::zone z;
    std::vector<msgpack::type::variant> v;
    v.push_back(msgpack::type::variant(1));
    v.push_back(msgpack::type::variant(-1));
    v.push_back(msgpack::type::variant(23.4));
    v.push_back(msgpack::type::variant("ABC"));
    msgpack::type::variant val1 = v;
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<std::vector<msgpack::type::variant> >(val2));
    EXPECT_TRUE(val1 == val2);
}

// multimap

TEST(MSGPACK_BOOST, pack_convert_variant_map)
{
    std::stringstream ss;
    typedef std::multimap<msgpack::type::variant, msgpack::type::variant> multimap_t;
    multimap_t v;
    v.insert(multimap_t::value_type(msgpack::type::variant(1), msgpack::type::variant(-1)));
    v.insert(multimap_t::value_type(msgpack::type::variant(23.4), msgpack::type::variant("ABC")));
    msgpack::type::variant val1 = v;

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant val2 = ret.get().as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<multimap_t>(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_with_zone_variant_map)
{
    msgpack::zone z;
    typedef std::multimap<msgpack::type::variant, msgpack::type::variant> multimap_t;
    multimap_t v;
    v.insert(multimap_t::value_type(msgpack::type::variant(1), msgpack::type::variant(-1)));
    v.insert(multimap_t::value_type(msgpack::type::variant(23.4), msgpack::type::variant("ABC")));
    msgpack::type::variant val1 = v;
    msgpack::object obj(val1, z);
    msgpack::type::variant val2 = obj.as<msgpack::type::variant>();
    EXPECT_NO_THROW(boost::get<multimap_t>(val2));
    EXPECT_TRUE(val1 == val2);
}

// variant_ref

// nil

TEST(MSGPACK_BOOST, pack_convert_variant_ref_nil)
{
    std::stringstream ss;
    msgpack::type::variant_ref val1 = msgpack::type::nil();

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant_ref val2 = ret.get().as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<msgpack::type::nil>(val2));
}

TEST(MSGPACK_BOOST, object_variant_ref_nil)
{
    msgpack::type::variant_ref val1 = msgpack::type::nil();
    msgpack::object obj(val1);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<msgpack::type::nil>(val2));
}

TEST(MSGPACK_BOOST, object_with_zone_variant_ref_nil)
{
    msgpack::zone z;
    msgpack::type::variant_ref val1 = msgpack::type::nil();
    msgpack::object obj(val1, z);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<msgpack::type::nil>(val2));
}

// nil (default constructor)

TEST(MSGPACK_BOOST, pack_convert_variant_ref_nil_default)
{
    std::stringstream ss;
    msgpack::type::variant_ref val1;

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant_ref val2 = ret.get().as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<msgpack::type::nil>(val2));
}

TEST(MSGPACK_BOOST, object_variant_ref_nil_default)
{
    msgpack::type::variant_ref val1;
    msgpack::object obj(val1);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<msgpack::type::nil>(val2));
}

TEST(MSGPACK_BOOST, object_with_zone_variant_ref_nil_default)
{
    msgpack::zone z;
    msgpack::type::variant_ref val1;
    msgpack::object obj(val1, z);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<msgpack::type::nil>(val2));
}

// bool

TEST(MSGPACK_BOOST, pack_convert_variant_ref_bool)
{
    std::stringstream ss;
    msgpack::type::variant_ref val1 = true;

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant_ref val2 = ret.get().as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<bool>(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_variant_ref_bool)
{
    msgpack::type::variant_ref val1 = true;
    msgpack::object obj(val1);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<bool>(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_with_zone_variant_ref_bool)
{
    msgpack::zone z;
    msgpack::type::variant_ref val1 = true;
    msgpack::object obj(val1, z);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<bool>(val2));
    EXPECT_TRUE(val1 == val2);
}

// positive integer

TEST(MSGPACK_BOOST, pack_convert_variant_ref_positive_integer)
{
    std::stringstream ss;
    msgpack::type::variant_ref val1 = 123;

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant_ref val2 = ret.get().as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<uint64_t>(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_variant_ref_positive_integer)
{
    msgpack::type::variant_ref val1 = 123;
    msgpack::object obj(val1);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<uint64_t>(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_with_zone_variant_ref_positive_integer)
{
    msgpack::zone z;
    msgpack::type::variant_ref val1 = 123;
    msgpack::object obj(val1, z);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<uint64_t>(val2));
    EXPECT_TRUE(val1 == val2);
}

// negative integer

TEST(MSGPACK_BOOST, pack_convert_variant_ref_negative_integer)
{
    std::stringstream ss;
    msgpack::type::variant_ref val1 = -123;

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant_ref val2 = ret.get().as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<int64_t>(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_variant_ref_negative_integer)
{
    msgpack::type::variant_ref val1 = -123;
    msgpack::object obj(val1);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<int64_t>(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_with_zone_variant_ref_negative_integer)
{
    msgpack::zone z;
    msgpack::type::variant_ref val1 = -123;
    msgpack::object obj(val1, z);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<int64_t>(val2));
    EXPECT_TRUE(val1 == val2);
}

// float

TEST(MSGPACK_BOOST, pack_convert_variant_ref_float)
{
    std::stringstream ss;
    msgpack::type::variant_ref val1 = 12.34;

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant_ref val2 = ret.get().as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<double>(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_variant_ref_float)
{
    msgpack::type::variant_ref val1 = 12.34;
    msgpack::object obj(val1);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<double>(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_with_zone_variant_ref_float)
{
    msgpack::zone z;
    msgpack::type::variant_ref val1 = 12.34;
    msgpack::object obj(val1, z);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<double>(val2));
    EXPECT_TRUE(val1 == val2);
}

// str

TEST(MSGPACK_BOOST, pack_convert_variant_ref_str)
{
    std::stringstream ss;
    std::string s("ABC");
    msgpack::type::variant_ref val1 = boost::string_ref(s);

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant_ref val2 = ret.get().as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<boost::string_ref>(val2));
    EXPECT_TRUE(val1 == val2);
}



TEST(MSGPACK_BOOST, object_with_zone_variant_ref_str)
{
    msgpack::zone z;
    std::string s("ABC");
    msgpack::type::variant_ref val1 = boost::string_ref(s);
    msgpack::object obj(val1, z);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<boost::string_ref>(val2));
    EXPECT_TRUE(val1 == val2);
}

// bin

TEST(MSGPACK_BOOST, pack_convert_variant_ref_bin)
{
    std::stringstream ss;
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    msgpack::type::variant_ref val1 = msgpack::type::raw_ref(v.data(), v.size());

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant_ref val2 = ret.get().as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<msgpack::type::raw_ref>(val2));
    EXPECT_TRUE(val1 == val2);
}



TEST(MSGPACK_BOOST, object_with_zone_variant_ref_bin)
{
    msgpack::zone z;
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    msgpack::type::variant_ref val1 = msgpack::type::raw_ref(v.data(), v.size());
    msgpack::object obj(val1, z);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<msgpack::type::raw_ref>(val2));
    EXPECT_TRUE(val1 == val2);
}

// ext

TEST(MSGPACK_BOOST, pack_convert_variant_ref_ext)
{
    std::stringstream ss;
    std::vector<char> v;
    v.push_back(static_cast<char>(42));
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    msgpack::type::variant_ref val1(msgpack::type::ext_ref(v.data(), v.size()));

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant_ref val2 = ret.get().as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<msgpack::type::ext_ref>(val2));
    EXPECT_TRUE(val1 == val2);
}



TEST(MSGPACK_BOOST, object_with_zone_variant_ref_ext)
{
    msgpack::zone z;
    std::vector<char> v;
    v.push_back(static_cast<char>(42));
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    msgpack::type::variant_ref val1(msgpack::type::ext_ref(v.data(), v.size()));
    msgpack::object obj(val1, z);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<msgpack::type::ext_ref>(val2));
    EXPECT_TRUE(val1 == val2);
}

// array

TEST(MSGPACK_BOOST, pack_convert_variant_ref_array)
{
    std::stringstream ss;
    std::vector<msgpack::type::variant_ref> v;
    v.push_back(msgpack::type::variant_ref(1));
    v.push_back(msgpack::type::variant_ref(-1));
    v.push_back(msgpack::type::variant_ref(23.4));
    std::string s("ABC");
    v.push_back(msgpack::type::variant_ref(boost::string_ref(s)));
    msgpack::type::variant_ref val1 = v;

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant_ref val2 = ret.get().as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<std::vector<msgpack::type::variant_ref> >(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_with_zone_variant_ref_array)
{
    msgpack::zone z;
    std::vector<msgpack::type::variant_ref> v;
    v.push_back(msgpack::type::variant_ref(1));
    v.push_back(msgpack::type::variant_ref(-1));
    v.push_back(msgpack::type::variant_ref(23.4));
    std::string s("ABC");
    v.push_back(msgpack::type::variant_ref(boost::string_ref(s)));
    msgpack::type::variant_ref val1 = v;
    msgpack::object obj(val1, z);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<std::vector<msgpack::type::variant_ref> >(val2));
    EXPECT_TRUE(val1 == val2);
}

// multimap

TEST(MSGPACK_BOOST, pack_convert_variant_ref_map)
{
    std::stringstream ss;
    typedef std::multimap<msgpack::type::variant_ref, msgpack::type::variant_ref> multimap_t;
    multimap_t v;
    v.insert(multimap_t::value_type(msgpack::type::variant_ref(1), msgpack::type::variant_ref(-1)));
    std::string s("ABC");
    v.insert(multimap_t::value_type(msgpack::type::variant_ref(23.4), msgpack::type::variant_ref(boost::string_ref(s))));
    msgpack::type::variant_ref val1 = v;

    msgpack::pack(ss, val1);

    msgpack::unpacked ret;
    msgpack::unpack(ret, ss.str().data(), ss.str().size());
    msgpack::type::variant_ref val2 = ret.get().as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<multimap_t>(val2));
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_BOOST, object_with_zone_variant_ref_map)
{
    msgpack::zone z;
    typedef std::multimap<msgpack::type::variant_ref, msgpack::type::variant_ref> multimap_t;
    multimap_t v;
    v.insert(multimap_t::value_type(msgpack::type::variant_ref(1), msgpack::type::variant_ref(-1)));
    std::string s("ABC");
    v.insert(multimap_t::value_type(msgpack::type::variant_ref(23.4), msgpack::type::variant_ref(boost::string_ref(s))));
    msgpack::type::variant_ref val1 = v;
    msgpack::object obj(val1, z);
    msgpack::type::variant_ref val2 = obj.as<msgpack::type::variant_ref>();
    EXPECT_NO_THROW(boost::get<multimap_t>(val2));
    EXPECT_TRUE(val1 == val2);
}

#endif // defined(MSGPACK_USE_BOOST)
