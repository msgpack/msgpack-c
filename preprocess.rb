require 'erb'

files = {
    "erb/v1/cpp03_msgpack_tuple_decl.hpp" => "include/msgpack/v1/adaptor/detail/cpp03_msgpack_tuple_decl.hpp",
    "erb/v1/cpp03_msgpack_tuple.hpp" => "include/msgpack/v1/adaptor/detail/cpp03_msgpack_tuple.hpp",
    "erb/v1/cpp03_define_array_decl.hpp" => "include/msgpack/v1/adaptor/detail/cpp03_define_array_decl.hpp",
    "erb/v1/cpp03_define_array.hpp" => "include/msgpack/v1/adaptor/detail/cpp03_define_array.hpp",
    "erb/v1/cpp03_define_map_decl.hpp" => "include/msgpack/v1/adaptor/detail/cpp03_define_map_decl.hpp",
    "erb/v1/cpp03_define_map.hpp" => "include/msgpack/v1/adaptor/detail/cpp03_define_map.hpp",
    "erb/v1/cpp03_zone_decl.hpp" => "include/msgpack/v1/detail/cpp03_zone_decl.hpp",
    "erb/v1/cpp03_zone.hpp" => "include/msgpack/v1/detail/cpp03_zone.hpp"
}

files.map { |erb, hpp|
    res = ERB.new(File.open(erb+".erb").read).result
    File.write(hpp, res)
}