#include "IpFilter.h"

#include <limits>

namespace {
// static const uint64_t maxReadNum = 80;
static const uint64_t maxReadNum = std::numeric_limits<uint64_t>::max();
static const int32_t ipSize = 4;
}  // namespace

/*
// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
*/
IpFilter::StrVec IpFilter::Split(const std::string &str, char d) {
  StrVec r;

  std::string::size_type start = 0;
  std::string::size_type stop = str.find_first_of(d);
  while (stop != std::string::npos) {
    r.push_back(str.substr(start, stop - start));

    start = stop + 1;
    stop = str.find_first_of(d, start);
  }

  r.push_back(str.substr(start));

  return r;
}

IpFilter::StrVec IpFilter::MakeIpStr(const IpPoolStr &ipPool) {
  StrVec strVec;
  strVec.reserve(ipPool.size());

  for (IpPoolStr::const_iterator ip = ipPool.cbegin(); ip != ipPool.cend();
       ++ip) {
    std::string tmpStr = "";
    for (StrVec::const_iterator ip_part = ip->cbegin(); ip_part != ip->cend();
         ++ip_part) {
      if (ip_part != ip->cbegin()) {
        tmpStr.append(".");
      }
      tmpStr.append(*ip_part);
    }
    //    tmpStr.append("\n");
    strVec.emplace_back(std::move(tmpStr));
  }
  return strVec;
}

void IpFilter::PrintIp() { PrintIp(ipPoolStr); }

void IpFilter::PrintIp(const IpPoolStr &ipPool) {
  for (IpPoolStr::const_iterator ip = ipPool.cbegin(); ip != ipPool.cend();
       ++ip) {
    for (StrVec::const_iterator ip_part = ip->cbegin(); ip_part != ip->cend();
         ++ip_part) {
      if (ip_part != ip->cbegin()) {
        std::cout << ".";
      }
      std::cout << *ip_part;
    }
    std::cout << std::endl;
  }
}

void IpFilter::ReadIpPool(std::ifstream &inFileStream) {
  uint64_t readCnt = 0;
  for (std::string line; std::getline(inFileStream, line);) {
    StrVec v = Split(line, '\t');
    ipPoolStr.push_back(Split(v.at(0), '.'));
    if (++readCnt > maxReadNum) {
      break;
    }
  }
}

void IpFilter::IpSort() {
  sortIpPool.reserve(ipPoolStr.size());

  for (const auto &line : ipPoolStr) {
    assert(line.size() == ipSize);
    sortIpPool.emplace_back(
        std::make_tuple(static_cast<uint8_t>(std::stoi(line[0])),
                        static_cast<uint8_t>(std::stoi(line[1])),
                        static_cast<uint8_t>(std::stoi(line[2])),
                        static_cast<uint8_t>(std::stoi(line[3]))));
  }

  std::sort(sortIpPool.begin(), sortIpPool.end(),
            [](const IpTuple &a, const IpTuple &b) { return a > b; });

  //    for (const auto& p: sortIpPool)
  //        std::cout << "{ " << std::get<0>(p)
  //                  << ", " << std::get<1>(p)
  //                  << ", " << std::get<2>(p)
  //                  << ", " << std::get<3>(p)
  //                  << " }\n";

  for (uint32_t i = 0; i < ipPoolStr.size(); ++i) {
    ipPoolStr[i][0] = std::to_string(std::get<0>(sortIpPool[i]));
    ipPoolStr[i][1] = std::to_string(std::get<1>(sortIpPool[i]));
    ipPoolStr[i][2] = std::to_string(std::get<2>(sortIpPool[i]));
    ipPoolStr[i][3] = std::to_string(std::get<3>(sortIpPool[i]));
  }

  //    printIp(ipPoolStr);

  return;
}

IpFilter::IpPoolStr IpFilter::Filter(int16_t firstByte, int16_t secondByte) {
  IpPoolStr filteredPool;
  for (const auto &ip : sortIpPool) {
    if (std::get<0>(ip) == firstByte) {
      bool needToAdd = false;
      if (secondByte != -1) {
        if (std::get<1>(ip) == secondByte) {
          needToAdd = true;
        }
      } else {
        needToAdd = true;
      }

      if (needToAdd) {
        auto vv = StrVec{
            std::to_string(std::get<0>(ip)), std::to_string(std::get<1>(ip)),
            std::to_string(std::get<2>(ip)), std::to_string(std::get<3>(ip))};
        filteredPool.emplace_back(std::move(vv));
      }
    }
  }
  return filteredPool;
}

IpFilter::IpPoolStr IpFilter::FilterAny(uint8_t val) {
  IpPoolStr filteredPool;
  uint8_t ips[4];
  for (const auto &ip : sortIpPool) {
    std::tie(ips[0], ips[1], ips[2], ips[3]) = ip;
    bool needToAdd = false;
    for (const auto &el : ips) {
      if (el == val) {
        needToAdd = true;
        break;
      }
    }
    if (needToAdd) {
      auto vv = StrVec{
          std::to_string(std::get<0>(ip)), std::to_string(std::get<1>(ip)),
          std::to_string(std::get<2>(ip)), std::to_string(std::get<3>(ip))};
      filteredPool.emplace_back(std::move(vv));
    }
  }
  return filteredPool;
}
