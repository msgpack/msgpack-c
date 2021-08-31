FIND_PACKAGE (Threads REQUIRED)
FIND_PACKAGE (ZLIB)
FIND_PACKAGE (Boost REQUIRED COMPONENTS unit_test_framework system)

LIST (APPEND check_PROGRAMS
    array_ref.cpp
    boost_fusion.cpp
    boost_optional.cpp
    boost_string_ref.cpp
    boost_string_view.cpp
    boost_variant.cpp
    buffer.cpp
    carray.cpp
    cases.cpp
    convert.cpp
    fixint.cpp
    inc_adaptor_define.cpp
    json.cpp
    limit.cpp
    msgpack_basic.cpp
    msgpack_container.cpp
    msgpack_stream.cpp
    msgpack_tuple.cpp
    msgpack_vref.cpp
    object.cpp
    object_with_zone.cpp
    pack_unpack.cpp
    raw.cpp
    reference.cpp
    size_equal_only.cpp
    streaming.cpp
    user_class.cpp
    version.cpp
    visitor.cpp
    zone.cpp
)

IF (MSGPACK_USE_X3_PARSE)
    LIST (APPEND check_PROGRAMS
        msgpack_x3_parse.cpp
    )
ENDIF ()

IF (MSGPACK_CXX11 OR MSGPACK_CXX14 OR MSGPACK_CXX17 OR MSGPACK_CXX20)
    LIST (APPEND check_PROGRAMS
        iterator_cpp11.cpp
        msgpack_cpp11.cpp
        reference_cpp11.cpp
        reference_wrapper_cpp11.cpp
        shared_ptr_cpp11.cpp
        unique_ptr_cpp11.cpp

        # fuzzers are cpp11 only
        fuzz_unpack_pack_fuzzer_cpp11.cpp
    )
ENDIF ()

IF (MSGPACK_CXX17 OR MSGPACK_CXX20)
    LIST (APPEND check_PROGRAMS
        msgpack_cpp17.cpp
    )
ENDIF ()

IF (MSGPACK_CXX20)
    LIST (APPEND check_PROGRAMS
        msgpack_cpp20.cpp
    )
ENDIF ()

FOREACH (source_file ${check_PROGRAMS})
    GET_FILENAME_COMPONENT (source_file_we ${source_file} NAME_WE)
    ADD_EXECUTABLE (
        ${source_file_we}
        ${source_file}
    )

    TARGET_COMPILE_DEFINITIONS (${source_file_we} PRIVATE
        $<IF:$<BOOL:${MSGPACK_USE_STATIC_BOOST}>,,BOOST_TEST_DYN_LINK>)

    TARGET_LINK_LIBRARIES (${source_file_we}
        msgpackc-cxx
        Boost::system
        Boost::unit_test_framework
        Threads::Threads
        ZLIB::ZLIB
    )

    ADD_TEST (NAME ${source_file_we} COMMAND ${source_file_we})

    IF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        SET_PROPERTY (TARGET ${source_file_we} APPEND_STRING PROPERTY COMPILE_FLAGS " -Wall -Wextra -Wconversion")
    ENDIF ()
    IF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        SET_PROPERTY (TARGET ${source_file_we} APPEND_STRING PROPERTY COMPILE_FLAGS " -Wno-mismatched-tags")
    ENDIF ()

    IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        IF (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
            STRING (REGEX REPLACE "/W[0-4]" "/W3 /WX" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        ELSE ()
            SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX")
        ENDIF ()
    ENDIF ()
ENDFOREACH ()

ADD_EXECUTABLE (
    multi_file
    multi_file1.cpp multi_file2.cpp
)

TARGET_LINK_LIBRARIES (multi_file
    msgpackc-cxx
)

ADD_TEST (multi_file multi_file)
