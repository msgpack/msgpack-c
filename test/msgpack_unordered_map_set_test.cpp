#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

#include <msgpack.hpp>

template <class T>
void local_pack(const T & val, msgpack::sbuffer & sbuf) { msgpack::pack(&sbuf, val); }

template <class T>
void local_unpack(const msgpack::sbuffer & sbuf, T & r) {
  msgpack::unpacked msg;
  msgpack::unpack(&msg, sbuf.data(), sbuf.size());
  auto obj = msg.get();
  obj.convert(&r);
}

int main(int argc, char *argv[])
{
  { // test for std::unordered_map
    std::unordered_map<std::string, std::vector<double> > d;
    std::vector<double> target1 = {1.1, 2.2, 3.3}; std::vector<double> target2 = {3.3, 2.2, 1.1};
    d["key_0"] = target1; d["key_1"] = target2;
    msgpack::sbuffer sbuf;
    local_pack(d, sbuf);
    std::unordered_map<std::string, std::vector<double> > result; 
    local_unpack(sbuf, result);
    for(auto & v : result) {
      for(auto & va : v.second) {
	std::cout << va << std::endl;
      }   
    } 
  } 
  { // test for std::unordered_set
    std::unordered_set<std::string> d({"red","green","blue"});
    msgpack::sbuffer sbuf;
    local_pack(d, sbuf);
    std::unordered_set<std::string> result;
    local_unpack(sbuf, result);
    for(auto & v : result) {
      std::cout << v << std::endl;
    }   
  }
  return 0;
}
