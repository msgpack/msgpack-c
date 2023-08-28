#include <msgpack.hpp>

#define BOOST_TEST_MODULE nan
#include <boost/test/unit_test.hpp>

#include <sstream>
#include <limits>
#include <cfloat>
#include <cmath>
#include <cstdlib>

#if defined (_MSC_VER)

#include <cfenv>

template <typename T>
inline bool is_quiet_nan(T t) {
    std::feclearexcept(FE_ALL_EXCEPT);
    T r = t * T();
    (void)r;
    int n = std::fetestexcept(FE_ALL_EXCEPT);
    if (n == 0) {
        return true;
    }
    else {
        return false;
    }
}

#else  // defined (_MSC_VER)

inline bool is_quiet_nan(double const& nan_val) {
    MSGPACK_ASSERT(nan_val != nan_val);
    uint64_t bit_pattern = reinterpret_cast<uint64_t const&>(nan_val);
    int is_quiet_bit_index = DBL_MANT_DIG - 2;
    return (bit_pattern >> is_quiet_bit_index) & 1;
}

inline bool is_quiet_nan(float const& nan_val) {
    MSGPACK_ASSERT(nan_val != nan_val);
    uint32_t bit_pattern = reinterpret_cast<uint32_t const&>(nan_val);
    int is_quiet_bit_index = FLT_MANT_DIG - 2;
    return (bit_pattern >> is_quiet_bit_index) & 1;
}

#endif // defined (_MSC_VER)

BOOST_AUTO_TEST_CASE(unpack_float_signaling)
{
    if (!std::numeric_limits<float>::has_signaling_NaN) {
        return;
    }
    if (sizeof(void*) == 4) { // skip 32bit arch
        return;
    }

    std::stringstream ss;
    float val = std::numeric_limits<float>::signaling_NaN();

    msgpack::pack(ss, val);

    try {
        msgpack::object_handle oh =
            msgpack::unpack(ss.str().c_str(), ss.str().size());
        BOOST_CHECK(!is_quiet_nan(oh->via.f32));
        BOOST_CHECK(!is_quiet_nan(oh->as<float>()));
        BOOST_CHECK(is_quiet_nan(oh->as<double>())); // convert removes signaling
    }
    catch(...) {
        BOOST_CHECK(false);
    }
}

BOOST_AUTO_TEST_CASE(unpack_float_quiet)
{
    if (!std::numeric_limits<float>::has_quiet_NaN) {
        return;
    }
    if (sizeof(void*) == 4) { // skip 32bit arch
        return;
    }

    std::stringstream ss;
    float val = std::numeric_limits<float>::quiet_NaN();
    msgpack::pack(ss, val);

    try {
        msgpack::object_handle oh =
            msgpack::unpack(ss.str().c_str(), ss.str().size());
        BOOST_CHECK(is_quiet_nan(oh->via.f32));
        BOOST_CHECK(is_quiet_nan(oh->as<float>()));
        BOOST_CHECK(is_quiet_nan(oh->as<double>()));
    }
    catch(...) {
        BOOST_CHECK(false);
    }
}

BOOST_AUTO_TEST_CASE(unpack_double_signaling)
{
    if (!std::numeric_limits<double>::has_signaling_NaN) {
        return;
    }
    if (sizeof(void*) == 4) { // skip 32bit arch
        return;
    }

    std::stringstream ss;
    double val = std::numeric_limits<double>::signaling_NaN();

    msgpack::pack(ss, val);

    try {
        msgpack::object_handle oh =
            msgpack::unpack(ss.str().c_str(), ss.str().size());
        BOOST_CHECK(!is_quiet_nan(oh->via.f64));
        BOOST_CHECK(is_quiet_nan(oh->as<float>())); // convert removes signaling
        BOOST_CHECK(!is_quiet_nan(oh->as<double>()));
    }
    catch(...) {
        BOOST_CHECK(false);
    }
}

BOOST_AUTO_TEST_CASE(unpack_double_quiet)
{
    if (!std::numeric_limits<double>::has_quiet_NaN) {
        return;
    }
    if (sizeof(void*) == 4) { // skip 32bit arch
        return;
    }

    std::stringstream ss;
    double val = std::numeric_limits<double>::quiet_NaN();
    msgpack::pack(ss, val);

    try {
        msgpack::object_handle oh =
            msgpack::unpack(ss.str().c_str(), ss.str().size());
        BOOST_CHECK(is_quiet_nan(oh->via.f64));
        BOOST_CHECK(is_quiet_nan(oh->as<float>()));
        BOOST_CHECK(is_quiet_nan(oh->as<double>()));
    }
    catch(...) {
        BOOST_CHECK(false);
    }
}


BOOST_AUTO_TEST_CASE(object_float_signaling)
{
    if (!std::numeric_limits<float>::has_signaling_NaN) {
        return;
    }
    if (sizeof(void*) == 4) { // skip 32bit arch
        return;
    }

    float val = std::numeric_limits<float>::signaling_NaN();

    msgpack::object obj(val);

    BOOST_CHECK(!is_quiet_nan(obj.via.f32));
    BOOST_CHECK(!is_quiet_nan(obj.as<float>()));
    BOOST_CHECK(is_quiet_nan(obj.as<double>())); // convert removes signaling
}

BOOST_AUTO_TEST_CASE(object_float_quiet)
{
    if (!std::numeric_limits<float>::has_quiet_NaN) {
        return;
    }
    if (sizeof(void*) == 4) { // skip 32bit arch
        return;
    }

    float val = std::numeric_limits<float>::quiet_NaN();

    msgpack::object obj(val);

    BOOST_CHECK(is_quiet_nan(obj.via.f32));
    BOOST_CHECK(is_quiet_nan(obj.as<float>()));
    BOOST_CHECK(is_quiet_nan(obj.as<double>()));
}

BOOST_AUTO_TEST_CASE(object_double_signaling)
{
    if (!std::numeric_limits<double>::has_signaling_NaN) {
        return;
    }
    if (sizeof(void*) == 4) { // skip 32bit arch
        return;
    }

    double val = std::numeric_limits<double>::signaling_NaN();

    msgpack::object obj(val);

    BOOST_CHECK(!is_quiet_nan(obj.via.f64));
    BOOST_CHECK(is_quiet_nan(obj.as<float>())); // convert removes signaling
    BOOST_CHECK(!is_quiet_nan(obj.as<double>()));
}

BOOST_AUTO_TEST_CASE(object_double_quiet)
{
    if (!std::numeric_limits<double>::has_quiet_NaN) {
        return;
    }
    if (sizeof(void*) == 4) { // skip 32bit arch
        return;
    }

    double val = std::numeric_limits<double>::quiet_NaN();

    msgpack::object obj(val);

    BOOST_CHECK(is_quiet_nan(obj.via.f64));
    BOOST_CHECK(is_quiet_nan(obj.as<float>()));
    BOOST_CHECK(is_quiet_nan(obj.as<double>()));
}

BOOST_AUTO_TEST_CASE(object_with_zone_float_signaling)
{
    if (!std::numeric_limits<float>::has_signaling_NaN) {
        return;
    }
    if (sizeof(void*) == 4) { // skip 32bit arch
        return;
    }

    float val = std::numeric_limits<float>::signaling_NaN();

    msgpack::zone z;
    msgpack::object obj(val, z);

    BOOST_CHECK(!is_quiet_nan(obj.via.f32));
    BOOST_CHECK(!is_quiet_nan(obj.as<float>()));
    BOOST_CHECK(is_quiet_nan(obj.as<double>())); // convert removes signaling
}

BOOST_AUTO_TEST_CASE(object_with_zone_float_quiet)
{
    if (!std::numeric_limits<float>::has_quiet_NaN) {
        return;
    }
    if (sizeof(void*) == 4) { // skip 32bit arch
        return;
    }

    float val = std::numeric_limits<float>::quiet_NaN();

    msgpack::zone z;
    msgpack::object obj(val, z);

    BOOST_CHECK(is_quiet_nan(obj.via.f32));
    BOOST_CHECK(is_quiet_nan(obj.as<float>()));
    BOOST_CHECK(is_quiet_nan(obj.as<double>()));
}

BOOST_AUTO_TEST_CASE(object_with_zone_double_signaling)
{
    if (!std::numeric_limits<double>::has_signaling_NaN) {
        return;
    }
    if (sizeof(void*) == 4) { // skip 32bit arch
        return;
    }

    double val = std::numeric_limits<double>::signaling_NaN();

    msgpack::zone z;
    msgpack::object obj(val, z);

    BOOST_CHECK(!is_quiet_nan(obj.via.f64));
    BOOST_CHECK(is_quiet_nan(obj.as<float>())); // convert removes signaling
    BOOST_CHECK(!is_quiet_nan(obj.as<double>()));
}

BOOST_AUTO_TEST_CASE(object_with_zone_double_quiet)
{
    if (!std::numeric_limits<double>::has_quiet_NaN) {
        return;
    }
    if (sizeof(void*) == 4) { // skip 32bit arch
        return;
    }

    double val = std::numeric_limits<double>::quiet_NaN();

    msgpack::zone z;
    msgpack::object obj(val, z);

    BOOST_CHECK(is_quiet_nan(obj.via.f64));
    BOOST_CHECK(is_quiet_nan(obj.as<float>()));
    BOOST_CHECK(is_quiet_nan(obj.as<double>()));
}
