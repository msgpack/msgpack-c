CMAKE_MINIMUM_REQUIRED (VERSION 2.8.7)
PROJECT (msgpack)

SET (VERSION 0.5.7)
SET (VERSION_MAJOR 5)
SET (VERSION_MINOR 7)

FIND_PACKAGE (Ruby REQUIRED)

OPTION (MSGPACK_BUILD_TESTS "Build msgpack tests." ON)
OPTION (MSGPACK_ENABLE_CXX "Enable C++ interface." ON)

INCLUDE (CheckCXXSourceCompiles)
CHECK_CXX_SOURCE_COMPILES ("
#include <bits/atomicity.h>
int atomic_sub(int i) { return __gnu_cxx::__exchange_and_add(&i, -1) - 1; }
int atomic_add(int i) { return __gnu_cxx::__exchange_and_add(&i, 1) + 1; }
int main(int argc, char * argv[])
{
	atomic_sub(1);
	atomic_add(1);
}
" MSGPACK_ENABLE_GCC_CXX_ATOMIC)

IF (MSGPACK_ENABLE_CXX)
	LIST (APPEND msgpack_SOURCES
		src/object.cpp
	)
ENDIF ()

IF (MSGPACK_ENABLE_GCC_CXX_ATOMIC)
	LIST (APPEND msgpack_SOURCES
		src/gcc_atomic.cpp
	)
ENDIF ()

LIST (APPEND msgpack_SOURCES
	src/unpack.c
	src/objectc.c
	src/version.c
	src/vrefbuffer.c
	src/zone.c
)

LIST (APPEND msgpack_HEADERS
	src/msgpack/pack_define.h
	src/msgpack/pack_template.h
	src/msgpack/unpack_define.h
	src/msgpack/unpack_template.h
	src/msgpack/sysdep.h
	src/msgpack.h
	src/msgpack/sbuffer.h
	src/msgpack/version.h
	src/msgpack/vrefbuffer.h
	src/msgpack/zbuffer.h
	src/msgpack/pack.h
	src/msgpack/unpack.h
	src/msgpack/object.h
	src/msgpack/zone.h
)

IF (MSGPACK_ENABLE_CXX)
	LIST (APPEND msgpack_HEADERS
		src/msgpack.hpp
		src/msgpack/sbuffer.hpp
		src/msgpack/vrefbuffer.hpp
		src/msgpack/zbuffer.hpp
		src/msgpack/pack.hpp
		src/msgpack/unpack.hpp
		src/msgpack/object.hpp
		src/msgpack/zone.hpp
		src/msgpack/type.hpp
		src/msgpack/type/bool.hpp
		src/msgpack/type/deque.hpp
		src/msgpack/type/float.hpp
		src/msgpack/type/fixint.hpp
		src/msgpack/type/int.hpp
		src/msgpack/type/list.hpp
		src/msgpack/type/map.hpp
		src/msgpack/type/nil.hpp
		src/msgpack/type/pair.hpp
		src/msgpack/type/raw.hpp
		src/msgpack/type/set.hpp
		src/msgpack/type/string.hpp
		src/msgpack/type/vector.hpp
		src/msgpack/type/tuple.hpp
		src/msgpack/type/define.hpp
		src/msgpack/type/tr1/unordered_map.hpp
		src/msgpack/type/tr1/unordered_set.hpp
	)
ENDIF ()

# Run preprocess
FOREACH (header_file type/tuple.hpp;type/define.hpp;zone.hpp)
	ADD_CUSTOM_COMMAND (
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/src/msgpack/${header_file}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/src/msgpack/type/
		COMMAND ${RUBY_EXECUTABLE} -r erb -e "puts ERB.new(ARGF.read).result" ${CMAKE_CURRENT_SOURCE_DIR}/src/msgpack/${header_file}.erb > ${CMAKE_CURRENT_BINARY_DIR}/src/msgpack/${header_file}
		VERBATIM
	)
ENDFOREACH ()

# Files to copy

MACRO (COPY_FILE SOURCE DESTINATION)
	# COPY_FILE (readme.txt dir1/dir2/) would add new target
	# that will copy readme.txt to dir1/dir2 where dir1 will be located
	# in current binary directory.
	ADD_CUSTOM_COMMAND (
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${DESTINATION}/${SOURCE}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${DESTINATION}
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE} ${CMAKE_CURRENT_BINARY_DIR}/${DESTINATION}/${SOURCE}
	)
ENDMACRO ()

COPY_FILE (sysdep.h src/msgpack/)
COPY_FILE (pack_define.h src/msgpack/)
COPY_FILE (pack_template.h src/msgpack/)
COPY_FILE (unpack_define.h src/msgpack/)
COPY_FILE (unpack_template.h src/msgpack/)
COPY_FILE (cases.mpac test/)
COPY_FILE (cases_compact.mpac test/)

# Trigger custom commands
LIST (APPEND msgpack_SOURCES
	${CMAKE_CURRENT_BINARY_DIR}/src/msgpack/type/tuple.hpp
	${CMAKE_CURRENT_BINARY_DIR}/src/msgpack/type/define.hpp
	${CMAKE_CURRENT_BINARY_DIR}/src/msgpack/zone.hpp
	${CMAKE_CURRENT_BINARY_DIR}/src/msgpack/sysdep.h
	${CMAKE_CURRENT_BINARY_DIR}/src/msgpack/pack_define.h
	${CMAKE_CURRENT_BINARY_DIR}/src/msgpack/pack_template.h
	${CMAKE_CURRENT_BINARY_DIR}/src/msgpack/unpack_define.h
	${CMAKE_CURRENT_BINARY_DIR}/src/msgpack/unpack_template.h
	${CMAKE_CURRENT_BINARY_DIR}/test/cases.mpac
	${CMAKE_CURRENT_BINARY_DIR}/test/cases_compact.mpac
)

EXECUTE_PROCESS (
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/src/msgpack
)

CONFIGURE_FILE (
	src/msgpack/version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/src/msgpack/version.h
)

INCLUDE_DIRECTORIES (
	./
	src/
	src/msgpack/
	${CMAKE_CURRENT_BINARY_DIR}/src/
	${CMAKE_CURRENT_BINARY_DIR}/src/msgpack/
)

ADD_LIBRARY (msgpack
	${msgpack_SOURCES}
	${msgpack_HEADERS}
)

IF (MSGPACK_BUILD_TESTS)
	ENABLE_TESTING ()
	ADD_SUBDIRECTORY (test)
ENDIF ()

# Doxygen
FIND_PACKAGE (Doxygen)
IF (DOXYGEN_FOUND)
	ADD_CUSTOM_TARGET (
		doxygen_c
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_c
		COMMAND ${CMAKE_COMMAND} -E echo "FILE_PATTERNS      = *.h" >> ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_c
		COMMAND ${CMAKE_COMMAND} -E echo "OUTPUT_DIRECTORY   = doc_c" >> ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_c
		COMMAND ${CMAKE_COMMAND} -E echo "PROJECT_NAME       = \"MessagePack for C\"" >> ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_c
		COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_c
		VERBATIM
	)
	ADD_CUSTOM_TARGET (
		doxygen_cpp
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_cpp
		COMMAND ${CMAKE_COMMAND} -E echo "FILE_PATTERNS      = *.hpp" >> ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_cpp
		COMMAND ${CMAKE_COMMAND} -E echo "OUTPUT_DIRECTORY   = doc_cpp" >> ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_cpp
		COMMAND ${CMAKE_COMMAND} -E echo "PROJECT_NAME       = \"MessagePack for C++\"" >> ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_cpp
		COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_cpp
		VERBATIM
	)
	ADD_CUSTOM_TARGET (
		doxygen
		DEPENDS doxygen_c doxygen_cpp
	)
ENDIF ()
