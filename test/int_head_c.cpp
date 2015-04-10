#include <msgpack.h>
#include <gtest/gtest.h>

TEST(int_head_c, uint8)
{
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint8_t v = 0x7fu;
        msgpack_pack_uint8(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0x7fu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint8_t v = 0x80u;
        msgpack_pack_uint8(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xccu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint8_t v = std::numeric_limits<uint8_t>::max();
        msgpack_pack_uint8(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xccu));

        msgpack_sbuffer_destroy(&sbuf);
    }
}

TEST(int_head_c, uint16)
{
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint16_t v = 0x7fu;
        msgpack_pack_uint16(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0x7fu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint16_t v = 0x80u;
        msgpack_pack_uint16(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xccu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint16_t v = std::numeric_limits<uint8_t>::max();
        msgpack_pack_uint16(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xccu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint16_t v = static_cast<uint16_t>(std::numeric_limits<uint8_t>::max()) + 1;
        msgpack_pack_uint16(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xcdu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint16_t v = std::numeric_limits<uint16_t>::max();
        msgpack_pack_uint16(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xcdu));

        msgpack_sbuffer_destroy(&sbuf);
    }
}

TEST(int_head_c, uint32)
{
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint32_t v = 0x7fu;
        msgpack_pack_uint32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0x7fu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint32_t v = 0x80u;
        msgpack_pack_uint32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xccu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint32_t v = std::numeric_limits<uint8_t>::max();
        msgpack_pack_uint32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xccu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint32_t v = static_cast<uint32_t>(std::numeric_limits<uint8_t>::max()) + 1;
        msgpack_pack_uint32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xcdu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint32_t v = std::numeric_limits<uint16_t>::max();
        msgpack_pack_uint32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xcdu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint32_t v = static_cast<uint32_t>(std::numeric_limits<uint16_t>::max()) + 1;
        msgpack_pack_uint32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xceu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint32_t v = std::numeric_limits<uint32_t>::max();
        msgpack_pack_uint32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xceu));

        msgpack_sbuffer_destroy(&sbuf);
    }
}

TEST(int_head_c, uint64)
{
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint64_t v = 0x7fu;
        msgpack_pack_uint64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0x7fu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint64_t v = 0x80u;
        msgpack_pack_uint64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xccu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint64_t v = std::numeric_limits<uint8_t>::max();
        msgpack_pack_uint64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xccu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint64_t v = static_cast<uint64_t>(std::numeric_limits<uint8_t>::max()) + 1;
        msgpack_pack_uint64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xcdu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint64_t v = std::numeric_limits<uint16_t>::max();
        msgpack_pack_uint64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xcdu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint64_t v = static_cast<uint64_t>(std::numeric_limits<uint16_t>::max()) + 1;
        msgpack_pack_uint64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xceu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint64_t v = std::numeric_limits<uint32_t>::max();
        msgpack_pack_uint64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xceu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint64_t v = static_cast<uint64_t>(std::numeric_limits<uint32_t>::max()) + 1;
        msgpack_pack_uint64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xcfu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        uint64_t v = std::numeric_limits<uint64_t>::max();
        msgpack_pack_uint64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xcfu));

        msgpack_sbuffer_destroy(&sbuf);
    }
}

TEST(int_head_c, int8)
{
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int8_t v = 0x7fu;
        msgpack_pack_int8(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0x7fu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int8_t v = -32;
        msgpack_pack_int8(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xe0u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int8_t v = std::numeric_limits<int8_t>::min();
        msgpack_pack_int8(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd0u));

        msgpack_sbuffer_destroy(&sbuf);
    }
}

TEST(int_head_c, int16)
{
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int16_t v = 0x7fu;
        msgpack_pack_int16(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0x7fu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int16_t v = -32;
        msgpack_pack_int16(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xe0u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int16_t v = std::numeric_limits<int8_t>::min();
        msgpack_pack_int16(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd0u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int16_t v = static_cast<int16_t>(std::numeric_limits<int8_t>::max()) + 1;
        msgpack_pack_int16(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xccu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int16_t v = static_cast<int16_t>(std::numeric_limits<uint8_t>::max());
        msgpack_pack_int16(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xccu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int16_t v = static_cast<int16_t>(std::numeric_limits<uint8_t>::max()) + 1;
        msgpack_pack_int16(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd1u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int16_t v = static_cast<int16_t>(std::numeric_limits<int8_t>::min()) - 1;
        msgpack_pack_int16(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd1u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int16_t v = std::numeric_limits<int16_t>::max();
        msgpack_pack_int16(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd1u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int16_t v = std::numeric_limits<int16_t>::min();
        msgpack_pack_int16(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd1u));

        msgpack_sbuffer_destroy(&sbuf);
    }
}

TEST(int_head_c, int32)
{
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int32_t v = 0x7fu;
        msgpack_pack_int32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0x7fu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int32_t v = -32;
        msgpack_pack_int32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xe0u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int32_t v = std::numeric_limits<int8_t>::min();
        msgpack_pack_int32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd0u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int32_t v = static_cast<int32_t>(std::numeric_limits<int8_t>::max()) + 1;
        msgpack_pack_int32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xccu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int32_t v = static_cast<int32_t>(std::numeric_limits<uint8_t>::max());
        msgpack_pack_int32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xccu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int32_t v = static_cast<int32_t>(std::numeric_limits<uint8_t>::max()) + 1;
        msgpack_pack_int32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd1u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int32_t v = static_cast<int32_t>(std::numeric_limits<int8_t>::min()) - 1;
        msgpack_pack_int32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd1u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int32_t v = std::numeric_limits<int16_t>::max();
        msgpack_pack_int32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd1u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int32_t v = std::numeric_limits<int16_t>::min();
        msgpack_pack_int32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd1u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int32_t v = static_cast<int32_t>(std::numeric_limits<int16_t>::max()) + 1;
        msgpack_pack_int32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xcdu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int32_t v = static_cast<int32_t>(std::numeric_limits<uint16_t>::max());
        msgpack_pack_int32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xcdu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int32_t v = static_cast<int32_t>(std::numeric_limits<uint16_t>::max()) + 1;
        msgpack_pack_int32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd2u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int32_t v = static_cast<int32_t>(std::numeric_limits<int16_t>::min()) - 1;
        msgpack_pack_int32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd2u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int32_t v = std::numeric_limits<int32_t>::max();
        msgpack_pack_int32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd2u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int32_t v = std::numeric_limits<int32_t>::min();
        msgpack_pack_int32(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd2u));

        msgpack_sbuffer_destroy(&sbuf);
    }
}

TEST(int_head_c, int64)
{
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int64_t v = 0x7fu;
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0x7fu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int64_t v = -32;
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xe0u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int64_t v = std::numeric_limits<int8_t>::min();
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd0u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int64_t v = static_cast<int64_t>(std::numeric_limits<int8_t>::max()) + 1;
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xccu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int64_t v = static_cast<int64_t>(std::numeric_limits<uint8_t>::max());
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xccu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int64_t v = static_cast<int64_t>(std::numeric_limits<uint8_t>::max()) + 1;
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd1u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int64_t v = static_cast<int64_t>(std::numeric_limits<int8_t>::min()) - 1;
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd1u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int64_t v = std::numeric_limits<int16_t>::max();
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd1u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int64_t v = std::numeric_limits<int16_t>::min();
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd1u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int32_t v = static_cast<int32_t>(std::numeric_limits<int16_t>::max()) + 1;
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xcdu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int32_t v = static_cast<int32_t>(std::numeric_limits<uint16_t>::max());
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xcdu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int32_t v = static_cast<int32_t>(std::numeric_limits<uint16_t>::max()) + 1;
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd2u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int64_t v = static_cast<int64_t>(std::numeric_limits<int16_t>::min()) - 1;
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd2u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int64_t v = std::numeric_limits<int32_t>::max();
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd2u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int64_t v = std::numeric_limits<int32_t>::min();
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd2u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int64_t v = static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1;
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xceu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int64_t v = static_cast<int64_t>(std::numeric_limits<uint32_t>::max());
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xceu));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int64_t v = static_cast<int64_t>(std::numeric_limits<uint32_t>::max()) + 1;
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd3u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int64_t v = static_cast<int64_t>(std::numeric_limits<int32_t>::min()) - 1;
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd3u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int64_t v = std::numeric_limits<int64_t>::min();
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd3u));

        msgpack_sbuffer_destroy(&sbuf);
    }
    {
        msgpack_sbuffer sbuf;
        msgpack_sbuffer_init(&sbuf);
        msgpack_packer pk;
        msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

        int64_t v = std::numeric_limits<int64_t>::max();
        msgpack_pack_int64(&pk, v);
        EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd3u));

        msgpack_sbuffer_destroy(&sbuf);
    }
}

TEST(int_head_c, fix_uint8)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    uint8_t v = 0;
    msgpack_pack_fix_uint8(&pk, v);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xccu));

    msgpack_sbuffer_destroy(&sbuf);
}

TEST(int_head_c, fix_uint16)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    uint16_t v = 0;
    msgpack_pack_fix_uint16(&pk, v);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xcdu));

    msgpack_sbuffer_destroy(&sbuf);
}

TEST(int_head_c, fix_uint32)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    uint32_t v = 0;
    msgpack_pack_fix_uint32(&pk, v);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xceu));

    msgpack_sbuffer_destroy(&sbuf);
}

TEST(int_head_c, fix_uint64)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    uint64_t v = 0;
    msgpack_pack_fix_uint64(&pk, v);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xcfu));

    msgpack_sbuffer_destroy(&sbuf);
}

TEST(int_head_c, fix_int8)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    int8_t v = 0;
    msgpack_pack_fix_int8(&pk, v);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd0u));

    msgpack_sbuffer_destroy(&sbuf);
}

TEST(int_head_c, fix_int16)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    int16_t v = 0;
    msgpack_pack_fix_int16(&pk, v);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd1u));

    msgpack_sbuffer_destroy(&sbuf);
}

TEST(int_head_c, fix_int32)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    int32_t v = 0;
    msgpack_pack_fix_int32(&pk, v);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd2u));

    msgpack_sbuffer_destroy(&sbuf);
}

TEST(int_head_c, fix_int64)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    int64_t v = 0;
    msgpack_pack_fix_int64(&pk, v);
    EXPECT_EQ(sbuf.data[0], static_cast<char>(0xd3u));

    msgpack_sbuffer_destroy(&sbuf);
}
