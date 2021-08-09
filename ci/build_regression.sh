#!/bin/bash

build_dir="$CXX-build"
mkdir $build_dir || exit 1

if [ "${ARCH}" == "32" ]
then
    echo "64 bit support required for regressions"
    exit 1
fi

cmake \
    -D CMAKE_PREFIX_PATH="${HOME}/boost-prefix/${ARCH};${HOME}/zlib-prefix/${ARCH}" \
    -D MSGPACK_FUZZ_REGRESSION="ON" \
    -D ${MSGPACK_CXX_VERSION} \
    -D MSGPACK_CHAR_SIGN=${CHAR_SIGN} \
    -D MSGPACK_DEFAULT_API_VERSION=${API_VERSION} \
    -D MSGPACK_USE_X3_PARSE=${X3_PARSE} \
    -D CMAKE_CXX_FLAGS="${CXXFLAGS}" \
    -B $build_dir \
    -S . || exit 1

cmake --build $build_dir --target all || exit 1

ctest -VV --test-dir $build_dir || exit 1
