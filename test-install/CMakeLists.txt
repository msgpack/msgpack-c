cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(test-install LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(msgpack REQUIRED)

add_executable(test-install simple.cpp)
target_link_libraries(test-install PRIVATE msgpackc-cxx)
