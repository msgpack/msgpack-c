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
  cmake -DMSGPACK_CXX11=ON ..
else
  cmake  ..
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

exit 0
