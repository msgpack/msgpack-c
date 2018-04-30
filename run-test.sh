#!/bin/bash

if [ -d build ]; then
    rm -rf build
fi

docker build -t t1 .

echo 'Run this'
echo 'CMAKE_CXX_COMPILER="${CXX}" CMAKE_C_COMPILER="${CC}" CMAKE_LIBRARY_PATH="${BASE}/usr/lib:${BASE}/build" GTEST_ROOT="${BASE}/usr" BOOST_ROOT="${BASE}/usr" CFLAGS="-Werror -g -fsanitize=address" CXXFLAGS="-Werror -g -fsanitize=address" ci/build_cmake.sh'

docker run -v $(pwd):$(pwd) -w $(pwd) -it t1 bash
