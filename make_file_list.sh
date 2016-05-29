#!/bin/sh

find src -name "*.c" | sed -e 's/\s\+/\n/g' | sort > srcs.tmp
find include -name "*.h" | sed -e 's/\s\+/\n/g' | sort > c_headers.tmp
find include -name "*.hpp" | sed -e 's/\s\+/\n/g' | sort > cpp_headers.tmp

echo 'LIST (APPEND msgpackc_SOURCES' > Files.cmake
cat srcs.tmp | sed -e 's/^/    /g' >> Files.cmake
echo ')' >> Files.cmake

echo 'LIST (APPEND msgpackc_HEADERS' >> Files.cmake
cat c_headers.tmp | sed -e 's/^/    /g' >> Files.cmake
echo ')' >> Files.cmake

echo 'IF (MSGPACK_ENABLE_CXX)' >> Files.cmake
echo '    LIST (APPEND msgpack_HEADERS' >> Files.cmake
cat cpp_headers.tmp | sed -e 's/^/        /g' >> Files.cmake
echo '    )' >> Files.cmake
echo 'ENDIF ()' >> Files.cmake

echo 'libmsgpackc_la_SOURCES = \' > src/files.mk
cat srcs.tmp | sed -e 's/^src\//\t/g' | sed -e 's/$/ \\/g' | sed '$ s/.$//' >> src/files.mk
echo 'nobase_include_HEADERS = \' >> src/files.mk
cat c_headers.tmp | sed -e 's/^/\t..\//g' | sed -e 's/$/ \\/g' | sed '$ s/.$//' >> src/files.mk
echo 'if ENABLE_CXX' >> src/files.mk
echo 'nobase_include_HEADERS += \' >> src/files.mk
cat cpp_headers.tmp | sed -e 's/^/\t..\//g' | sed -e 's/$/ \\/g' | sed '$ s/.$//' >> src/files.mk
echo 'endif' >> src/files.mk

rm -f srcs.tmp c_headers.tmp cpp_headers.tmp
