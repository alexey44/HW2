#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <fstream>
#include <map>
#include <algorithm>

static const int32_t ipSize = 4;
using IpPoolStr = std::vector<std::vector<std::string> >;
using IpTuple = std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>;
using IpPoolTuple = std::vector<IpTuple>;

std::vector<IpTuple> sortIpPool;

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

void printIp(IpPoolStr & ip_pool) {
    for(IpPoolStr::const_iterator ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
    {
        for(std::vector<std::string>::const_iterator ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
        {
            if (ip_part != ip->cbegin())
            {
                std::cout << ".";

            }
            std::cout << *ip_part;
        }
        std::cout << std::endl;
    }
}

void IpSort(IpPoolStr &ipPoolStr) {
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

    printIp(ipPoolStr);

    return;
}

IpPoolStr Filter(int16_t firstByte, int16_t secondByte = -1) {
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
                auto vv =
                    std::vector<std::string>{std::to_string(std::get<0>(ip)),
                                             std::to_string(std::get<1>(ip)),
                                             std::to_string(std::get<2>(ip)),
                                             std::to_string(std::get<3>(ip))};
                filteredPool.emplace_back(vv);
            }
        }
    }
    return filteredPool;
}

IpPoolStr FilterAny(uint8_t val) {
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
            auto vv = std::vector<std::string>{std::to_string(std::get<0>(ip)),
                std::to_string(std::get<1>(ip)),
                std::to_string(std::get<2>(ip)),
                std::to_string(std::get<3>(ip))
            };
            filteredPool.emplace_back(vv);
        }
    }
    return filteredPool;
}

// int main(int argc, char const *argv[])
int main() {
    try {
        //        int32_t maxReadNum = 80;
        int32_t maxReadNum = std::numeric_limits<int32_t>::max();
        int32_t readCnt = 0;

        std::string filename = "ip_filter.tsv";
        std::ifstream inFileStream;
        inFileStream.open(filename);

        IpPoolStr ipPoolStr;
        for(std::string line; std::getline(inFileStream, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            ipPoolStr.push_back(split(v.at(0), '.'));
            if (++readCnt > maxReadNum) {
                break;
            }
        }
        inFileStream.close();

        printIp(ipPoolStr);

        // TODO reverse lexicographically sort
        IpSort(ipPoolStr);
        std::cout << "ipSort ===================================" << std::endl;
        printIp(ipPoolStr);

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        // ip = filter(1)
        IpPoolStr ipFirst = Filter(1);
        std::cout << "filter(1) ==================================="
                  << std::endl;
        printIp(ipFirst);

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)
        IpPoolStr ipSecond = Filter(46, 70);
        std::cout << "filter(46, 70) ==================================="
                  << std::endl;
        printIp(ipSecond);

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)
        IpPoolStr ipAny = FilterAny(46);
        std::cout << "filter_any(46) ==================================="
                  << std::endl;
        printIp(ipAny);

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
