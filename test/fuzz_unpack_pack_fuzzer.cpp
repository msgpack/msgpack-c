#include <gtest/gtest.h>

#include "../fuzz/unpack_pack_fuzzer.cc"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size);

TEST(FUZZ_UNPACK_PACK_FUZZER, works)
{
  EXPECT_EQ(0, LLVMFuzzerTestOneInput(0, 0));
}
