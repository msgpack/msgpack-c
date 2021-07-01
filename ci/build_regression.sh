#!/bin/bash

mkdir build || exit 1

if [ "${ARCH}" == "32" ]
then
    echo "64 bit support required for regressions"
    exit 1
fi

cmake \
    -D CMAKE_PREFIX_PATH="$HOME/boost-prefix;$HOME/zlib-prefix" \
    -D MSGPACK_FUZZ_REGRESSION="ON" \
    -D ${MSGPACK_CXX_VERSION} \
    -D MSGPACK_CHAR_SIGN=${CHAR_SIGN} \
    -D MSGPACK_DEFAULT_API_VERSION=${API_VERSION} \
    -D MSGPACK_USE_X3_PARSE=${X3_PARSE} \
    -D CMAKE_CXX_FLAGS="${CXXFLAGS}" \
    -B build \
    -S . || exit 1

cmake --build . --target all || exit 1

ctest -VV --test-dir build || exit 1
