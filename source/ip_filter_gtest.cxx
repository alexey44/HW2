#include <gtest/gtest.h>

#include "IpFilter.h"

// class [+]
// gtest using

// read data
// parse data
// compare with etalon

// test on sort
// test on filter 46
// test on filter 46, 70
// test on filter any
// test on checksum

//// Demonstrate some basic assertions.
// TEST(HelloTest, BasicAssertions) {
//   // Expect two strings not to be equal.
//   EXPECT_STRNE("hello", "world");
//   // Expect equality.
//   EXPECT_EQ(7 * 6, 42);
// }

// namespace Test {

bool ReadTestDataFile(IpFilter& ipFilter) {
  std::string filename = "ip_filter.tsv";
  std::ifstream inFileStream;
  inFileStream.open(filename);

  if (inFileStream.is_open() == false) {
    std::cerr << "[ReadTestDataFile()] Can't open file! filename = " << filename
              << std::endl;
    return false;
  }

  ipFilter.ReadIpPool(inFileStream);

  inFileStream.close();
  return true;
}

// Demonstrate some basic assertions.
TEST(IpFilterTests, FilterFirstTest) {
  IpFilter ipFilter;
  if (ReadTestDataFile(ipFilter) == false) {
    GTEST_FATAL_FAILURE_("[IpFilterTests] Can't read test data from file");
  }
  ipFilter.PrintIp();

  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

//}  // namespace Test
