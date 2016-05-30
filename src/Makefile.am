# For C++
#
# The C++ parts of msgpack-c is a header only library,
# so there is no libraries.

# For C

lib_LTLIBRARIES = libmsgpackc.la

#AM_CPPFLAGS = -I../include
AM_CFLAGS = -I../include

include files.mk

# -version-info CURRENT:REVISION:AGE
libmsgpackc_la_LDFLAGS = -version-info 2:0:0 -no-undefined

EXTRA_DIST = \
	../erb/v1/cpp03_define_array.hpp.erb \
	../erb/v1/cpp03_define_array_decl.hpp.erb \
	../erb/v1/cpp03_define_map.hpp.erb \
	../erb/v1/cpp03_define_map_decl.hpp.erb \
	../erb/v1/cpp03_msgpack_tuple.hpp.erb \
	../erb/v1/cpp03_msgpack_tuple_decl.hpp.erb \
	../erb/v1/cpp03_zone.hpp.erb \
	../erb/v1/cpp03_zone_decl.hpp.erb

doxygen_c:
	cat ../Doxyfile > Doxyfile_c
	echo "FILE_PATTERNS      = *.h" >> Doxyfile_c
	echo "OUTPUT_DIRECTORY   = doc_c" >> Doxyfile_c
	echo "INPUT              = ../include" >> Doxyfile_c
	echo "EXTRACT_ALL        = YES" >> Doxyfile_c
	echo "PROJECT_NAME       = \"MessagePack for C\"" >> Doxyfile_c
	doxygen Doxyfile_c

doxygen_cpp:
	cat ../Doxyfile > Doxyfile_cpp
	echo "FILE_PATTERNS      = *.hpp" >> Doxyfile_cpp
	echo "OUTPUT_DIRECTORY   = doc_cpp" >> Doxyfile_cpp
	echo "INPUT              = ../include" >> Doxyfile_cpp
	echo "EXTRACT_ALL        = YES" >> Doxyfile_cpp
	echo "PROJECT_NAME       = \"MessagePack for C++\"" >> Doxyfile_cpp
	doxygen Doxyfile_cpp

doxygen: doxygen_c doxygen_cpp
