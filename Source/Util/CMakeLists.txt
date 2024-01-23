#
# Add sources
#

add_library(Util STATIC "")

target_sources(Util
    PRIVATE
        DataStream.c
        Log.c
        Alloc.c
        Nanos.c
    PUBLIC
        Enums.h
        JSONHelpers.h
        TOMLHelpers.h
        DataStream.h
        Log.h
        Alloc.h
        Nanos.h
        Ensure.h
)

target_link_libraries(Util
    PUBLIC
        TrackerCommon
)
