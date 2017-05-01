IF (MSGPACK_BOOST)
    LIST (APPEND exec_PROGRAMS
        msgpack_variant_capitalize.cpp
        msgpack_variant_mapbased.cpp
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
ENDFOREACH ()
