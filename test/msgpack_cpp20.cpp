#include <msgpack.hpp>

#define BOOST_TEST_MODULE MSGPACK_CPP20
#include <boost/test/unit_test.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// For C++ standards lower than C++20
BOOST_AUTO_TEST_CASE(dummy)
{
}

// Some compilers still do not set the corresponding macro to 202002
#if MSGPACK_CPP_VERSION > 201703

#if MSGPACK_HAS_INCLUDE(<span>)

template <typename Byte>
bool operator==(const std::span<const Byte>& lhs, const std::span<const Byte>& rhs)
{
    if (lhs.size() != rhs.size())
        return false;
    for (std::size_t i = 0; i < lhs.size(); ++i)
        if (lhs[i] != rhs[i])
            return false;
    return true;
}

template <typename Byte>
bool operator==(const std::span<const Byte>& lhs, const std::vector<Byte>& rhs)
{
    return lhs == std::span<const Byte>{rhs.data(), rhs.size()};
}

template <typename Byte>
bool operator==(const std::vector<Byte>& lhs, const std::span<const Byte>& rhs)
{
    return std::span<const Byte>{lhs.data(), lhs.size()} == rhs;
}

#define MSGPACK_TEST_SPAN_BYTE_PACK_CONVERT(byte_t, display_name)                                               \
    BOOST_AUTO_TEST_CASE(span_##display_name##_pack_convert)                                                    \
    {                                                                                                           \
        std::stringstream ss;                                                                                   \
        byte_t raw_data[] = {                                                                                   \
            (byte_t)(0x01), (byte_t)(0x02), (byte_t)(0x7f), (byte_t)(0x80), (byte_t)(0xff)                      \
        };                                                                                                      \
        std::span<const byte_t> val1{raw_data, sizeof(raw_data)};                                               \
                                                                                                                \
        msgpack::pack(ss, val1);                                                                                \
        std::string const& str = ss.str();                                                                      \
                                                                                                                \
        char packed[] = { char(0xc4), char(0x05), char(0x01), char(0x02), char(0x7f), char(0x80), char(0xff) }; \
        BOOST_CHECK_EQUAL(str.size(), sizeof(packed));                                                          \
        for (size_t i = 0; i != sizeof(packed); ++i) {                                                          \
            BOOST_CHECK_EQUAL(str[i], packed[i]);                                                               \
        }                                                                                                       \
                                                                                                                \
        msgpack::object_handle oh;                                                                              \
        msgpack::unpack(oh, str.data(), str.size());                                                            \
        {                                                                                                       \
            auto val2 = oh.get().as<std::vector<byte_t>>();                                                     \
            BOOST_CHECK(val1 == val2);                                                                          \
        }                                                                                                       \
        {                                                                                                       \
            auto val2 = oh.get().as<std::span<const byte_t>>();                                                 \
            BOOST_CHECK(val1 == val2);                                                                          \
        }                                                                                                       \
    }                                                                                                           \

#define MSGPACK_TEST_SPAN_BYTE_OBJECT(byte_t, display_name)                                                     \
    BOOST_AUTO_TEST_CASE(span_##display_name##_object)                                                          \
    {                                                                                                           \
        byte_t raw_data[] = {                                                                                   \
            (byte_t)(0x01), (byte_t)(0x02), (byte_t)(0x7f), (byte_t)(0x80), (byte_t)(0xff)                      \
        };                                                                                                      \
        std::span<const byte_t> val1{raw_data, sizeof(raw_data)};                                               \
                                                                                                                \
        /* Caller needs to manage val1's lifetime. The Data is not copied. */                                   \
        msgpack::object obj(val1);                                                                              \
                                                                                                                \
        {                                                                                                       \
            auto val2 = obj.as<std::vector<byte_t>>();                                                          \
            BOOST_CHECK(val1 == val2);                                                                          \
        }                                                                                                       \
        {                                                                                                       \
            auto val2 = obj.as<std::span<const byte_t>>();                                                      \
            BOOST_CHECK(val1 == val2);                                                                          \
        }                                                                                                       \
    }                                                                                                           \

#define MSGPACK_TEST_SPAN_BYTE_OBJECT_WITH_ZONE(byte_t, display_name)                                           \
    BOOST_AUTO_TEST_CASE(span_##display_name##_object_with_zone)                                                \
    {                                                                                                           \
        msgpack::zone z;                                                                                        \
        byte_t raw_data[] = {                                                                                   \
            (byte_t)(0x01), (byte_t)(0x02), (byte_t)(0x7f), (byte_t)(0x80), (byte_t)(0xff)                      \
        };                                                                                                      \
        std::span<const byte_t> val1(raw_data, sizeof(raw_data));                                               \
                                                                                                                \
        msgpack::object obj(val1, z);                                                                           \
                                                                                                                \
        {                                                                                                       \
            auto val2 = obj.as<std::vector<byte_t>>();                                                          \
            BOOST_CHECK(val1 == val2);                                                                          \
        }                                                                                                       \
        {                                                                                                       \
            auto val2 = obj.as<std::span<const byte_t>>();                                                      \
            BOOST_CHECK(val1 == val2);                                                                          \
        }                                                                                                       \
    }                                                                                                           \

#define MSGPACK_TEST_SPAN_BYTE(byte_t, display_name)                                                            \
    MSGPACK_TEST_SPAN_BYTE_PACK_CONVERT(byte_t, display_name)                                                   \
    MSGPACK_TEST_SPAN_BYTE_OBJECT(byte_t, display_name)                                                         \
    MSGPACK_TEST_SPAN_BYTE_OBJECT_WITH_ZONE(byte_t, display_name)                                               \

MSGPACK_TEST_SPAN_BYTE(std::byte, byte)
MSGPACK_TEST_SPAN_BYTE(char, char)
MSGPACK_TEST_SPAN_BYTE(unsigned char, unsigned_char)

#undef MSGPACK_TEST_SPAN_BYTE
#undef MSGPACK_TEST_SPAN_BYTE_OBJECT_WITH_ZONE
#undef MSGPACK_TEST_SPAN_BYTE_OBJECT
#undef MSGPACK_TEST_SPAN_BYTE_PACK_CONVERT

#endif // MSGPACK_HAS_INCLUDE(<span>)

#endif // MSGPACK_CPP_VERSION > 201703
