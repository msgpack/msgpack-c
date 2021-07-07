#include <msgpack.hpp>

#define BOOST_TEST_MODULE fixint
#include <boost/test/unit_test.hpp>

template <typename T>
void check_size(size_t size) {
    T v(0);
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, v);
    BOOST_CHECK_EQUAL(size, sbuf.size());
}

BOOST_AUTO_TEST_CASE(size)
{
    check_size<msgpack::type::fix_int8>(2);
    check_size<msgpack::type::fix_int16>(3);
    check_size<msgpack::type::fix_int32>(5);
    check_size<msgpack::type::fix_int64>(9);

    check_size<msgpack::type::fix_uint8>(2);
    check_size<msgpack::type::fix_uint16>(3);
    check_size<msgpack::type::fix_uint32>(5);
    check_size<msgpack::type::fix_uint64>(9);
}


template <typename T>
void check_convert() {
    T v1(typename T::value_type(-11));
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, v1);

    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());

    T v2;
    oh.get().convert(v2);

    BOOST_CHECK_EQUAL(v1.get(), v2.get());

    BOOST_CHECK_EQUAL(oh.get(), msgpack::object(T(v1.get())));
}

BOOST_AUTO_TEST_CASE(convert)
{
    check_convert<msgpack::type::fix_int8>();
    check_convert<msgpack::type::fix_int16>();
    check_convert<msgpack::type::fix_int32>();
    check_convert<msgpack::type::fix_int64>();

    check_convert<msgpack::type::fix_uint8>();
    check_convert<msgpack::type::fix_uint16>();
    check_convert<msgpack::type::fix_uint32>();
    check_convert<msgpack::type::fix_uint64>();
}
