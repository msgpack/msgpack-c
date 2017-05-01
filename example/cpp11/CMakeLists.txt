IF (MSGPACK_CXX11)
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
                MsgPack::Cpp
            )
        ELSE ()
            TARGET_INCLUDE_DIRECTORIES (${source_file_we}
                PRIVATE
                    $<TARGET_PROPERTY:MsgPack::C,INTERFACE_INCLUDE_DIRECTORIES>
            )
        ENDIF ()
    ENDFOREACH ()
ENDIF ()
