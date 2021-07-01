#!/bin/bash

mkdir $CXX-build  || exit 1
mkdir $CXX-prefix || exit 1

if [ "${ARCH}" == "32" ]
then
    export BIT32="ON"
    export ARCH_FLAG="-m32"
else
    export BIT32="OFF"
    export ARCH_FLAG="-m64"
fi

cmake \
    -D CMAKE_PREFIX_PATH="$HOME/boost-prefix;$HOME/zlib-prefix" \
    -D MSGPACK_BUILD_TESTS=ON \
    -D ${MSGPACK_CXX_VERSION} \
    -D MSGPACK_32BIT=${BIT32} \
    -D MSGPACK_CHAR_SIGN=${CHAR_SIGN} \
    -D MSGPACK_DEFAULT_API_VERSION=${API_VERSION} \
    -D MSGPACK_USE_X3_PARSE=${X3_PARSE} \
    -D CMAKE_CXX_FLAGS="${CXXFLAGS} ${ARCH_FLAG}" \
    -D CMAKE_INSTALL_PREFIX=`pwd`/$CXX-prefix \
    -B $CXX-build \
    -S . || exit 1

cmake --build $CXX-build --target install || exit 1

ctest -VV --test-dir $CXX-build || exit 1

if [ "${ARCH}" != "32" ] && [ `uname` = "Linux" ]
then
    ctest -T memcheck --test-dir $CXX-build | tee memcheck.log

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

if [ "${ARCH}" != "32" ]
then
    cd test-install || exit 1

    mkdir $CXX-build
    cmake \
        -D CMAKE_PREFIX_PATH="`pwd`/../$CXX-prefix;$HOME/boost-prefix" \
        -B $CXX-build \
        -S . || exit 1
    cmake --build $CXX-build --target all || exit 1

    $CXX-build/test-install || exit 1
fi
