#!/bin/bash

usage()
{
  cat <<EOL
  -b   - 32-bit or 64-bit library, maybe 32, 64 or both
  -t   - the toolset, maybe gcc, clang or both
  -p   - installation prefix
EOL
}

build_boost()
{
  ./b2 \
      --toolset=$1 \
      --prefix=$3/$2 \
      --with-test \
      --with-headers \
      --with-chrono \
      --with-context \
      --with-filesystem \
      --with-system \
      --with-timer \
      address-model=$2 \
      install || exit 1
}

bit="64"
toolset="gcc"
prefix="$HOME/boost-prefix"

while getopts "b:t:p:" c; do
  case "$c" in
    b)
      bit="$OPTARG"
      [ "$bit" != "32" ] && [ "$bit" != "64" ] && [ "$bit" != "both" ] && usage && exit 1
      ;;
    t)
      toolset="$OPTARG"
      [ "$toolset" != "gcc" ] && [ "$toolset" != "clang" ] && [ "$toolset" != "both" ] && usage && exit 1
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
wget https://boostorg.jfrog.io/artifactory/main/release/1.76.0/source/boost_1_76_0.tar.bz2 || exit 1
tar xf boost_1_76_0.tar.bz2 || exit 1
cd boost_1_76_0
./bootstrap.sh || exit 1

build()
{
  if [ "$bit" = "both" ]; then
    build_boost $1 32 $prefix
    build_boost $1 64 $prefix
  else
    build_boost $1 $bit $prefix
  fi
}

if [ "$toolset" = "both" ]; then
  build gcc
  build clang
else
  build $toolset
fi
