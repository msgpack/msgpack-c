#include <msgpack.hpp>
#include <gtest/gtest.h>
#include <sstream>

TEST(pack, num)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
}


TEST(pack, vector)
{
    msgpack::sbuffer sbuf;
    std::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    msgpack::pack(sbuf, vec);
}


TEST(pack, to_ostream)
{
    std::ostringstream stream;
    msgpack::pack(stream, 1);
}


struct myclass {
    myclass() : num(0), str("default") { }

    myclass(int num, const std::string& str) :
        num(num), str(str) { }

    ~myclass() { }

    int num;
    std::string str;

    MSGPACK_DEFINE(num, str);
};


TEST(pack, myclass)
{
    msgpack::sbuffer sbuf;
    myclass m(1, "msgpack");
    msgpack::pack(sbuf, m);
}


TEST(unpack, myclass_no_offset)
{
    msgpack::sbuffer sbuf;
    myclass m1(1, "phraser");
    msgpack::pack(sbuf, m1);

    msgpack::zone z;
    msgpack::object obj;

    // obsolete
    msgpack::unpack_return ret =
        msgpack::unpack(sbuf.data(), sbuf.size(), z, obj);

    EXPECT_EQ(ret, msgpack::UNPACK_SUCCESS);

    myclass m2 = obj.as<myclass>();
    EXPECT_EQ(m1.num, m2.num);
    EXPECT_EQ(m1.str, m2.str);
}

TEST(unpack, myclass_offset)
{
    msgpack::sbuffer sbuf;
    myclass m1(1, "phraser");
    msgpack::pack(sbuf, m1);

    msgpack::zone z;
    msgpack::object obj;
    std::size_t off = 0;
    // obsolete
    msgpack::unpack_return ret =
        msgpack::unpack(sbuf.data(), sbuf.size(), off, z, obj);

    EXPECT_EQ(ret, msgpack::UNPACK_SUCCESS);

    myclass m2 = obj.as<myclass>();
    EXPECT_EQ(m1.num, m2.num);
    EXPECT_EQ(m1.str, m2.str);
    EXPECT_EQ(off, sbuf.size());
}

TEST(unpack, myclass_offset_pointer)
{
    msgpack::sbuffer sbuf;
    myclass m1(1, "phraser");
    msgpack::pack(sbuf, m1);

    msgpack::zone z;
    msgpack::object obj;
    std::size_t off = 0;
    // obsolete
    msgpack::unpack_return ret =
        msgpack::unpack(sbuf.data(), sbuf.size(), &off, &z, &obj);

    EXPECT_EQ(ret, msgpack::UNPACK_SUCCESS);

    myclass m2 = obj.as<myclass>();
    EXPECT_EQ(m1.num, m2.num);
    EXPECT_EQ(m1.str, m2.str);
    EXPECT_EQ(off, sbuf.size());
}

TEST(unpack, myclass_offset_null_pointer)
{
    msgpack::sbuffer sbuf;
    myclass m1(1, "phraser");
    msgpack::pack(sbuf, m1);

    msgpack::zone z;
    msgpack::object obj;
    // obsolete
    msgpack::unpack_return ret =
        msgpack::unpack(sbuf.data(), sbuf.size(), nullptr, &z, &obj);

    EXPECT_EQ(ret, msgpack::UNPACK_SUCCESS);

    myclass m2 = obj.as<myclass>();
    EXPECT_EQ(m1.num, m2.num);
    EXPECT_EQ(m1.str, m2.str);
}

TEST(unpack, int_no_offset)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::unpacked msg;

    msgpack::unpack(msg, sbuf.data(), sbuf.size());
    EXPECT_EQ(1, msg.get().as<int>());
}

TEST(unpack, int_offset)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::unpacked msg;

    std::size_t off = 0;

    msgpack::unpack(msg, sbuf.data(), sbuf.size(), off);
    EXPECT_EQ(1, msg.get().as<int>());
    EXPECT_EQ(off, sbuf.size());
}

TEST(unpack, int_pointer)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::unpacked msg;

    std::size_t off = 0;

    // obsolete
    msgpack::unpack(&msg, sbuf.data(), sbuf.size(), &off);
    EXPECT_EQ(1, msg.get().as<int>());
    EXPECT_EQ(off, sbuf.size());
}

TEST(unpack, int_null_pointer)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::unpacked msg;

    // obsolete
    msgpack::unpack(&msg, sbuf.data(), sbuf.size(), nullptr);
    EXPECT_EQ(1, msg.get().as<int>());
}

TEST(unpack, int_default_null_pointer)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::unpacked msg;

    // obsolete
    msgpack::unpack(&msg, sbuf.data(), sbuf.size());
    EXPECT_EQ(1, msg.get().as<int>());
}

TEST(unpack, sequence)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::pack(sbuf, 2);
    msgpack::pack(sbuf, 3);

    std::size_t off = 0;

    msgpack::unpacked msg;

    msgpack::unpack(msg, sbuf.data(), sbuf.size(), off);
    EXPECT_EQ(1, msg.get().as<int>());

    msgpack::unpack(msg, sbuf.data(), sbuf.size(), off);
    EXPECT_EQ(2, msg.get().as<int>());

    msgpack::unpack(msg, sbuf.data(), sbuf.size(), off);
    EXPECT_EQ(3, msg.get().as<int>());

    EXPECT_EQ(off, sbuf.size());
}

TEST(unpack_return, int_no_offset)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);

    msgpack::zone z;
    msgpack::object obj;
    msgpack::unpack_return ret;

    // obsolete
    ret = msgpack::unpack(sbuf.data(), sbuf.size(), z, obj);
    EXPECT_TRUE(ret >= 0);
    EXPECT_EQ(ret, msgpack::UNPACK_SUCCESS);
    EXPECT_EQ(1, obj.as<int>());
}

TEST(unpack_return, int_offset)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);

    std::size_t off = 0;

    msgpack::zone z;
    msgpack::object obj;
    msgpack::unpack_return ret;

    // obsolete
    ret = msgpack::unpack(sbuf.data(), sbuf.size(), off, z, obj);
    EXPECT_TRUE(ret >= 0);
    EXPECT_EQ(ret, msgpack::UNPACK_SUCCESS);
    EXPECT_EQ(1, obj.as<int>());
    EXPECT_EQ(off, sbuf.size());
}

TEST(unpack_return, int_pointer)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);

    std::size_t off = 0;

    msgpack::zone z;
    msgpack::object obj;
    msgpack::unpack_return ret;

    // obsolete
    ret = msgpack::unpack(sbuf.data(), sbuf.size(), &off, &z, &obj);
    EXPECT_TRUE(ret >= 0);
    EXPECT_EQ(ret, msgpack::UNPACK_SUCCESS);
    EXPECT_EQ(1, obj.as<int>());
    EXPECT_EQ(off, sbuf.size());
}

TEST(unpack_return, int_null_pointer)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);

    msgpack::zone z;
    msgpack::object obj;
    msgpack::unpack_return ret;

    // obsolete
    ret = msgpack::unpack(sbuf.data(), sbuf.size(), nullptr, &z, &obj);
    EXPECT_TRUE(ret >= 0);
    EXPECT_EQ(ret, msgpack::UNPACK_SUCCESS);
    EXPECT_EQ(1, obj.as<int>());
}


TEST(unpack, sequence_compat)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, 1);
    msgpack::pack(sbuf, 2);
    msgpack::pack(sbuf, 3);

    std::size_t off = 0;

    msgpack::zone z;
    msgpack::object obj;
    msgpack::unpack_return ret;

    // obsolete
    ret = msgpack::unpack(sbuf.data(), sbuf.size(), off, z, obj);
    EXPECT_TRUE(ret >= 0);
    EXPECT_EQ(ret, msgpack::UNPACK_EXTRA_BYTES);
    EXPECT_EQ(1, obj.as<int>());

    // obsolete
    ret = msgpack::unpack(sbuf.data(), sbuf.size(), off, z, obj);
    EXPECT_TRUE(ret >= 0);
    EXPECT_EQ(ret, msgpack::UNPACK_EXTRA_BYTES);
    EXPECT_EQ(2, obj.as<int>());

    // obsolete
    ret = msgpack::unpack(sbuf.data(), sbuf.size(), off, z, obj);
    EXPECT_TRUE(ret >= 0);
    EXPECT_EQ(ret, msgpack::UNPACK_SUCCESS);
    EXPECT_EQ(3, obj.as<int>());
}
