// g++ -std=c++11 -O3 -g -Ipath_to_msgpack_src -Ipath_to_boost speed_test.cc -Lpath_to_boost_lib -lboost_timer -lboost_system
// export LD_LIBRARY_PATH=path_to_boost_lib

#include <msgpack.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <boost/timer/timer.hpp>

void test_map_pack_unpack() {
    std::cout << "[TEST][map_pack_unpack]" << std::endl;
    // setup
    std::cout << "Setting up map data..." << std::endl;
    std::map<int, int> m1;
    int const num = 30000000L;
    for (int i = 0; i < num; ++i) m1[i] = i;
    std::cout << "Start packing..." << std::endl;
    std::stringstream buffer;
    {
        boost::timer::cpu_timer timer;
        msgpack::pack(buffer, m1);
        std::string result = timer.format();
        std::cout << result << std::endl;
    }
    std::cout << "Pack finished..." << std::endl;

    buffer.seekg(0);
    std::string str(buffer.str());

    msgpack::unpacked unpacked;
    std::cout << "Start unpacking...by void unpack(unpacked& result, const char* data, size_t len)" << std::endl;
    {
        boost::timer::cpu_timer timer;
        msgpack::unpack(unpacked, str.data(), str.size());
        std::string result = timer.format();
        std::cout << result << std::endl;
    }
    std::cout << "Unpack finished..." << std::endl;
    std::map<int, int> m2;
    std::cout << "Start converting..." << std::endl;
    {
        boost::timer::cpu_timer timer;
        unpacked.get().convert(&m2);
        std::string result = timer.format();
        std::cout << result << std::endl;
    }
    std::cout << "Convert finished..." << std::endl;
}

int main(void)
{
    test_map_pack_unpack();
}

