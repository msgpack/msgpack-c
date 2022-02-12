#!/bin/bash

find include -name "*.hpp" -or -name "*.h" | sed -e 's/\s\+/\n/g' | LC_ALL=C sort > cpp_headers.tmp

echo 'SET (msgpackc_HEADERS' > Files.cmake
cat cpp_headers.tmp | sed -e 's/^/    /g' >> Files.cmake
echo ')' >> Files.cmake

rm -f cpp_headers.tmp
