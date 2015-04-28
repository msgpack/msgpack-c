#include <msgpack.hpp>
#include <fstream>
#include <sstream>
#include <gtest/gtest.h>

TEST(json, basic_elements)
{
    typedef std::map<std::string, int> map_s_i;
    map_s_i msi;
    msi.insert(map_s_i::value_type("Hello", 789));
    msi.insert(map_s_i::value_type("World", -789));

    msgpack::type::tuple<int, int, double, double, bool, bool, std::string, map_s_i>
        t1(12, -34, 1.23, -4.56, true, false, "ABC", msi);

    msgpack::zone z;
    msgpack::object o(t1, z);
    std::stringstream ss;
    ss << o;
    EXPECT_EQ(ss.str(), "[12, -34, 1.23, -4.56, true, false, \"ABC\", {\"Hello\":789, \"World\":-789}]");
}

TEST(json, escape)
{
    std::string s = "\"\\/\b\f\n\r\tabc";

    msgpack::zone z;
    msgpack::object o(s, z);
    std::stringstream ss;
    ss << o;
    EXPECT_EQ(ss.str(), "\"\\\"\\\\\\/\\b\\f\\n\\r\\tabc\"");
}
