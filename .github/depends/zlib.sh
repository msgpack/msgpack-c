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
      [ "$bit" != "32" ] && [ "$bit" != "64" ] && usage && exit 1
      ;;
    p)
      prefix="$OPTARG"
      ;;
    ?*)
      echo "invalid arguments." && exit 1
      ;;
  esac
done

wget https://zlib.net/zlib-1.2.11.tar.gz || exit 1
tar -xf zlib-1.2.11.tar.gz || exit 1
cd zlib-1.2.11

mkdir $prefix
cmake \
    -D CMAKE_BUILD_TYPE=Release \
    -D CMAKE_INSTALL_PREFIX=$prefix \
    -D CMAKE_C_FLAGS="-m${bit}" \
    -D CMAKE_SHARED_LINKER_FLAGS="-m${bit}" \
    -S .
cmake --build . --target install
