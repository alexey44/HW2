
#include <cstdlib>
#include <iostream>
#include <string>

#include "IpFilter.h"

// int main(int argc, char const *argv[])
int main() {
  try {
    //
    IpFilter ipFilter;

    bool debugMode = false;
    if (debugMode) {
      std::string filename = "ip_filter.tsv";
      std::ifstream inFileStream;
      inFileStream.open(filename);

      if (inFileStream.is_open() == false) {
        std::cerr << "[ip_filter] Can't open file! filename = " << filename
                  << std::endl;
        return -1;
      }

      ipFilter.ReadIpPool(inFileStream);

      inFileStream.close();
    } else {  // release mode
      for (std::string line; std::getline(std::cin, line);) {
        IpFilter::StrVec v = IpFilter::Split(line, '\t');
        ipFilter.ipPoolStr.push_back(IpFilter::Split(v.at(0), '.'));
      }
    }

    // TODO reverse lexicographically sort
    ipFilter.IpSort();
    //        std::cout << "ipSort ===================================" <<
    //        std::endl;
    ipFilter.PrintIp();

    // 222.173.235.246
    // 222.130.177.64
    // 222.82.198.61
    // ...
    // 1.70.44.170
    // 1.29.168.152
    // 1.1.234.8

    // TODO filter by first byte and output
    // ip = filter(1)
    IpFilter::IpPoolStr ipFirst = ipFilter.Filter(1);
    //        std::cout << "filter(1) ==================================="
    //                  << std::endl;
    ipFilter.PrintIp(ipFirst);

    // 1.231.69.33
    // 1.87.203.225
    // 1.70.44.170
    // 1.29.168.152
    // 1.1.234.8

    // TODO filter by first and second bytes and output
    // ip = filter(46, 70)
    IpFilter::IpPoolStr ipSecond = ipFilter.Filter(46, 70);
    //        std::cout << "filter(46, 70)
    //        ==================================="
    //                  << std::endl;
    ipFilter.PrintIp(ipSecond);

    // 46.70.225.39
    // 46.70.147.26
    // 46.70.113.73
    // 46.70.29.76

    // TODO filter by any byte and output
    // ip = filter_any(46)
    IpFilter::IpPoolStr ipAny = ipFilter.FilterAny(46);
    //        std::cout << "filter_any(46) " << std::endl;
    ipFilter.PrintIp(ipAny);

    // 186.204.34.46
    // 186.46.222.194
    // 185.46.87.231
    // 185.46.86.132
    // 185.46.86.131
    // 185.46.86.131
    // 185.46.86.22
    // 185.46.85.204
    // 185.46.85.78
    // 68.46.218.208
    // 46.251.197.23
    // 46.223.254.56
    // 46.223.254.56
    // 46.182.19.219
    // 46.161.63.66
    // 46.161.61.51
    // 46.161.60.92
    // 46.161.60.35
    // 46.161.58.202
    // 46.161.56.241
    // 46.161.56.203
    // 46.161.56.174
    // 46.161.56.106
    // 46.161.56.106
    // 46.101.163.119
    // 46.101.127.145
    // 46.70.225.39
    // 46.70.147.26
    // 46.70.113.73
    // 46.70.29.76
    // 46.55.46.98
    // 46.49.43.85
    // 39.46.86.85
    // 5.189.203.46
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
