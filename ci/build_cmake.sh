#!/bin/bash

mkdir build

ret=$?
if [ $ret -ne 0 ]
then
    exit $ret
fi

cd build

ret=$?
if [ $ret -ne 0 ]
then
    exit $ret
fi

if [ $1 = "cpp11" ]
then
    if [ $2 = "32" ]
    then
        cmake -DMSGPACK_CXX11=ON -DMSGPACK_32BIT=ON ..
    else
        cmake -DMSGPACK_CXX11=ON ..
    fi
else
    if [ $2 = "32" ]
    then
        cmake -DMSGPACK_32BIT=ON ..
    else
        cmake ..
    fi
fi

ret=$?
if [ $ret -ne 0 ]
then
    exit $ret
fi

make

ret=$?
if [ $ret -ne 0 ]
then
    exit $ret
fi

make test

ret=$?
if [ $ret -ne 0 ]
then
    exit $ret
fi

make install DESTDIR=`pwd`/install

ret=$?
if [ $ret -ne 0 ]
then
    exit $ret
fi

if [ $2 != "32" ]
then
    ctest -T memcheck | tee memcheck.log

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

exit 0
