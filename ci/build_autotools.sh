#!/bin/sh

./bootstrap
ret=$?
if [ $ret -ne 0 ]
then
    exit $ret
fi

if [ $1 = "cpp11" ]
then
    ./configure CXXFLAGS="-std=c++11"
else
    ./configure
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

make check

ret=$?
if [ $ret -ne 0 ]
then
    exit $ret
fi

exit 0
