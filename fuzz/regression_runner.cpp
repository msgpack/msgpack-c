#include "unpack_pack_fuzzer.cpp"
#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include <vector>

namespace {

  using ::testing::TestWithParam;
  using ::testing::Values;

  extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size);

  class UnpackPackFuzzerRegressionTest : public ::testing::TestWithParam<const char *> {
  public:
  };

  TEST_P(UnpackPackFuzzerRegressionTest, Returns0) {
    auto fpath = GetParam();
    std::ifstream in(fpath, std::ifstream::binary);
    if (!in) {
      FAIL() << fpath << " not found" << std::endl;
    }

    in.seekg(0, in.end);
    size_t length = in.tellg();
    in.seekg(0, in.beg);
    std::vector<char> bytes(length);
    in.read(bytes.data(), bytes.size());

    assert(in);

    EXPECT_EQ(0, LLVMFuzzerTestOneInput(reinterpret_cast<const uint8_t *>(bytes.data()),
                                        bytes.size()));
  }

  // this should use something like CMAKE's External_data
  // also, it should do this by looking at the directory, not statically.
  INSTANTIATE_TEST_CASE_P(UnpackPackFuzzerRegressions,
                          UnpackPackFuzzerRegressionTest,
                          ::testing::Values("../../fuzz/unpack_pack_fuzzer_regressions/clusterfuzz-testcase-minimized-unpack_pack_fuzzer-5656982724804608",
                                            "../../fuzz/unpack_pack_fuzzer_regressions/clusterfuzz-testcase-minimized-unpack_pack_fuzzer-6022481354686464"));
};
