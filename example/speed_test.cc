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

	// deserialized object is valid during the msgpack::zone instance alive.
	msgpack::zone mempool;
	msgpack::object deserialized;
	std::cout << "Start unpacking..." << std::endl;
	{
		boost::timer::cpu_timer timer;
		msgpack::unpack(str.data(), str.size(), NULL, &mempool, &deserialized);
		std::string result = timer.format();
		std::cout << result << std::endl;
	}
	std::cout << "Unpack finished..." << std::endl;

	msgpack::unpacked unpacked;
	std::cout << "Start unpacking...by void unpack(unpacked* result, const char* data, size_t len, size_t* offset = NULL)" << std::endl;
	{
		boost::timer::cpu_timer timer;
		msgpack::unpack(&unpacked, str.data(), str.size());
		std::string result = timer.format();
		std::cout << result << std::endl;
	}
	std::cout << "Unpack finished..." << std::endl;
	std::map<int, int> m2;
	std::cout << "Start converting..." << std::endl;
	{
		boost::timer::cpu_timer timer;
		deserialized.convert(&m2);
		std::string result = timer.format();
		std::cout << result << std::endl;
	}
	std::cout << "Convert finished..." << std::endl;
}

template <typename T, std::size_t level>
struct vecvec {
	typedef std::vector<typename vecvec<T, level - 1>::type> type;
	static void fill(type& v, std::size_t num_of_elems, T const& val) {
		for (int elem = 0; elem < num_of_elems; ++elem) {
			typename vecvec<T, level - 1>::type child;
			vecvec<T, level - 1>::fill(child, num_of_elems, val);
			v.push_back(child);
		}
	}
};

template <typename T>
struct vecvec<T, 0> {
	typedef std::vector<T> type;
	static void fill(type& v, std::size_t num_of_elems, T const& val) {
		for (int elem = 0; elem < num_of_elems; ++elem) {
			v.push_back(val);
		}
	}
};

void test_array_of_array() {
	std::cout << "[TEST][array_of_array]" << std::endl;
	// setup
	int const depth = 16;
	std::cout << "Setting up array data..." << std::endl;
	typename vecvec<int, depth>::type v1;
	vecvec<int, depth>::fill(v1, 3, 42);

	std::cout << "Start packing..." << std::endl;
	std::stringstream buffer;
	{
		boost::timer::cpu_timer timer;
		msgpack::pack(buffer, v1);
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
		msgpack::unpack(str.data(), str.size(), NULL, &mempool, &deserialized);
		std::string result = timer.format();
		std::cout << result << std::endl;
	}
	std::cout << "Unpack finished..." << std::endl;

	msgpack::unpacked unpacked;
	std::cout << "Start unpacking...by void unpack(unpacked* result, const char* data, size_t len, size_t* offset = NULL)" << std::endl;
	{
		boost::timer::cpu_timer timer;
		msgpack::unpack(&unpacked, str.data(), str.size());
		std::string result = timer.format();
		std::cout << result << std::endl;
	}
	std::cout << "Unpack finished..." << std::endl;
	typename vecvec<int, depth>::type v2;
	std::cout << "Start converting..." << std::endl;
	{
		boost::timer::cpu_timer timer;
		deserialized.convert(&v2);
		std::string result = timer.format();
		std::cout << result << std::endl;
	}
	std::cout << "Convert finished..." << std::endl;
}



int main(void)
{
	test_map_pack_unpack();
	test_array_of_array();
}

