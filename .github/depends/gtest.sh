#!/bin/sh

usage()
{
  cat <<EOL
  -b   - 32-bit or 64-bit library, maybe 32, 64 or both
EOL
}

bit="64"

while getopts "b:" c; do
  case "$c" in
    b)
      bit="$OPTARG"
      [ "$bit" != "32" ] && [ "$bit" != "64" ] && [ "$bit" != "both" ] && usage && exit 1
      ;;
    ?*)
      echo "invalid arguments." && exit 1
      ;;
  esac
done

[ -z "$CXX" ] && CXX=g++

wget https://github.com/google/googletest/archive/release-1.7.0.zip -O googletest-release-1.7.0.zip
unzip -q googletest-release-1.7.0.zip
cd googletest-release-1.7.0
$CXX -m$bit src/gtest-all.cc -I. -Iinclude -c -fPIC
$CXX -m$bit src/gtest_main.cc -I. -Iinclude -c -fPIC
ar -rv libgtest.a gtest-all.o
ar -rv libgtest_main.a gtest_main.o
cp -r include/gtest /usr/local/include
mv *.a /usr/local/lib
