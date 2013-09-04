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

	// deserialized object is valid during the msgpack::zone instance alive.
	msgpack::zone mempool;
	msgpack::object deserialized;
	std::cout << "Start unpacking..." << std::endl;
	{
		boost::timer::cpu_timer timer;
		msgpack::unpack(str.data(), str.size(), NULL, mempool, deserialized);
		std::string result = timer.format();
		std::cout << result << std::endl;
	}
	std::cout << "Unpack finished..." << std::endl;

	msgpack::unpacked unpacked;
	std::cout << "Start unpacking...by void unpack(unpacked* result, const char* data, size_t len, size_t* offset = NULL)" << std::endl;
	{
		boost::timer::cpu_timer timer;
		msgpack::unpack(unpacked, str.data(), str.size());
		std::string result = timer.format();
		std::cout << result << std::endl;
	}
	std::cout << "Unpack finished..." << std::endl;
}



int main(void)
{
	test_map_pack_unpack();
}

