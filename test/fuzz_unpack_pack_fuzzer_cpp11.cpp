#define BOOST_TEST_MODULE FUZZ_UNPACK_PACK_FUZZER
#include <boost/test/unit_test.hpp>

#include "../fuzz/unpack_pack_fuzzer.cpp"

BOOST_AUTO_TEST_CASE(works)
{
  BOOST_CHECK_EQUAL(0, LLVMFuzzerTestOneInput(MSGPACK_NULLPTR, 0));
}
