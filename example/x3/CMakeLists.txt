IF (MSGPACK_USE_X3_PARSE AND MSGPACK_DEFAULT_API_VERSION VERSION_GREATER 1)
    FIND_PACKAGE (Boost REQUIRED COMPONENTS context system)
    FIND_PACKAGE (Threads REQUIRED)

    LIST (APPEND exec_PROGRAMS
        unpack.cpp
        parse.cpp
    )
    LIST (APPEND with_boost_PROGRAMS
        stream_unpack.cpp
    )
    FOREACH (source_file ${exec_PROGRAMS})
        GET_FILENAME_COMPONENT (source_file_we ${source_file} NAME_WE)
        ADD_EXECUTABLE (
            ${source_file_we}
            ${source_file}
        )
        TARGET_LINK_LIBRARIES (${source_file_we} PRIVATE msgpack-cxx)

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
    FOREACH (source_file ${with_boost_PROGRAMS})
        GET_FILENAME_COMPONENT (source_file_we ${source_file} NAME_WE)
        ADD_EXECUTABLE (
            ${source_file_we}
            ${source_file}
        )
        TARGET_LINK_LIBRARIES (${source_file_we} PRIVATE
            msgpack-cxx
            Boost::context
            Boost::system
            Threads::Threads
        )
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
ENDIF ()
