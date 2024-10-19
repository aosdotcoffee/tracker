#
# Add sources
#

add_library(Util STATIC "")

target_sources(Util
    PRIVATE
        DataStream.c
        Log.c
        Nanos.c
    PUBLIC
        Enums.h
        ENetHelpers.h
        JSONHelpers.h
        TOMLHelpers.h
        DataStream.h
        Log.h
        Nanos.h
        Mem.h
        Ensure.h
)

target_link_libraries(Util
    PUBLIC
        TrackerCommon
)
