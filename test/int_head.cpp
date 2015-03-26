#include <msgpack.hpp>
#include <gtest/gtest.h>
#include <sstream>

TEST(int_head, positive_fixint)
{
    {
        std::stringstream ss;
        msgpack::pack(ss, 0);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0x00u));
    }
    {
        std::stringstream ss;
        msgpack::pack(ss, 0x7fu);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0x7fu));
    }
}

TEST(int_head, negative_fixint)
{
    {
        std::stringstream ss;
        msgpack::pack(ss, -32);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xe0u));
    }
    {
        std::stringstream ss;
        msgpack::pack(ss, -1);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xffu));
    }
}

TEST(int_head, uint8)
{
    {
        std::stringstream ss;
        uint8_t v = 0x7fu;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0x7fu));
    }
    {
        std::stringstream ss;
        uint8_t v = 0x80u;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xccu));
    }
    {
        std::stringstream ss;
        uint8_t v = std::numeric_limits<uint8_t>::max();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xccu));
    }
}

TEST(int_head, uint16)
{
    {
        std::stringstream ss;
        uint16_t v = 0x7fu;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0x7fu));
    }
    {
        std::stringstream ss;
        uint16_t v = 0x80u;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xccu));
    }
    {
        std::stringstream ss;
        uint16_t v = std::numeric_limits<uint8_t>::max();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xccu));
    }
    {
        std::stringstream ss;
        uint16_t v = static_cast<uint16_t>(std::numeric_limits<uint8_t>::max()) + 1;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xcdu));
    }
    {
        std::stringstream ss;
        uint16_t v = std::numeric_limits<uint16_t>::max();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xcdu));
    }
}

TEST(int_head, uint32)
{
    {
        std::stringstream ss;
        uint32_t v = 0x7fu;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0x7fu));
    }
    {
        std::stringstream ss;
        uint32_t v = 0x80u;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xccu));
    }
    {
        std::stringstream ss;
        uint32_t v = std::numeric_limits<uint8_t>::max();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xccu));
    }
    {
        std::stringstream ss;
        uint32_t v = static_cast<uint32_t>(std::numeric_limits<uint8_t>::max()) + 1;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xcdu));
    }
    {
        std::stringstream ss;
        uint32_t v = std::numeric_limits<uint16_t>::max();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xcdu));
    }
    {
        std::stringstream ss;
        uint32_t v = static_cast<uint32_t>(std::numeric_limits<uint16_t>::max()) + 1;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xceu));
    }
    {
        std::stringstream ss;
        uint32_t v = std::numeric_limits<uint32_t>::max();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xceu));
    }
}

TEST(int_head, uint64)
{
    {
        std::stringstream ss;
        uint64_t v = 0x7fu;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0x7fu));
    }
    {
        std::stringstream ss;
        uint64_t v = 0x80u;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xccu));
    }
    {
        std::stringstream ss;
        uint64_t v = std::numeric_limits<uint8_t>::max();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xccu));
    }
    {
        std::stringstream ss;
        uint64_t v = static_cast<uint64_t>(std::numeric_limits<uint8_t>::max()) + 1;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xcdu));
    }
    {
        std::stringstream ss;
        uint64_t v = std::numeric_limits<uint16_t>::max();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xcdu));
    }
    {
        std::stringstream ss;
        uint64_t v = static_cast<uint64_t>(std::numeric_limits<uint16_t>::max()) + 1;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xceu));
    }
    {
        std::stringstream ss;
        uint64_t v = std::numeric_limits<uint32_t>::max();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xceu));
    }
    {
        std::stringstream ss;
        uint64_t v = static_cast<uint64_t>(std::numeric_limits<uint32_t>::max()) + 1;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xcfu));
    }
    {
        std::stringstream ss;
        uint64_t v = std::numeric_limits<uint64_t>::max();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xcfu));
    }
}

TEST(int_head, int8)
{
    {
        std::stringstream ss;
        int8_t v = 0x7fu;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0x7fu));
    }
    {
        std::stringstream ss;
        int8_t v = -32;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xe0u));
    }
    {
        std::stringstream ss;
        int8_t v = std::numeric_limits<int8_t>::min();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd0u));
    }
}

TEST(int_head, int16)
{
    {
        std::stringstream ss;
        int16_t v = 0x7fu;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0x7fu));
    }
    {
        std::stringstream ss;
        int16_t v = -32;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xe0u));
    }
    {
        std::stringstream ss;
        int16_t v = std::numeric_limits<int8_t>::min();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd0u));
    }
    {
        std::stringstream ss;
        int16_t v = static_cast<int16_t>(std::numeric_limits<int8_t>::max()) + 1;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd1u));
    }
    {
        std::stringstream ss;
        int16_t v = static_cast<int16_t>(std::numeric_limits<int8_t>::min()) - 1;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd1u));
    }
    {
        std::stringstream ss;
        int16_t v = std::numeric_limits<int16_t>::max();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd1u));
    }
    {
        std::stringstream ss;
        int16_t v = std::numeric_limits<int16_t>::min();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd1u));
    }
}

TEST(int_head, int32)
{
    {
        std::stringstream ss;
        int32_t v = 0x7fu;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0x7fu));
    }
    {
        std::stringstream ss;
        int32_t v = -32;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xe0u));
    }
    {
        std::stringstream ss;
        int32_t v = std::numeric_limits<int8_t>::min();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd0u));
    }
    {
        std::stringstream ss;
        int32_t v = static_cast<int32_t>(std::numeric_limits<int8_t>::max()) + 1;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd1u));
    }
    {
        std::stringstream ss;
        int32_t v = static_cast<int32_t>(std::numeric_limits<int8_t>::min()) - 1;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd1u));
    }
    {
        std::stringstream ss;
        int32_t v = std::numeric_limits<int16_t>::max();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd1u));
    }
    {
        std::stringstream ss;
        int32_t v = std::numeric_limits<int16_t>::min();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd1u));
    }
    {
        std::stringstream ss;
        int32_t v = static_cast<int32_t>(std::numeric_limits<int16_t>::max()) + 1;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd2u));
    }
    {
        std::stringstream ss;
        int32_t v = static_cast<int32_t>(std::numeric_limits<int16_t>::min()) - 1;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd2u));
    }
    {
        std::stringstream ss;
        int32_t v = std::numeric_limits<int32_t>::max();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd2u));
    }
    {
        std::stringstream ss;
        int32_t v = std::numeric_limits<int32_t>::min();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd2u));
    }
}

TEST(int_head, int64)
{
    {
        std::stringstream ss;
        int64_t v = 0x7fu;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0x7fu));
    }
    {
        std::stringstream ss;
        int64_t v = -32;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xe0u));
    }
    {
        std::stringstream ss;
        int64_t v = std::numeric_limits<int8_t>::min();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd0u));
    }
    {
        std::stringstream ss;
        int64_t v = static_cast<int64_t>(std::numeric_limits<int8_t>::max()) + 1;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd1u));
    }
    {
        std::stringstream ss;
        int64_t v = static_cast<int64_t>(std::numeric_limits<int8_t>::min()) - 1;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd1u));
    }
    {
        std::stringstream ss;
        int64_t v = std::numeric_limits<int16_t>::max();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd1u));
    }
    {
        std::stringstream ss;
        int64_t v = std::numeric_limits<int16_t>::min();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd1u));
    }
    {
        std::stringstream ss;
        int64_t v = static_cast<int64_t>(std::numeric_limits<int16_t>::max()) + 1;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd2u));
    }
    {
        std::stringstream ss;
        int64_t v = static_cast<int64_t>(std::numeric_limits<int16_t>::min()) - 1;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd2u));
    }
    {
        std::stringstream ss;
        int64_t v = std::numeric_limits<int32_t>::max();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd2u));
    }
    {
        std::stringstream ss;
        int64_t v = std::numeric_limits<int32_t>::min();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd2u));
    }
    {
        std::stringstream ss;
        int64_t v = static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd3u));
    }
    {
        std::stringstream ss;
        int64_t v = static_cast<int64_t>(std::numeric_limits<int32_t>::min()) - 1;
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd3u));
    }
    {
        std::stringstream ss;
        int64_t v = std::numeric_limits<int64_t>::max();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd3u));
    }
    {
        std::stringstream ss;
        int64_t v = std::numeric_limits<int64_t>::min();
        msgpack::pack(ss, v);
        EXPECT_EQ(ss.str()[0], static_cast<char>(0xd3u));
    }
}
