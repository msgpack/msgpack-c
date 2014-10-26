#!/bin/sh

./bootstrap
ret=$?
if [ $ret -ne 0 ]
then
    exit $ret
fi

if [ $1 = "cpp11" ]
then
    if [ $2 = "32" ]
    then
        ./configure CFLAGS="-m32" CXXFLAGS="-std=c++11 -m32"
    else
        ./configure CXXFLAGS="-std=c++11"
    fi
else
    if [ $2 = "32" ]
    then
        ./configure CFLAGS="-m32" CXXFLAGS="-m32"
    else
        ./configure
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

make check

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

exit 0
