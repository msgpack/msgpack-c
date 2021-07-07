#include <msgpack.hpp>

#define BOOST_TEST_MODULE MSGPACK_CPP17
#include <boost/test/unit_test.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// For C++ standards lower than C++17
BOOST_AUTO_TEST_CASE(dummy)
{
}

#if MSGPACK_CPP_VERSION >= 201703

// C++17

#if MSGPACK_HAS_INCLUDE(<optional>)

BOOST_AUTO_TEST_CASE(optional_pack_convert_nil)
{
    std::stringstream ss;
    std::optional<int> val1;
    msgpack::pack(ss, val1);
    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    std::optional<int> val2 = oh.get().as<std::optional<int> >();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(optional_pack_convert_int)
{
    std::stringstream ss;
    std::optional<int> val1 = 1;
    msgpack::pack(ss, val1);
    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    std::optional<int> val2 = oh.get().as<std::optional<int> >();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(optional_pack_convert_vector)
{
    typedef std::optional<std::vector<int> > ovi_t;
    std::stringstream ss;
    ovi_t val1;
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    val1 = v;
    msgpack::pack(ss, val1);
    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    ovi_t  val2 = oh.get().as<ovi_t>();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(optional_pack_convert_vector_optional)
{
    typedef std::vector<std::optional<int> > voi_t;
    std::stringstream ss;
    voi_t val1;
    val1.resize(3);
    val1[0] = 1;
    val1[2] = 3;
    msgpack::pack(ss, val1);
    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    voi_t  val2 = oh.get().as<voi_t>();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(optional_object_nil)
{
    std::optional<int> val1;
    msgpack::object obj(val1);
    std::optional<int> val2 = obj.as<std::optional<int> >();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(optional_object_int)
{
    std::optional<int> val1 = 1;
    msgpack::object obj(val1);
    std::optional<int> val2 = obj.as<std::optional<int> >();
    BOOST_CHECK(val1 == val2);
}

// Compile error as expected
/*
  BOOST_AUTO_TEST_CASE(optional_object_vector)
  {
  typedef std::optional<std::vector<int> > ovi_t;
  ovi_t val1;
  std::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  val1 = v;
  msgpack::object obj(val1);
  ovi_t  val2 = obj.as<ovi_t>();
  BOOST_CHECK(val1 == val2);
  }
*/

BOOST_AUTO_TEST_CASE(optional_object_with_zone_nil)
{
    msgpack::zone z;
    std::optional<int> val1;
    msgpack::object obj(val1, z);
    std::optional<int> val2 = obj.as<std::optional<int> >();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(optional_object_with_zone_int)
{
    msgpack::zone z;
    std::optional<int> val1 = 1;
    msgpack::object obj(val1, z);
    std::optional<int> val2 = obj.as<std::optional<int> >();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(optional_object_with_zone_vector_optional)
{
    typedef std::vector<std::optional<int> > voi_t;
    msgpack::zone z;
    voi_t val1;
    val1.resize(3);
    val1[0] = 1;
    val1[2] = 3;
    msgpack::object obj(val1, z);
    voi_t  val2 = obj.as<voi_t>();
    BOOST_CHECK(val1 == val2);
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

BOOST_AUTO_TEST_CASE(optional_pack_convert_no_def_con)
{
    std::stringstream ss;
    std::optional<no_def_con> val1 = no_def_con(1);
    msgpack::pack(ss, val1);
    std::string const& str = ss.str();
    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());
    std::optional<no_def_con> val2 = oh.get().as<std::optional<no_def_con>>();
    BOOST_CHECK(val1 == val2);
}

#endif // MSGPACK_HAS_INCLUDE(<optional>)

#if MSGPACK_HAS_INCLUDE(<string_view>)

BOOST_AUTO_TEST_CASE(string_view_pack_convert)
{
    std::stringstream ss;
    std::string s = "ABC";
    std::string_view val1(s);

    msgpack::pack(ss, val1);

    std::string const& str = ss.str();
    msgpack::object_handle oh;
    msgpack::unpack(oh, str.data(), str.size());
    std::string_view val2 = oh.get().as<std::string_view>();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(string_view_object)
{
    std::string s = "ABC";
    std::string_view val1(s);
    msgpack::object obj(val1);
    std::string_view val2 = obj.as<std::string_view>();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(string_view_object_with_zone)
{
    msgpack::zone z;
    std::string s = "ABC";
    std::string_view val1(s);
    msgpack::object obj(val1, z);
    std::string_view val2 = obj.as<std::string_view>();
    BOOST_CHECK(val1 == val2);
}

#endif // MSGPACK_HAS_INCLUDE(<string_view>)

BOOST_AUTO_TEST_CASE(byte_pack_convert)
{
    std::stringstream ss;
    std::byte val1{0xff};

    msgpack::pack(ss, val1);

    msgpack::object_handle oh;
    std::string const& str = ss.str();
    msgpack::unpack(oh, str.data(), str.size());
    std::byte val2 = oh.get().as<std::byte>();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(byte_object)
{
    std::byte val1{0x00};
    msgpack::object obj(val1);
    std::byte val2 = obj.as<std::byte>();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(byte_object_with_zone)
{
    msgpack::zone z;
    std::byte val1{80};
    msgpack::object obj(val1, z);
    std::byte val2 = obj.as<std::byte>();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(vector_byte_pack_convert)
{
    std::stringstream ss;
    std::vector<std::byte> val1{
        std::byte{0x01}, std::byte{0x02}, std::byte{0x7f}, std::byte{0x80}, std::byte{0xff}
    };

    msgpack::pack(ss, val1);
    std::string const& str = ss.str();

    char packed[] = { char(0xc4), char(0x05), char(0x01), char(0x02), char(0x7f), char(0x80), char(0xff) };
    BOOST_CHECK_EQUAL(str.size(), sizeof(packed));
    for (size_t i = 0; i != sizeof(packed); ++i) {
        BOOST_CHECK_EQUAL(str[i], packed[i]);
    }

    msgpack::object_handle oh;
    msgpack::unpack(oh, str.data(), str.size());
    std::vector<std::byte> val2 = oh.get().as<std::vector<std::byte>>();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(vector_byte_object)
{
    std::vector<std::byte> val1{
        std::byte{0x01}, std::byte{0x02}, std::byte{0x7f}, std::byte{0x80}, std::byte{0xff}
    };

    // Caller need to manage val1's lifetime. The Data is not copied.
    msgpack::object obj(val1);

    std::vector<std::byte> val2 = obj.as<std::vector<std::byte>>();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(vector_byte_object_with_zone)
{
    msgpack::zone z;
    std::vector<std::byte> val1{
        std::byte{0x01}, std::byte{0x02}, std::byte{0x7f}, std::byte{0x80}, std::byte{0xff}
    };
    msgpack::object obj(val1, z);

    std::vector<std::byte> val2 = obj.as<std::vector<std::byte>>();
    BOOST_CHECK(val1 == val2);
}

BOOST_AUTO_TEST_CASE(array_byte_pack_convert)
{
    std::stringstream ss;
    std::array<std::byte, 5> val1{
        std::byte{0x01}, std::byte{0x02}, std::byte{0x7f}, std::byte{0x80}, std::byte{0xff}
    };

    msgpack::pack(ss, val1);
    std::string const& str = ss.str();

    char packed[] = { char(0xc4), char(0x05), char(0x01), char(0x02), char(0x7f), char(0x80), char(0xff) };
    BOOST_CHECK_EQUAL(str.size(), sizeof(packed));
    for (size_t i = 0; i != sizeof(packed); ++i) {
        BOOST_CHECK_EQUAL(str[i], packed[i]);
    }

    {
        msgpack::object_handle oh;
        msgpack::unpack(oh, str.data(), str.size());
        auto val2 = oh.get().as<std::array<std::byte, 5>>();
        BOOST_CHECK(val1 == val2);
    }
    {
        msgpack::object_handle oh;
        msgpack::unpack(oh, str.data(), str.size());
        BOOST_CHECK_THROW((oh.get().as<std::array<std::byte, 0>>()),    msgpack::type_error);
        BOOST_CHECK_THROW((oh.get().as<std::array<std::byte, 1>>()),    msgpack::type_error);
        BOOST_CHECK_THROW((oh.get().as<std::array<std::byte, 8192>>()), msgpack::type_error);
    }
}

BOOST_AUTO_TEST_CASE(array_byte_object)
{
    std::array<std::byte, 5> val1{
        std::byte{0x01}, std::byte{0x02}, std::byte{0x7f}, std::byte{0x80}, std::byte{0xff}
    };

    // Caller need to manage val1's lifetime. The Data is not copied.
    msgpack::object obj(val1);

    auto val2 = obj.as<std::array<std::byte, 5>>();
    BOOST_CHECK(val1 == val2);

    BOOST_CHECK_THROW((obj.as<std::array<std::byte, 0>>()),    msgpack::type_error);
    BOOST_CHECK_THROW((obj.as<std::array<std::byte, 1>>()),    msgpack::type_error);
    BOOST_CHECK_THROW((obj.as<std::array<std::byte, 8192>>()), msgpack::type_error);
}

BOOST_AUTO_TEST_CASE(array_byte_object_with_zone)
{
    msgpack::zone z;
    std::array<std::byte, 5> val1{
        std::byte{0x01}, std::byte{0x02}, std::byte{0x7f}, std::byte{0x80}, std::byte{0xff}
    };
    msgpack::object obj(val1, z);

    auto val2 = obj.as<std::array<std::byte, 5>>();
    BOOST_CHECK(val1 == val2);

    BOOST_CHECK_THROW((obj.as<std::array<std::byte, 0>>()),    msgpack::type_error);
    BOOST_CHECK_THROW((obj.as<std::array<std::byte, 1>>()),    msgpack::type_error);
    BOOST_CHECK_THROW((obj.as<std::array<std::byte, 8192>>()), msgpack::type_error);
}

BOOST_AUTO_TEST_CASE(array_byte_empty_pack_convert)
{
    std::stringstream ss;
    std::array<std::byte, 0> val1{};

    msgpack::pack(ss, val1);
    std::string const& str = ss.str();

    char packed[] = { char(0xc4), char(0x00) };
    BOOST_CHECK_EQUAL(str.size(), sizeof(packed));
    for (size_t i = 0; i != sizeof(packed); ++i) {
        BOOST_CHECK_EQUAL(str[i], packed[i]);
    }

    {
        msgpack::object_handle oh;
        msgpack::unpack(oh, str.data(), str.size());
        auto val2 = oh.get().as<std::array<std::byte, 0>>();
        BOOST_CHECK(val1 == val2);
    }
    {
        msgpack::object_handle oh;
        msgpack::unpack(oh, str.data(), str.size());
        BOOST_CHECK_THROW((oh.get().as<std::array<std::byte, 1>>()),    msgpack::type_error);
        BOOST_CHECK_THROW((oh.get().as<std::array<std::byte, 8192>>()), msgpack::type_error);
    }
}

BOOST_AUTO_TEST_CASE(array_byte_empty_object)
{
    std::array<std::byte, 0> val1{};

    // Caller need to manage val1's lifetime. The Data is not copied.
    msgpack::object obj(val1);

    auto val2 = obj.as<std::array<std::byte, 0>>();
    BOOST_CHECK(val1 == val2);

    BOOST_CHECK_THROW((obj.as<std::array<std::byte, 1>>()),    msgpack::type_error);
    BOOST_CHECK_THROW((obj.as<std::array<std::byte, 8192>>()), msgpack::type_error);
}

BOOST_AUTO_TEST_CASE(array_byte_empty_object_with_zone)
{
    msgpack::zone z;
    std::array<std::byte, 0> val1{};
    msgpack::object obj(val1, z);

    auto val2 = obj.as<std::array<std::byte, 0>>();
    BOOST_CHECK(val1 == val2);

    BOOST_CHECK_THROW((obj.as<std::array<std::byte, 1>>()),    msgpack::type_error);
    BOOST_CHECK_THROW((obj.as<std::array<std::byte, 8192>>()), msgpack::type_error);
}

BOOST_AUTO_TEST_CASE(carray_byte_pack_convert)
{
    std::stringstream ss;
    std::byte val1[] = {
        std::byte{0x01}, std::byte{0x02}, std::byte{0x7f}, std::byte{0x80}, std::byte{0xff}
    };

    msgpack::pack(ss, val1);
    std::string const& str = ss.str();

    char packed[] = { char(0xc4), char(0x05), char(0x01), char(0x02), char(0x7f), char(0x80), char(0xff) };
    BOOST_CHECK_EQUAL(str.size(), sizeof(packed));
    for (size_t i = 0; i != sizeof(packed); ++i) {
        BOOST_CHECK_EQUAL(str[i], packed[i]);
    }

    msgpack::object_handle oh;
    msgpack::unpack(oh, str.data(), str.size());
    std::byte val2[sizeof(val1)];
    oh.get().convert(val2);
    for (size_t i = 0; i != sizeof(val1); ++i) {
        BOOST_CHECK(val1[i] == val2[i]);
    }
}

BOOST_AUTO_TEST_CASE(carray_byte_object_with_zone)
{
    msgpack::zone z;
    std::byte val1[] = {
        std::byte{0x01}, std::byte{0x02}, std::byte{0x7f}, std::byte{0x80}, std::byte{0xff}
    };
    msgpack::object obj(val1, z);

    std::byte val2[sizeof(val1)];
    obj.convert(val2);
    for (size_t i = 0; i != sizeof(val1); ++i) {
        BOOST_CHECK(val1[i] == val2[i]);
    }
}

#endif // MSGPACK_CPP_VERSION >= 201703
