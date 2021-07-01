#include <msgpack.hpp>

#define BOOST_TEST_MODULE msgpack_tuple
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(member_get)
{
    msgpack::type::tuple<int, bool, std::string> t1(42, true, "ABC");
    BOOST_CHECK_EQUAL(42, t1.get<0>());
    BOOST_CHECK_EQUAL(true, t1.get<1>());
    BOOST_CHECK_EQUAL("ABC", t1.get<2>());
    t1.get<0>() = 40;
    t1.get<1>() = false;
    t1.get<2>() = "DEFG";
    BOOST_CHECK_EQUAL(40, t1.get<0>());
    BOOST_CHECK(!t1.get<1>());
    BOOST_CHECK_EQUAL("DEFG", t1.get<2>());
}

BOOST_AUTO_TEST_CASE(non_member_get)
{
    msgpack::type::tuple<int, bool, std::string> t1(42, true, "ABC");
    BOOST_CHECK_EQUAL(42, msgpack::type::get<0>(t1));
    BOOST_CHECK_EQUAL(true, msgpack::type::get<1>(t1));
    BOOST_CHECK_EQUAL("ABC", msgpack::type::get<2>(t1));
    msgpack::type::get<0>(t1) = 40;
    msgpack::type::get<1>(t1) = false;
    msgpack::type::get<2>(t1) = "DEFG";
    BOOST_CHECK_EQUAL(40, msgpack::type::get<0>(t1));
    BOOST_CHECK(!msgpack::type::get<1>(t1));
    BOOST_CHECK_EQUAL("DEFG", msgpack::type::get<2>(t1));
}

#if MSGPACK_CPP_VERSION >= 201103L

BOOST_AUTO_TEST_CASE(std_non_member_get)
{
    msgpack::type::tuple<int, bool, std::string> t1(42, true, "ABC");
    BOOST_CHECK_EQUAL(42, std::get<0>(t1));
    BOOST_CHECK_EQUAL(true, std::get<1>(t1));
    BOOST_CHECK_EQUAL("ABC", std::get<2>(t1));
    std::get<0>(t1) = 40;
    std::get<1>(t1) = false;
    std::get<2>(t1) = "DEFG";
    BOOST_CHECK_EQUAL(40, std::get<0>(t1));
    BOOST_CHECK(!std::get<1>(t1));
    BOOST_CHECK_EQUAL("DEFG", std::get<2>(t1));
}

BOOST_AUTO_TEST_CASE(make_tuple)
{
    msgpack::type::tuple<int, bool, std::string> t1 = msgpack::type::make_tuple(42, true, "ABC");
    BOOST_CHECK_EQUAL(42, t1.get<0>());
    BOOST_CHECK_EQUAL(true, t1.get<1>());
    BOOST_CHECK_EQUAL("ABC", t1.get<2>());
    t1.get<0>() = 40;
    t1.get<1>() = false;
    t1.get<2>() = "DEFG";
    BOOST_CHECK_EQUAL(40, t1.get<0>());
    BOOST_CHECK(!t1.get<1>());
    BOOST_CHECK_EQUAL("DEFG", t1.get<2>());
}

BOOST_AUTO_TEST_CASE(std_make_tuple)
{
    msgpack::type::tuple<int, bool, std::string> t1 = std::make_tuple(42, true, "ABC");
    BOOST_CHECK_EQUAL(42, t1.get<0>());
    BOOST_CHECK_EQUAL(true, t1.get<1>());
    BOOST_CHECK_EQUAL("ABC", t1.get<2>());
}

BOOST_AUTO_TEST_CASE(tie)
{
    int i(43);
    bool b(false);
    std::string s("DEFG");
    msgpack::type::tie(i, b, s) = msgpack::type::make_tuple(42, true, "ABC");
    BOOST_CHECK_EQUAL(42, i);
    BOOST_CHECK_EQUAL(true, b);
    BOOST_CHECK_EQUAL("ABC", s);
}

BOOST_AUTO_TEST_CASE(tuple_cat)
{
    msgpack::type::tuple<int> t1 = msgpack::type::make_tuple(42);
    msgpack::type::tuple<bool, std::string> t2 = msgpack::type::make_tuple(true, "ABC");
    msgpack::type::tuple<int, bool, std::string> t3 = msgpack::type::tuple_cat(t1, std::move(t2));
    BOOST_CHECK_EQUAL(42, t3.get<0>());
    BOOST_CHECK_EQUAL(true, t3.get<1>());
    BOOST_CHECK_EQUAL("ABC", t3.get<2>());
}

BOOST_AUTO_TEST_CASE(swap)
{
    msgpack::type::tuple<int, bool, std::string>  t1 = msgpack::type::make_tuple(42, true, "ABC");
    msgpack::type::tuple<int, bool, std::string>  t2 = msgpack::type::make_tuple(40, false, "DEFG");
    msgpack::type::swap(t1, t2);
    BOOST_CHECK_EQUAL(42, t2.get<0>());
    BOOST_CHECK_EQUAL(true, t2.get<1>());
    BOOST_CHECK_EQUAL("ABC", t2.get<2>());
    BOOST_CHECK_EQUAL(40, t1.get<0>());
    BOOST_CHECK(!t1.get<1>());
    BOOST_CHECK_EQUAL("DEFG", t1.get<2>());
}

#endif // MSGPACK_CPP_VERSION >= 201103L
