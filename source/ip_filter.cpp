#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <fstream>
#include <map>
#include <algorithm>

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

void printIp(std::vector<std::vector<std::string> > & ip_pool) {
    for(std::vector<std::vector<std::string> >::const_iterator ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
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

void ipSort(std::vector<std::vector<std::string> > & ipPool) {

    std::vector < std::tuple<int, int, int, int> > vect;
    vect.reserve(ipPool.size());

    for (const auto & line : ipPool) {
//        auto val = std::pair<int32_t, int32_t>(std::stoi(line.at(col)), pos++);
        vect.emplace_back( std::make_tuple( std::stoi(line.at(0)),
                                            std::stoi(line.at(1)),
                                            std::stoi(line.at(2)),
                                            std::stoi(line.at(3)) ) );
    }

//    std::sort(vect.begin(), vect.end());

    sort(vect.begin(), vect.end(), [](const std::tuple<int, int, int, int>& a,
                                      const std::tuple<int, int, int, int>& b)
         {return a > b; });

    for (const auto& p: vect)
        std::cout << "{ " << std::get<0>(p)
                  << ", " << std::get<1>(p)
                  << ", " << std::get<2>(p)
                  << ", " << std::get<3>(p)
                  << " }\n";


//    printIp(ipPoolSort);

    return;

/*
    int32_t numLines = ipPool.size();
    int32_t numCols = ipPool.at(0).size();

    int32_t pos = 0;
    std::map<int32_t, int32_t> ipval_pos;
    std::vector<std::vector<std::string> > ipPoolSort;
    ipPoolSort.reserve(ipPool.size());

    for (int32_t col = 0; col < numCols; ++col) {




        ipval_pos.clear();
        for (const auto & line : ipPool) {
            //        ipval_pos.insert({std::stoi(line.at(0)), pos++});
            auto val = std::pair<int32_t, int32_t>(std::stoi(line.at(col)), pos++);
            ipval_pos.insert(val);
//            std::cout << "ipval_pos = [" << val.first << ", \t"<< val.second << "]" << std::endl;
        }
//        std::cout << "=========" << std::endl;
//        for (const auto & val : ipval_pos) {
//            std::cout << "ipval_pos = [" << val.first << ", \t"<< val.second << "]"<< std::endl;
//        }
//        std::cout << "=========" << std::endl;

        ipPoolSort.clear();

        for (const auto & val : ipval_pos) {
            auto idx = val.second;
//            std::cout << "ipval_pos = [" << idx << "]"<< std::endl;
            ipPoolSort.emplace_back(std::move(  ipPool.at(  idx )  ));
        }

        ipPool = std::move(ipPoolSort);

    }

    std::cout << "HW2 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
//    printIp(ipPoolSort);
*/

}



 int main(int argc, char const *argv[])
//int main()
{
//    std::cout << "HW2 jfjfjfjfj" << std::endl;
//    return 0;
    try
    {
        int32_t maxReadNum = 80;
        int32_t readCnt = 0;


        std::string filename = "ip_filter.tsv";
        std::ifstream inFileStream;
        inFileStream.open(filename);

        std::vector<std::vector<std::string> > ip_pool;

        for(std::string line; std::getline(inFileStream, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.push_back(split(v.at(0), '.'));

            if (++readCnt > maxReadNum) {
                break;
            }
        }
        inFileStream.close();

        printIp(ip_pool);

        // TODO reverse lexicographically sort
        ipSort(ip_pool);




        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        // ip = filter(1)

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)

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
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
