#include <msgpack.hpp>
#include <cstddef>
#include <cstdint>
// A standalone visitor: does NOT derive from msgpack::null_visitor.
struct standalone_visitor {
    bool visit_nil(){return true;} bool visit_boolean(bool){return true;}
    bool visit_positive_integer(uint64_t){return true;} bool visit_negative_integer(int64_t){return true;}
    bool visit_float32(float){return true;} bool visit_float64(double){return true;}
    bool visit_str(const char*,uint32_t){return true;} bool visit_bin(const char*,uint32_t){return true;}
    bool visit_ext(const char*, std::size_t){return true;}          // implements visit_ext, no null_visitor base
    bool start_array(uint32_t){return true;} bool start_array_item(){return true;}
    bool end_array_item(){return true;} bool end_array(){return true;}
    bool start_map(uint32_t){return true;} bool start_map_key(){return true;}
    bool end_map_key(){return true;} bool start_map_value(){return true;}
    bool end_map_value(){return true;} bool end_map(){return true;}
    void parse_error(std::size_t,std::size_t){} void insufficient_bytes(std::size_t,std::size_t){}
};
int main(){
    msgpack::sbuffer sb; char d[]={9}; msgpack::packer<msgpack::sbuffer> pk(&sb);
    pk.pack_ext(1,7); pk.pack_ext_body(d,1);
    standalone_visitor v; std::size_t off=0;
    return msgpack::v2::parse(sb.data(), sb.size(), off, v) ? 0 : 1;
}
