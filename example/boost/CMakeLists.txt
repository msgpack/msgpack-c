FIND_PACKAGE (Boost REQUIRED COMPONENTS system)
FIND_PACKAGE (Threads REQUIRED)
FIND_PACKAGE (ZLIB REQUIRED)

LIST (APPEND exec_PROGRAMS
    msgpack_variant_capitalize.cpp
    msgpack_variant_mapbased.cpp
)

IF (MSGPACK_CXX11 OR MSGPACK_CXX14 OR MSGPACK_CXX17 OR MSGPACK_CXX20)
    LIST (APPEND exec_PROGRAMS asio_send_recv.cpp)
    IF (ZLIB_FOUND)
        LIST (APPEND exec_PROGRAMS asio_send_recv_zlib.cpp)
    ENDIF ()
ENDIF ()

FOREACH (source_file ${exec_PROGRAMS})
    GET_FILENAME_COMPONENT (source_file_we ${source_file} NAME_WE)
    ADD_EXECUTABLE (
        ${source_file_we}
        ${source_file}
    )
    TARGET_LINK_LIBRARIES (${source_file_we} PRIVATE
        msgpack-cxx
        Boost::system
        Threads::Threads
    )
    IF (ZLIB_FOUND)
        TARGET_LINK_LIBRARIES (${source_file_we} PRIVATE ZLIB::ZLIB)
    ENDIF ()
    IF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        SET_PROPERTY (TARGET ${source_file_we} APPEND_STRING PROPERTY COMPILE_FLAGS " -Wall -Wextra")
    ENDIF ()

    IF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        SET_PROPERTY (TARGET ${source_file_we} APPEND_STRING PROPERTY COMPILE_FLAGS " -Wno-mismatched-tags")
    ENDIF ()

    IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        IF (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
            STRING(REGEX REPLACE "/W[0-4]" "/W3 /WX" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        ELSE ()
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX")
        ENDIF ()
    ENDIF ()
ENDFOREACH ()
