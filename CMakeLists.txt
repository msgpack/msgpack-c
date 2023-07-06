CMAKE_MINIMUM_REQUIRED (VERSION 3.1 FATAL_ERROR)

CMAKE_POLICY (SET CMP0054 NEW)

PROJECT (msgpack-cxx LANGUAGES CXX)

ADD_LIBRARY (msgpack-cxx INTERFACE)

FILE (READ ${CMAKE_CURRENT_SOURCE_DIR}/include/msgpack/version_master.hpp contents)
STRING (REGEX MATCH "#define MSGPACK_VERSION_MAJOR *([0-9a-zA-Z_]*)" NULL_OUT ${contents})
SET (VERSION_MAJOR ${CMAKE_MATCH_1})
STRING (REGEX MATCH "#define MSGPACK_VERSION_MINOR *([0-9a-zA-Z_]*)" NULL_OUT ${contents})
SET (VERSION_MINOR ${CMAKE_MATCH_1})
STRING (REGEX MATCH "#define MSGPACK_VERSION_REVISION *([0-9a-zA-Z_]*)" NULL_OUT ${contents})
SET (VERSION_REVISION ${CMAKE_MATCH_1})
SET (VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_REVISION})

LIST (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
SET (GNUCXX_STD_SUPPORT_VERSION "4.3")

OPTION (MSGPACK_CXX11 "Using c++11 compiler" OFF)
OPTION (MSGPACK_CXX14 "Using c++14 compiler" OFF)
OPTION (MSGPACK_CXX17 "Using c++17 compiler" OFF)
OPTION (MSGPACK_CXX20 "Using c++20 compiler" OFF)

OPTION (MSGPACK_32BIT                   "32bit compile"                        OFF)
OPTION (MSGPACK_USE_BOOST               "Use Boost libraried"                  ON)
OPTION (MSGPACK_USE_X3_PARSE            "Use Boost X3 parse"                   OFF)
OPTION (MSGPACK_BUILD_TESTS             "Build tests"                          OFF)
OPTION (MSGPACK_BUILD_DOCS              "Build Doxygen documentation"          ON)
OPTION (MSGPACK_FUZZ_REGRESSION         "Enable regression testing"            OFF)
OPTION (MSGPACK_BUILD_EXAMPLES          "Build msgpack examples"               OFF)
OPTION (MSGPACK_GEN_COVERAGE            "Generate coverage report"             OFF)
OPTION (MSGPACK_USE_STATIC_BOOST        "Statically link with boost libraries" OFF)
OPTION (MSGPACK_CHAR_SIGN               "Char sign to use (signed or unsigned)")
OPTION (MSGPACK_USE_STD_VARIANT_ADAPTOR "Enable the adaptor for std::variant"  OFF)

SET (CMAKE_CXX_STANDARD_REQUIRED ON)

IF (MSGPACK_USE_X3_PARSE)
    IF (NOT (MSGPACK_CXX14 OR MSGPACK_CXX17 OR MSGPACK_CXX20))
        MESSAGE (FATAL_ERROR "MSGPACK_USE_X3_PARSE requires MSGPACK_CXX14 or newer")
    ENDIF ()
    SET (CMAKE_CXX_FLAGS "-DMSGPACK_USE_X3_PARSE ${CMAKE_CXX_FLAGS}")
ENDIF ()

IF (MSGPACK_CXX20)
    SET (CMAKE_CXX_STANDARD 20)
ELSEIF (MSGPACK_CXX17)
    SET (CMAKE_CXX_STANDARD 17)
ELSEIF (MSGPACK_CXX14)
    SET (CMAKE_CXX_STANDARD 14)
ELSEIF (MSGPACK_CXX11)
    SET (CMAKE_CXX_STANDARD 11)
ELSE ()
    SET (CMAKE_CXX_STANDARD 98)
ENDIF ()

IF (MSGPACK_32BIT)
    IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        TARGET_COMPILE_OPTIONS(msgpack-cxx INTERFACE -m32)
        TARGET_LINK_OPTIONS(msgpack-cxx INTERFACE -m32)
    ELSEIF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        TARGET_COMPILE_OPTIONS(msgpack-cxx INTERFACE -m32)
        TARGET_LINK_OPTIONS(msgpack-cxx INTERFACE -m32)
    ENDIF ()
ENDIF ()

IF (MSGPACK_USE_BOOST)
    SET (Boost_USE_MULTITHREADED ON)

    IF (MSGPACK_USE_STATIC_BOOST)
        MESSAGE (STATUS "Staticly linking with Boost")
        SET (Boost_USE_STATIC_LIBS TRUE)
    ELSE ()
        MESSAGE (STATUS "Dynamically linking with Boost")
        SET (Boost_USE_STATIC_LIBS FALSE)
    ENDIF ()

    FIND_PACKAGE (Boost REQUIRED)
ELSE ()
    TARGET_COMPILE_DEFINITIONS(msgpack-cxx INTERFACE MSGPACK_NO_BOOST)
ENDIF ()

IF (MSGPACK_CHAR_SIGN)
    TARGET_COMPILE_OPTIONS(msgpack-cxx INTERFACE -f${MSGPACK_CHAR_SIGN}-char)
ENDIF ()

IF (MSGPACK_DEFAULT_API_VERSION)
    TARGET_COMPILE_DEFINITIONS(msgpack-cxx INTERFACE MSGPACK_DEFAULT_API_VERSION=${MSGPACK_DEFAULT_API_VERSION})
ELSE ()
    SET (MSGPACK_DEFAULT_API_VERSION 3)
    TARGET_COMPILE_DEFINITIONS(msgpack-cxx INTERFACE MSGPACK_DEFAULT_API_VERSION=3)
ENDIF ()

IF (MSGPACK_USE_STD_VARIANT_ADAPTOR)
    TARGET_COMPILE_DEFINITIONS(msgpack-cxx INTERFACE MSGPACK_USE_STD_VARIANT_ADAPTOR)
ENDIF ()

IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    IF (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.1)
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
"
        MSGPACK_ENABLE_GCC_CXX_ATOMIC)
    ENDIF ()
ENDIF ()


INCLUDE (Files.cmake)

TARGET_INCLUDE_DIRECTORIES (msgpack-cxx
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

IF (MSGPACK_USE_BOOST)
    TARGET_LINK_LIBRARIES (msgpack-cxx INTERFACE Boost::boost)
ENDIF ()

IF (MSGPACK_GEN_COVERAGE)
    IF (NOT MSGPACK_BUILD_TESTS)
        MESSAGE(FATAL_ERROR "Coverage requires -DMSGPACK_BUILD_TESTS=ON")
    ENDIF ()
    STRING (TOUPPER "${CMAKE_BUILD_TYPE}" UPPER_CMAKE_BUILD_TYPE)
    IF (NOT "${UPPER_CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
        MESSAGE (FATAL_ERROR "Coverage requires -DCMAKE_BUILD_TYPE=Debug")
    ENDIF ()

    INCLUDE (CodeCoverage)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_FLAGS}")
    SETUP_TARGET_FOR_COVERAGE(coverage make coverage test)
ENDIF ()

IF (MSGPACK_BUILD_TESTS)
    IF (NOT MSGPACK_USE_BOOST)
        MESSAGE(FATAL_ERROR "Test requires -DMSGPACK_USE_BOOST=ON")
    ENDIF ()
    ENABLE_TESTING ()
    # MEMORYCHECK_COMMAND_OPTIONS needs to place prior to CTEST_MEMORYCHECK_COMMAND
    SET (MEMORYCHECK_COMMAND_OPTIONS "--leak-check=full --show-leak-kinds=definite,possible --error-exitcode=1")
    FIND_PROGRAM (CTEST_MEMORYCHECK_COMMAND NAMES valgrind)
    INCLUDE (Dart)
    ADD_SUBDIRECTORY (test)
ENDIF ()

# enable regression testing
IF (MSGPACK_FUZZ_REGRESSION)
    ENABLE_TESTING ()
    ADD_SUBDIRECTORY (fuzz)
ENDIF ()

IF ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "sparc")
    TARGET_COMPILE_DEFINITIONS(msgpack-cxx INTERFACE MSGPACK_ZONE_ALIGN=8)
ENDIF ()

IF (MSGPACK_BUILD_EXAMPLES)
    ADD_SUBDIRECTORY (example)
ENDIF ()

# Doxygen
IF (MSGPACK_BUILD_DOCS)
    FIND_PACKAGE (Doxygen)
    IF (DOXYGEN_FOUND)
        LIST (APPEND Doxyfile_cpp_CONTENT
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_cpp
            COMMAND ${CMAKE_COMMAND} -E echo "FILE_PATTERNS      = *.hpp" >> ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_cpp
            COMMAND ${CMAKE_COMMAND} -E echo "OUTPUT_DIRECTORY   = doc_cpp" >> ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_cpp
            COMMAND ${CMAKE_COMMAND} -E echo "INPUT              = ${CMAKE_CURRENT_SOURCE_DIR}/include" >> ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_cpp
            COMMAND ${CMAKE_COMMAND} -E echo "EXTRACT_ALL        = YES" >> ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_cpp
            COMMAND ${CMAKE_COMMAND} -E echo "STRIP_FROM_PATH    = ${CMAKE_CURRENT_SOURCE_DIR}/include" >> ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_cpp
        )
        IF (DOXYGEN_DOT_FOUND)
            LIST (APPEND Doxyfile_cpp_CONTENT
                COMMAND ${CMAKE_COMMAND} -E echo "HAVE_DOT       = YES" >> ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_cpp
            )
        ENDIF ()
        ADD_CUSTOM_TARGET (
            doxygen
            ${Doxyfile_cpp_CONTENT}
            COMMAND ${CMAKE_COMMAND} -E echo "PROJECT_NAME       = \"MessagePack for C++\"" >> ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_cpp
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_cpp
            VERBATIM
        )
    ENDIF ()
ENDIF ()

include (GNUInstallDirs)

# Install library.
INSTALL (TARGETS msgpack-cxx
         EXPORT msgpack-cxx-targets
         COMPONENT msgpack-cxx
         # This provides include directory in exported target
         # relative to prefix in single directory we've put everything in.
         INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# Install headers from source tree.
INSTALL (DIRECTORY include/
         DESTINATION include
         COMPONENT msgpack-cxx
)

INCLUDE (CMakePackageConfigHelpers)

IF (NOT (CMAKE_VERSION VERSION_LESS 3.14))
    SET (extra_version_file_args ARCH_INDEPENDENT)
ENDIF ()
SET (cmake_config_path "${CMAKE_INSTALL_LIBDIR}/cmake/msgpack-cxx")

# Configure the main package file from source tree.
CONFIGURE_PACKAGE_CONFIG_FILE (
    msgpack-cxx-config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/msgpack-cxx-config.cmake"
    INSTALL_DESTINATION "${cmake_config_path}"
)

# Write package version file.
WRITE_BASIC_PACKAGE_VERSION_FILE (
    msgpack-cxx-config-version.cmake
    VERSION ${VERSION}
    COMPATIBILITY SameMajorVersion
    ${extra_version_file_args}
)

# Install the generated package version file and the main package file.
INSTALL (FILES
    "${CMAKE_CURRENT_BINARY_DIR}/msgpack-cxx-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/msgpack-cxx-config-version.cmake"
    DESTINATION "${cmake_config_path}"
    COMPONENT msgpack-cxx
)

# This installs package in install tree for using installed targets.
INSTALL (
    EXPORT msgpack-cxx-targets
    FILE msgpack-cxx-targets.cmake
    DESTINATION "${cmake_config_path}"
    COMPONENT msgpack-cxx
)
