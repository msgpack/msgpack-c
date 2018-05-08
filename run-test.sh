#!/bin/bash

set -e
set +x

if [ -d build ]; then
    rm -rf build
fi

docker build -t san .

echo 'CTEST_OUTPUT_ON_FAILURE=1 MSGPACK_SAN="ASAN" CXX11="ON" BOOST="ON" SHARED="ON" CHAR_SIGN="unsigned" API_VERSION="3" X3_PARSE="ON" CMAKE_CXX_COMPILER="${CXX}" CMAKE_C_COMPILER="${CC}" CMAKE_LIBRARY_PATH="${BASE}/boost:${BASE}" GTEST_ROOT="${BASE}/gtest" BOOST_ROOT="${BASE}/boost" CFLAGS="-Werror -g" CXXFLAGS="-Werror -g" ci/build_cmake.sh'
docker run --cap-add SYS_PTRACE --rm -v $(pwd):/src/msgpack-c -w /src/msgpack-c -it san bash
