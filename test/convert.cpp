#include <msgpack.hpp>

#define BOOST_TEST_MODULE convert
#include <boost/test/unit_test.hpp>

class enum_member {
public:
    enum_member() : flag(A) { }

    enum flags_t {
        A = 0,
        B = 1
    };

    flags_t flag;

    MSGPACK_DEFINE(flag);
};

MSGPACK_ADD_ENUM(enum_member::flags_t);

class compatibility {
public:
    compatibility() : str1("default"), str2("default") { }

    std::string str1;
    std::string str2;

    MSGPACK_DEFINE(str1, str2);
};

BOOST_AUTO_TEST_CASE(compatibility_less)
{
    std::vector<std::string> src(1);
    src[0] = "kumofs";

    msgpack::zone z;
    msgpack::object obj(src, z);

    compatibility c;
    BOOST_CHECK_NO_THROW( obj.convert(c) );

    BOOST_CHECK_EQUAL("kumofs",  c.str1);
    BOOST_CHECK_EQUAL("default", c.str2);
}

BOOST_AUTO_TEST_CASE(compatibility_more)
{
    std::vector<std::string> src(3);
    src[0] = "kumofs";
    src[1] = "mpio";
    src[2] = "cloudy";

    msgpack::zone z;
    msgpack::object obj(src, z);

    compatibility to;
    BOOST_CHECK_NO_THROW( obj.convert(to) );

    BOOST_CHECK_EQUAL("kumofs", to.str1);
    BOOST_CHECK_EQUAL("mpio",   to.str2);
}

BOOST_AUTO_TEST_CASE(enum_member_)
{
    enum_member src;
    src.flag = enum_member::B;

    msgpack::zone z;
    msgpack::object obj(src, z);

    enum_member to;
    BOOST_CHECK_NO_THROW( obj.convert(to) );

    BOOST_CHECK_EQUAL(enum_member::B, to.flag);
}

BOOST_AUTO_TEST_CASE(return_value_ref)
{
    msgpack::zone z;
    msgpack::object obj(1, z);

    int i;
    int const& j = obj.convert(i);
    BOOST_CHECK_EQUAL(&i, &j);
    BOOST_CHECK_EQUAL(i, j);
}

#if MSGPACK_DEFAULT_API_VERSION == 1 && !defined(MSGPACK_DISABLE_LEGACY_CONVERT)

BOOST_AUTO_TEST_CASE(return_value_ptr)
{
    msgpack::zone z;
    msgpack::object obj(1, z);

    int i;
    // obsolete
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
    BOOST_CHECK_EQUAL(obj.convert(&i), &i);
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
    BOOST_CHECK_EQUAL(1, i);
}

#endif // MSGPACK_DEFAULT_API_VERSION == 1 && !defined(MSGPACK_DISABLE_LEGACY_CONVERT)

BOOST_AUTO_TEST_CASE(if_not_nil_nil)
{
    msgpack::object obj;
    int i;
    BOOST_CHECK(!obj.convert_if_not_nil(i));
}

BOOST_AUTO_TEST_CASE(if_not_nil_not_nil)
{
    msgpack::zone z;
    msgpack::object obj(1, z);

    int i;
    BOOST_CHECK(obj.convert_if_not_nil(i));
    BOOST_CHECK_EQUAL(i, 1);
}
