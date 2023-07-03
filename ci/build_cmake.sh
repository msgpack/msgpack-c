#!/bin/bash

build_dir="$CXX-build"
prefix_dir="`pwd`/$CXX-prefix"
mkdir $build_dir  || exit 1
mkdir $prefix_dir || exit 1

if [ "${ARCH}" == "32" ]; then
    export BIT32="ON"
    export ARCH_FLAG="-m32"
else
    export BIT32="OFF"
    export ARCH_FLAG="-m64"
fi

cmake \
    -D CMAKE_PREFIX_PATH="${HOME}/boost-prefix/${ARCH};${HOME}/zlib-prefix/${ARCH}" \
    -D MSGPACK_BUILD_TESTS=ON \
    -D ${MSGPACK_CXX_VERSION} \
    -D MSGPACK_32BIT=${BIT32} \
    -D MSGPACK_CHAR_SIGN=${CHAR_SIGN} \
    -D MSGPACK_DEFAULT_API_VERSION=${API_VERSION} \
    -D MSGPACK_USE_X3_PARSE=${X3_PARSE} \
    -D MSGPACK_USE_STD_VARIANT_ADAPTOR=${STD_VARIANT_ADAPTOR} \
    -D CMAKE_CXX_FLAGS="${CXXFLAGS} ${ARCH_FLAG}" \
    -D CMAKE_INSTALL_PREFIX=$prefix_dir \
    -B $build_dir \
    -S . || exit 1

cmake --build $build_dir --target install || exit 1

ctest -VV --test-dir $build_dir || exit 1

if [ "${ARCH}" != "32" ] && [ `uname` = "Linux" ]; then
    ctest -T memcheck --test-dir $build_dir | tee memcheck.log

    ret=${PIPESTATUS[0]}
    if [ $ret -ne 0 ]
    then
        exit $ret
    fi
    cat memcheck.log | grep "Memory Leak" > /dev/null
    ret=$?
    if [ $ret -eq 0 ]
    then
        exit 1
    fi
fi

if [ "${ARCH}" != "32" ]; then
    cd test-install || exit 1

    mkdir $build_dir
    cmake \
        -D CMAKE_PREFIX_PATH="$prefix_dir;${HOME}/boost-prefix/${ARCH}" \
        -B $build_dir \
        -S . || exit 1
    cmake --build $build_dir --target all || exit 1

    $build_dir/test-install || exit 1
fi
