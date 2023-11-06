#pragma once

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

class IpFilter {
 public:
  //  IpFilter() = default;
  //  ~IpFilter() = default;

  /*
  // ("",  '.') -> [""]
  // ("11", '.') -> ["11"]
  // ("..", '.') -> ["", "", ""]
  // ("11.", '.') -> ["11", ""]
  // (".11", '.') -> ["", "11"]
  // ("11.22", '.') -> ["11", "22"]
  */
  static std::vector<std::string> Split(const std::string& str, char d);

  using IpPoolStr = std::vector<std::vector<std::string> >;
  using IpTuple = std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>;
  using IpPoolTuple = std::vector<IpTuple>;

  void ReadIpPool(std::ifstream& inFileStream);
  void PrintIp();
  void PrintIp(const IpPoolStr& ipPool);
  void IpSort();
  IpPoolStr Filter(int16_t firstByte, int16_t secondByte = -1);
  IpPoolStr FilterAny(uint8_t val);

  IpPoolStr ipPoolStr;

 private:
  int t = 0;
  //  std::vector<std::string> Split(const std::string& str, char d);

  std::vector<IpTuple> sortIpPool;
};
