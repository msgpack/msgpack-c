#include <boost/filesystem.hpp>

// Use parameterized tests instead of modern data-driven test cases
// because BOOST_DATA_TEST_CASE requires C++11 or newer. See:
// - https://www.boost.org/doc/libs/1_76_0/libs/test/doc/html/boost_test/tests_organization/test_cases/param_test.html
// - https://www.boost.org/doc/libs/1_76_0/libs/test/doc/html/boost_test/tests_organization/test_cases/test_case_generation.html
#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>

#include <fstream>
#include <string>
#include <vector>

#include "unpack_pack_fuzzer.cpp"

std::vector<std::string> ListDirectory(const std::string& path) {
  std::vector<std::string> v;

  boost::filesystem::path p(path);
  boost::filesystem::directory_iterator f(p);

  if (boost::filesystem::is_directory(p)) {
    while (f != boost::filesystem::directory_iterator()) {
      v.push_back((*f++).path().string());
    }
  }
  return v;
}

void UnpackPackFuzzerRegressionTest(const std::string& fpath) {
  std::ifstream in(fpath.c_str(), std::ios_base::binary);
  if (!in) {
    BOOST_FAIL(fpath + " not found");
  }
  in.seekg(0, in.end);
  size_t length = in.tellg();
  in.seekg(0, in.beg);
  std::vector<char> bytes(length);
  in.read(bytes.data(), bytes.size());
  BOOST_REQUIRE(in);
  BOOST_REQUIRE_EQUAL(0, LLVMFuzzerTestOneInput(reinterpret_cast<const uint8_t *>(bytes.data()), bytes.size()));
}

boost::unit_test::test_suite* init_unit_test_suite(int /*argc*/, char* /*argv*/[])
{
  std::vector<std::string> files = ListDirectory("../../fuzz/unpack_pack_fuzzer_regressions");
  boost::unit_test::framework::master_test_suite().add(BOOST_PARAM_TEST_CASE(&UnpackPackFuzzerRegressionTest, files.begin(), files.end()));
  return 0;
}
