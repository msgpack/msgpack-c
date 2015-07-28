#include <msgpack.hpp>

#include <gtest/gtest.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if !defined(MSGPACK_USE_CPP03)

class TestEnumClassMemberClass
{
public:
    TestEnumClassMemberClass()
        : t1(TestEnumClassType::STATE_A), t2(TestEnumClassType::STATE_B), t3(TestEnumClassType::STATE_C) {}

    enum class TestEnumClassType:long {
        STATE_INVALID = 0,
            STATE_A = 1,
            STATE_B = 2,
            STATE_C = 3
        };
    TestEnumClassType t1;
    TestEnumClassType t2;
    TestEnumClassType t3;

    MSGPACK_DEFINE(t1, t2, t3);
};

MSGPACK_ADD_ENUM(TestEnumClassMemberClass::TestEnumClassType);

using namespace std;

const unsigned int kLoop = 10000;
const unsigned int kElements = 100;


// C++11

TEST(MSGPACK_CPP11, simple_tuple)
{
    msgpack::sbuffer sbuf;
    std::tuple<bool, std::string, double> val1(true, "kzk", 12.3);
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    std::tuple<bool, std::string, double> val2 = ret.get().as<std::tuple<bool, std::string, double> >();
    EXPECT_EQ(val1, val2);
}

TEST(MSGPACK_CPP11, simple_tuple_empty)
{
    msgpack::sbuffer sbuf;
    std::tuple<> val1;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    std::tuple<> val2 = ret.get().as<std::tuple<> >();
    EXPECT_EQ(val1, val2);
}

TEST(MSGPACK_CPP11, simple_array)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        array<int, kElements> val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1[i] = rand();
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::unpacked ret;
        msgpack::unpack(ret, sbuf.data(), sbuf.size());
        EXPECT_EQ(ret.get().type, msgpack::type::ARRAY);
        array<int, kElements> val2 = ret.get().as<array<int, kElements> >();
        EXPECT_EQ(val1.size(), val2.size());
        EXPECT_TRUE(equal(val1.begin(), val1.end(), val2.begin()));
    }
}

TEST(MSGPACK_CPP11, simple_buffer_array_char)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        array<char, kElements> val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1[i] = rand();
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::unpacked ret;
        msgpack::unpack(ret, sbuf.data(), sbuf.size());
        EXPECT_EQ(ret.get().type, msgpack::type::BIN);
        array<char, kElements> val2 = ret.get().as<array<char, kElements> >();
        EXPECT_EQ(val1.size(), val2.size());
        EXPECT_TRUE(equal(val1.begin(), val1.end(), val2.begin()));
    }
}

TEST(MSGPACK_STL, simple_buffer_forward_list)
{
    for (unsigned int k = 0; k < kLoop; k++) {
        forward_list<int> val1;
        for (unsigned int i = 0; i < kElements; i++)
            val1.push_front(rand());
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, val1);
        msgpack::unpacked ret;
        msgpack::unpack(ret, sbuf.data(), sbuf.size());
        forward_list<int> val2 = ret.get().as<forward_list<int> >();
        EXPECT_EQ(val1, val2);
    }
}

TEST(MSGPACK_STL, simple_buffer_unordered_map)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    unordered_map<int, int> val1;
    for (unsigned int i = 0; i < kElements; i++)
      val1[rand()] = rand();
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    unordered_map<int, int> val2 = ret.get().as<unordered_map<int, int> >();
    EXPECT_EQ(val1, val2);
  }
}

TEST(MSGPACK_STL, simple_buffer_unordered_multimap)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    unordered_multimap<int, int> val1;
    for (unsigned int i = 0; i < kElements; i++) {
      int i1 = rand();
      val1.insert(make_pair(i1, rand()));
      val1.insert(make_pair(i1, rand()));
    }
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    unordered_multimap<int, int> val2 = ret.get().as<unordered_multimap<int, int> >();

    EXPECT_EQ(val1, val2);
  }
}

TEST(MSGPACK_STL, simple_buffer_unordered_set)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    unordered_set<int> val1;
    for (unsigned int i = 0; i < kElements; i++)
      val1.insert(rand());
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    unordered_set<int> val2 = ret.get().as<unordered_set<int> >();
    EXPECT_EQ(val1, val2);
  }
}

TEST(MSGPACK_STL, simple_buffer_unordered_multiset)
{
  for (unsigned int k = 0; k < kLoop; k++) {
    unordered_multiset<int> val1;
    for (unsigned int i = 0; i < kElements; i++)
      val1.insert(rand());
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    unordered_multiset<int> val2 = ret.get().as<unordered_multiset<int> >();
    EXPECT_EQ(val1, val2);
  }
}

TEST(MSGPACK_USER_DEFINED, simple_buffer_enum_class_member)
{
    TestEnumClassMemberClass val1;
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    TestEnumClassMemberClass val2 = ret.get().as<TestEnumClassMemberClass>();
    EXPECT_EQ(val1.t1, val2.t1);
    EXPECT_EQ(val1.t2, val2.t2);
    EXPECT_EQ(val1.t3, val2.t3);
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

inline bool operator<(no_def_con const& lhs, no_def_con const& rhs) {
    return lhs.i <  rhs.i;
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

namespace std {
template <> struct hash<no_def_con> {
    size_t operator()(const no_def_con & x) const {
        return hash<int>()(x.i);
    }
};
} // std

TEST(MSGPACK_NO_DEF_CON, simple_buffer)
{
    no_def_con val1(42);
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());

    no_def_con val2 = ret.get().as<no_def_con>();
    EXPECT_EQ(val1, val2);
}

struct no_def_con_composite {
    no_def_con_composite() = delete;
    no_def_con_composite(int i):ndc(i) {}
    no_def_con_composite(no_def_con const& a):ndc(a) {}
    no_def_con ndc;
    MSGPACK_DEFINE(ndc);
};

inline bool operator==(no_def_con_composite const& lhs, no_def_con_composite const& rhs) {
    return lhs.ndc == rhs.ndc;
}

inline bool operator!=(no_def_con_composite const& lhs, no_def_con_composite const& rhs) {
    return !(lhs == rhs);
}

inline bool operator<(no_def_con_composite const& lhs, no_def_con_composite const& rhs) {
    return lhs.ndc < rhs.ndc;
}

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
namespace adaptor {
template <>
struct as<no_def_con_composite> {
    no_def_con_composite operator()(msgpack::object const& o) const {
        if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
        if (o.via.array.size != 1) throw msgpack::type_error();
        return no_def_con_composite(o.via.array.ptr[0].as<no_def_con>());
    }
};
} // adaptor
} // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // msgpack

TEST(MSGPACK_NO_DEF_CON_COMPOSITE, simple_buffer)
{
    no_def_con_composite val1(42);
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    no_def_con_composite val2 = ret.get().as<no_def_con_composite>();
    EXPECT_EQ(val1, val2);
}

struct no_def_con_inherit : no_def_con {
    no_def_con_inherit() = delete;
    no_def_con_inherit(no_def_con const& a):no_def_con(a) {}
    MSGPACK_DEFINE(MSGPACK_BASE(no_def_con));
};

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
namespace adaptor {
template <>
struct as<no_def_con_inherit> {
    no_def_con_inherit operator()(msgpack::object const& o) const {
        if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
        if (o.via.array.size != 1) throw msgpack::type_error();
        return no_def_con_inherit(o.via.array.ptr[0].as<no_def_con>());
    }
};
} // adaptor
} // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // msgpack

TEST(MSGPACK_NO_DEF_CON_INHERIT, simple_buffer)
{
    no_def_con_inherit val1(42);
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    no_def_con_inherit val2 = ret.get().as<no_def_con_inherit>();
    EXPECT_EQ(val1, val2);
}

TEST(MSGPACK_NO_DEF_CON_VECTOR, simple_buffer)
{
    std::vector<no_def_con> val1 { 1, 2, 3 };
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    std::vector<no_def_con> val2 = ret.get().as<std::vector<no_def_con>>();
    EXPECT_EQ(val1, val2);
}

TEST(MSGPACK_NO_DEF_CON_LIST, simple_buffer)
{
    std::list<no_def_con> val1 { 1, 2, 3 };
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    std::list<no_def_con> val2 = ret.get().as<std::list<no_def_con>>();
    EXPECT_EQ(val1, val2);
}

TEST(MSGPACK_NO_DEF_CON_SET, simple_buffer)
{
    std::set<no_def_con> val1 { 1, 2, 3 };
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    std::set<no_def_con> val2 = ret.get().as<std::set<no_def_con>>();
    EXPECT_EQ(val1, val2);
}

TEST(MSGPACK_NO_DEF_CON_MULTISET, simple_buffer)
{
    std::multiset<no_def_con> val1 { 1, 2, 3 };
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    std::multiset<no_def_con> val2 = ret.get().as<std::multiset<no_def_con>>();
    EXPECT_EQ(val1, val2);
}

TEST(MSGPACK_NO_DEF_CON_ASSOC_VECTOR, simple_buffer)
{
    msgpack::type::assoc_vector<no_def_con, no_def_con_composite> val1 { {1, 2}, {3, 4}, {5, 6}};
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    msgpack::type::assoc_vector<no_def_con, no_def_con_composite> val2
        = ret.get().as<msgpack::type::assoc_vector<no_def_con, no_def_con_composite>>();
    EXPECT_EQ(val1, val2);
}

TEST(MSGPACK_NO_DEF_CON_MAP, simple_buffer)
{
    std::map<no_def_con, no_def_con_composite> val1 { {1, 2}, {3, 4}, {5, 6}};
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    std::map<no_def_con, no_def_con_composite> val2
        = ret.get().as<std::map<no_def_con, no_def_con_composite>>();
    EXPECT_EQ(val1, val2);
}

TEST(MSGPACK_NO_DEF_CON_MULTIMAP, simple_buffer)
{
    std::multimap<no_def_con, no_def_con_composite> val1 { {1, 2}, {3, 4}, {5, 6}};
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    std::multimap<no_def_con, no_def_con_composite> val2
        = ret.get().as<std::multimap<no_def_con, no_def_con_composite>>();
    EXPECT_EQ(val1, val2);
}

TEST(MSGPACK_NO_DEF_CON_PAIR, simple_buffer)
{
    std::pair<no_def_con, no_def_con_composite> val1 {1, 2};
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    std::pair<no_def_con, no_def_con_composite> val2
        = ret.get().as<std::pair<no_def_con, no_def_con_composite>>();
    EXPECT_EQ(val1, val2);
}

TEST(MSGPACK_NO_DEF_CON_TUPLE, simple_buffer)
{
    std::tuple<no_def_con, no_def_con, no_def_con_composite> val1 {1, 2, 3};
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    std::tuple<no_def_con, no_def_con, no_def_con_composite> val2
        = ret.get().as<std::tuple<no_def_con, no_def_con, no_def_con_composite>>();
    EXPECT_EQ(val1, val2);
}

TEST(MSGPACK_NO_DEF_CON_MSGPACK_TUPLE, simple_buffer)
{
    msgpack::type::tuple<no_def_con, no_def_con, no_def_con_composite> val1 {1, 2, 3};
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    msgpack::type::tuple<no_def_con, no_def_con, no_def_con_composite> val2
        = ret.get().as<msgpack::type::tuple<no_def_con, no_def_con, no_def_con_composite>>();
    EXPECT_EQ(val1, val2);
}

TEST(MSGPACK_NO_DEF_FORWARD_LIST, simple_buffer)
{
    std::forward_list<no_def_con> val1 { 1, 2, 3 };
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    std::forward_list<no_def_con> val2 = ret.get().as<std::forward_list<no_def_con>>();
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_NO_DEF_CON_UNORDERED_SET, simple_buffer)
{
    std::unordered_set<no_def_con> val1 { 1, 2, 3 };
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    std::unordered_set<no_def_con> val2 = ret.get().as<std::unordered_set<no_def_con>>();
    EXPECT_EQ(val1, val2);
}

TEST(MSGPACK_NO_DEF_CON_UNORDERED_MULTISET, simple_buffer)
{
    std::unordered_multiset<no_def_con> val1 { 1, 2, 3 };
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    std::unordered_multiset<no_def_con> val2 = ret.get().as<std::unordered_multiset<no_def_con>>();
    EXPECT_EQ(val1, val2);
}

TEST(MSGPACK_NO_DEF_CON_UNORDERED_MAP, simple_buffer)
{
    std::unordered_map<no_def_con, no_def_con_composite> val1 { {1, 2}, {3, 4}, {5, 6}};
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    std::unordered_map<no_def_con, no_def_con_composite> val2
        = ret.get().as<std::unordered_map<no_def_con, no_def_con_composite>>();
    EXPECT_EQ(val1, val2);
}

TEST(MSGPACK_NO_DEF_CON_UNORDERED_MULTIMAP, simple_buffer)
{
    std::unordered_multimap<no_def_con, no_def_con_composite> val1 { {1, 2}, {3, 4}, {5, 6}};
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    std::unordered_multimap<no_def_con, no_def_con_composite> val2
        = ret.get().as<std::unordered_multimap<no_def_con, no_def_con_composite>>();
    EXPECT_EQ(val1, val2);
}

TEST(MSGPACK_NO_DEF_CON_ARRAY, simple_buffer)
{
    std::array<no_def_con, 3> val1 { { 1, 2, 3 } };
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, val1);
    msgpack::unpacked ret;
    msgpack::unpack(ret, sbuf.data(), sbuf.size());
    std::array<no_def_con, 3> val2 = ret.get().as<std::array<no_def_con, 3>>();
    EXPECT_EQ(val1, val2);
}

#endif // !defined(MSGPACK_USE_CPP03)
