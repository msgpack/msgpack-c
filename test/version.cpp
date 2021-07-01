#include <msgpack.hpp>

#define BOOST_TEST_MODULE version
#include <boost/test/unit_test.hpp>

#include <cstdio>

BOOST_AUTO_TEST_CASE(print)
{
    std::printf("MSGPACK_VERSION            : %s\n", MSGPACK_VERSION);
    std::printf("MSGPACK_VERSION_MAJOR      : %d\n", MSGPACK_VERSION_MAJOR);
    std::printf("MSGPACK_VERSION_MINOR      : %d\n", MSGPACK_VERSION_MINOR);
    std::printf("MSGPACK_VERSION_REVISION   : %d\n", MSGPACK_VERSION_REVISION);
    std::printf("msgpack_version()          : %s\n", msgpack_version());
    std::printf("msgpack_version_major()    : %d\n", msgpack_version_major());
    std::printf("msgpack_version_minor()    : %d\n", msgpack_version_minor());
    std::printf("msgpack_version_revision() : %d\n", msgpack_version_revision());
}
