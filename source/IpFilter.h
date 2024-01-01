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
  IpFilter() = default;
  ~IpFilter() = default;

  /*
  // ("",  '.') -> [""]
  // ("11", '.') -> ["11"]
  // ("..", '.') -> ["", "", ""]
  // ("11.", '.') -> ["11", ""]
  // (".11", '.') -> ["", "11"]
  // ("11.22", '.') -> ["11", "22"]
  */

  using StrVec = std::vector<std::string>;
  using IpPoolStr = std::vector<StrVec>;
  using IpTuple = std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>;
  using IpPoolTuple = std::vector<IpTuple>;

  static StrVec Split(const std::string& str, char d);

  void ReadIpPool(std::ifstream& inFileStream);
  void PrintIp();
  void PrintIp(const IpPoolStr& ipPool);
  void IpSort();
  IpPoolStr Filter(int16_t firstByte, int16_t secondByte = -1);
  IpPoolStr FilterAny(uint8_t val);

  static StrVec MakeIpStr(const IpPoolStr& ipPool);

  IpPoolStr ipPoolStr;

 private:
  std::vector<IpTuple> sortIpPool;
};
