FIND_PACKAGE (Threads REQUIRED)
FIND_PACKAGE (Boost REQUIRED COMPONENTS system filesystem unit_test_framework)

LIST (APPEND check_PROGRAMS
    regression_runner.cpp
)

FOREACH (source_file ${check_PROGRAMS})
    GET_FILENAME_COMPONENT (source_file_we ${source_file} NAME_WE)
    ADD_EXECUTABLE (
        ${source_file_we}
        ${source_file}
    )

    TARGET_COMPILE_DEFINITIONS (${source_file_we} PRIVATE
        $<IF:$<BOOL:${MSGPACK_USE_STATIC_BOOST}>,,BOOST_TEST_DYN_LINK>)

    TARGET_LINK_LIBRARIES (${source_file_we}
        msgpack-cxx
        Threads::Threads
        Boost::filesystem
        Boost::system
        Boost::unit_test_framework
    )

    ADD_TEST (${source_file_we} ${source_file_we})

    IF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
      SET_PROPERTY (TARGET ${source_file_we} APPEND_STRING PROPERTY COMPILE_FLAGS "-Wall -Wextra -Wno-mismatched-tags -g")
        IF ("${MSGPACK_SAN}" STREQUAL "ASAN")
            SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
            SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
        ELSEIF ("${MSGPACK_SAN}" STREQUAL "UBSAN")
            SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined")
            SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
        ENDIF()
    ENDIF()
ENDFOREACH ()
