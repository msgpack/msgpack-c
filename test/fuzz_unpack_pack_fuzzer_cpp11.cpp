#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"

#include <gtest/gtest.h>

#pragma GCC diagnostic pop

#include "../fuzz/unpack_pack_fuzzer.cpp"

TEST(FUZZ_UNPACK_PACK_FUZZER, works)
{
  EXPECT_EQ(0, LLVMFuzzerTestOneInput(0, 0));
}
