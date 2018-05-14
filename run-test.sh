#!/bin/bash

set -e
set +x

if [ -d build ]; then
    rm -rf build
fi

docker build -t san .

echo 'ASAN_SYMOLIZER_PATH=/usr/lib/llvm-5.0/bin/llvm-symbolizer CMAKE_CXX_COMPILER="${CXX}" CMAKE_C_COMPILER="${CC}" CMAKE_LIBRARY_PATH="${BASE}" GTEST_ROOT="${BASE}/gtest" BOOST_ROOT="${BASE}/boost" CTEST_OUTPUT_ON_FAILURE=1 MSGPACK_SAN="ASAN" ci/build_regression.sh'

docker run --cap-add SYS_PTRACE --rm -v $(pwd):/src/msgpack-c -w /src/msgpack-c -it san bash
