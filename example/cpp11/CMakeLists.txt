IF (MSGPACK_CXX11)
    INCLUDE_DIRECTORIES (
        ../include
    )

    LIST (APPEND exec_PROGRAMS
        container.cpp
        non_def_con_class.cpp
    )

    IF ("${MSGPACK_DEFAULT_API_VERSION}" GREATER "1")
        LIST (APPEND exec_PROGRAMS
            socket_stream_example.cpp
        )
    ENDIF ()

    FOREACH (source_file ${exec_PROGRAMS})
        GET_FILENAME_COMPONENT (source_file_we ${source_file} NAME_WE)
        ADD_EXECUTABLE (
            ${source_file_we}
            ${source_file}
        )
        IF (NOT CMAKE_VERSION VERSION_LESS 3.0)
            TARGET_LINK_LIBRARIES (${source_file_we}
                MsgPack::msgpackcpp
            )
        ELSE ()
            TARGET_INCLUDE_DIRECTORIES (${source_file_we}
                PRIVATE
                    $<TARGET_PROPERTY:MsgPack::msgpackc,INTERFACE_INCLUDE_DIRECTORIES>
            )
        ENDIF ()
        IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
            SET_PROPERTY (TARGET ${source_file_we} APPEND_STRING PROPERTY COMPILE_FLAGS "-Wall -Wextra -Werror -g -O3 ")
        ENDIF ()

        IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
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
