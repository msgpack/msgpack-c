#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "unpack_pack_fuzzer.cpp"

using ::testing::TestWithParam;
using ::testing::ValuesIn;


std::vector<std::string> * ListDirectory(const char * path) {
  auto v = new std::vector<std::string>();

  boost::filesystem::path p(path);
  boost::filesystem::directory_iterator f{p};

  if(boost::filesystem::is_directory(p)) {
    while (f != boost::filesystem::directory_iterator{}) {
      v->push_back(boost::filesystem::canonical(*f++).string());
    }
  }
  return v;
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size);

class UnpackPackFuzzerRegressionTest : public ::testing::TestWithParam<const char *> {
public:
};

TEST_P(UnpackPackFuzzerRegressionTest, Returns0) {
  auto fpath = GetParam();
  std::ifstream in(fpath, std::ifstream::binary);
  if (!in) {
    FAIL() << fpath << " not found";
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

INSTANTIATE_TEST_CASE_P(UnpackPackFuzzerRegressions,
                        UnpackPackFuzzerRegressionTest,
                        ::testing::ValuesIn(ListDirectory("../../fuzz/unpack_pack_regressions")->data()));
                        // "../../fuzz/unpack_pack_fuzzer_regressions/clusterfuzz-testcase-minimized-unpack_pack_fuzzer-5656982724804608",
                        // "../../fuzz/unpack_pack_fuzzer_regressions/clusterfuzz-testcase-minimized-unpack_pack_fuzzer-6022481354686464"));
