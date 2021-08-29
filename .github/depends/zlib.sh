#!/bin/bash

usage()
{
  cat <<EOL
  -b   - 32-bit or 64-bit library, maybe 32 or 64
  -p   - installation prefix
EOL
}

bit="64"
prefix="$HOME/zlib-prefix"

while getopts "b:t:p:" c; do
  case "$c" in
    b)
      bit="$OPTARG"
      [ "$bit" != "32" ] && [ "$bit" != "64" ] && [ "$bit" != "both" ] && usage && exit 1
      ;;
    p)
      prefix="$OPTARG"
      ;;
    ?*)
      echo "invalid arguments." && exit 1
      ;;
  esac
done

mkdir $prefix || exit 1
wget https://zlib.net/zlib-1.2.11.tar.gz || exit 1
tar -xf zlib-1.2.11.tar.gz || exit 1
cd zlib-1.2.11

build()
{
  cmake \
    -D CMAKE_BUILD_TYPE=Release \
    -D CMAKE_INSTALL_PREFIX=$2/$1 \
    -D CMAKE_C_FLAGS="-m$1" \
    -D CMAKE_SHARED_LINKER_FLAGS="-m$1" \
    -B build$1 \
    -S .
  cmake --build build$1 --target install
}

if [ "$bit" = "both" ]; then
  build 32 $prefix
  build 64 $prefix
else
  build $bit $prefix
fi
