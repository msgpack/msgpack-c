#include <sstream>
#include <msgpack.hpp>

#define BOOST_TEST_MODULE size_equal_only
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(array)
{
    std::stringstream ss;
    int buf[3] = { 1, 2, 3 };
    msgpack::type::size_equal_only<int[3]> seo(buf);

    msgpack::pack(ss, seo);
    std::string const& str = ss.str();
    msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());

    int ret_buf1[3];
    oh.get().convert(ret_buf1);
    BOOST_CHECK_EQUAL(buf[0], ret_buf1[0]);
    BOOST_CHECK_EQUAL(buf[1], ret_buf1[1]);
    BOOST_CHECK_EQUAL(buf[2], ret_buf1[2]);

    int ret_buf2[4];
    oh.get().convert(ret_buf2);
    BOOST_CHECK_EQUAL(buf[0], ret_buf2[0]);
    BOOST_CHECK_EQUAL(buf[1], ret_buf2[1]);
    BOOST_CHECK_EQUAL(buf[2], ret_buf2[2]);

    int ret_buf3[3];
    msgpack::type::size_equal_only<int[3]> ret_seo3(ret_buf3);
    oh.get().convert(ret_seo3);
    BOOST_CHECK_EQUAL(buf[0], ret_buf3[0]);
    BOOST_CHECK_EQUAL(buf[1], ret_buf3[1]);
    BOOST_CHECK_EQUAL(buf[2], ret_buf3[2]);

    int ret_buf4[4];
    msgpack::type::size_equal_only<int[4]> ret_seo4(ret_buf4);
    try {
        oh.get().convert(ret_seo4);
        BOOST_CHECK(false);
    }
    catch (msgpack::type_error const&) {
        BOOST_CHECK(true);
    }
}

BOOST_AUTO_TEST_CASE(vector)
{
    std::stringstream ss;
    std::vector<int> buf;
    buf.push_back(1);
    buf.push_back(2);
    buf.push_back(3);

    msgpack::type::size_equal_only<std::vector<int> > seo(buf);

    msgpack::pack(ss, seo);
    std::string const& str = ss.str();
    msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());

    std::vector<int> ret_buf1;
    oh.get().convert(ret_buf1);
    BOOST_CHECK(buf == ret_buf1);


    std::vector<int> ret_buf2;
    ret_buf2.resize(3);
    msgpack::type::size_equal_only<std::vector<int> > ret_seo2(ret_buf2);
    oh.get().convert(ret_seo2);
    BOOST_CHECK(buf == ret_buf2);

    std::vector<int> ret_buf3;
    ret_buf2.resize(4);
    msgpack::type::size_equal_only<std::vector<int> > ret_seo3(ret_buf3);
    try {
        oh.get().convert(ret_seo3);
        BOOST_CHECK(false);
    }
    catch (msgpack::type_error const&) {
        BOOST_CHECK(true);
    }
}

BOOST_AUTO_TEST_CASE(msgpack_tuple)
{
    std::stringstream ss;
    msgpack::type::tuple<int, bool, std::string> buf(1, false, "ABC");

    msgpack::type::size_equal_only<msgpack::type::tuple<int, bool, std::string> > seo(buf);

    msgpack::pack(ss, seo);
    std::string const& str = ss.str();
    msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());

    msgpack::type::tuple<int, bool, std::string> ret_buf1;
    oh.get().convert(ret_buf1);
    BOOST_CHECK_EQUAL(buf.get<0>(), ret_buf1.get<0>());
    BOOST_CHECK_EQUAL(buf.get<1>(), ret_buf1.get<1>());
    BOOST_CHECK_EQUAL(buf.get<2>(), ret_buf1.get<2>());

    msgpack::type::tuple<int, bool, std::string> ret_buf2;
    msgpack::type::size_equal_only<msgpack::type::tuple<int, bool, std::string> > ret_seo2(ret_buf2);
    oh.get().convert(ret_seo2);
    BOOST_CHECK_EQUAL(buf.get<0>(), ret_buf2.get<0>());
    BOOST_CHECK_EQUAL(buf.get<1>(), ret_buf2.get<1>());
    BOOST_CHECK_EQUAL(buf.get<2>(), ret_buf2.get<2>());

    msgpack::type::tuple<int, bool, std::string, int> ret_buf3;
    oh.get().convert(ret_buf3);
    BOOST_CHECK_EQUAL(buf.get<0>(), ret_buf3.get<0>());
    BOOST_CHECK_EQUAL(buf.get<1>(), ret_buf3.get<1>());
    BOOST_CHECK_EQUAL(buf.get<2>(), ret_buf3.get<2>());

    msgpack::type::tuple<int, bool, std::string, int> ret_buf4;
    msgpack::type::size_equal_only<msgpack::type::tuple<int, bool, std::string, int> > ret_seo4(ret_buf4);
    try {
        oh.get().convert(ret_seo4);
        BOOST_CHECK(false);
    }
    catch (msgpack::type_error const&) {
        BOOST_CHECK(true);
    }

    msgpack::type::tuple<int, bool, std::string> ret_buf5;
    oh.get().convert(ret_buf5);
    BOOST_CHECK_EQUAL(buf.get<0>(), ret_buf5.get<0>());
    BOOST_CHECK_EQUAL(buf.get<1>(), ret_buf5.get<1>());

    msgpack::type::tuple<int, bool, std::string, int> ret_buf6;
    msgpack::type::size_equal_only<msgpack::type::tuple<int, bool, std::string, int> > ret_seo6(ret_buf6);
    try {
        oh.get().convert(ret_seo6);
        BOOST_CHECK(false);
    }
    catch (msgpack::type_error const&) {
        BOOST_CHECK(true);
    }
}

#if !defined(MSGPACK_USE_CPP03)

BOOST_AUTO_TEST_CASE(tuple)
{
    std::stringstream ss;
    std::tuple<int, bool, std::string> buf(1, false, "ABC");

    auto seo = msgpack::type::make_size_equal_only(buf);

    msgpack::pack(ss, seo);
    std::string const& str = ss.str();
    msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());

    std::tuple<int, bool, std::string> ret_buf1;
    oh.get().convert(ret_buf1);
    BOOST_CHECK(buf == ret_buf1);

    std::tuple<int, bool, std::string> ret_buf2;
    auto ret_seo2 = msgpack::type::make_size_equal_only(ret_buf2);
    oh.get().convert(ret_seo2);
    BOOST_CHECK(buf == ret_buf2);

    std::tuple<int, bool, std::string, int> ret_buf3;
    oh.get().convert(ret_buf3);
    BOOST_CHECK_EQUAL(std::get<0>(buf), std::get<0>(ret_buf3));
    BOOST_CHECK_EQUAL(std::get<1>(buf), std::get<1>(ret_buf3));
    BOOST_CHECK_EQUAL(std::get<2>(buf), std::get<2>(ret_buf3));

    std::tuple<int, bool, std::string, int> ret_buf4;
    auto ret_seo4 = msgpack::type::make_size_equal_only(ret_buf4);
    try {
        oh.get().convert(ret_seo4);
        BOOST_CHECK(false);
    }
    catch (msgpack::type_error const&) {
        BOOST_CHECK(true);
    }

    std::tuple<int, bool, std::string> ret_buf5;
    oh.get().convert(ret_buf5);
    BOOST_CHECK_EQUAL(std::get<0>(buf), std::get<0>(ret_buf5));
    BOOST_CHECK_EQUAL(std::get<1>(buf), std::get<1>(ret_buf5));

    std::tuple<int, bool, std::string, int> ret_buf6;
    auto ret_seo6 = msgpack::type::make_size_equal_only(ret_buf6);
    try {
        oh.get().convert(ret_seo6);
        BOOST_CHECK(false);
    }
    catch (msgpack::type_error const&) {
        BOOST_CHECK(true);
    }
}

struct foo1 {
    foo1(int i, bool b):t(i, b), seo(t) {}
    std::tuple<int, bool> t;
    msgpack::type::size_equal_only<std::tuple<int, bool> > seo;
    MSGPACK_DEFINE(seo);
};

struct foo2 {
    foo2(int i, bool b, std::string const& s):t(i, b, s), seo(t) {}
    std::tuple<int, bool, std::string> t;
    msgpack::type::size_equal_only<std::tuple<int, bool, std::string> > seo;
    MSGPACK_DEFINE(seo);
};

BOOST_AUTO_TEST_CASE(custom_class)
{
    std::stringstream ss;
    foo1 f1(42, true);
    msgpack::pack(ss, f1);
    std::string const& str = ss.str();
    msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());

    foo2 f2(123, false, "ABC");
    try {
        oh.get().convert(f2);
        BOOST_CHECK(false);
    }
    catch (msgpack::type_error const&) {
        BOOST_CHECK(true);
    }
}

#endif //  !defined(MSGPACK_USE_CPP03)
