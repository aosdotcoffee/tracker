#
# Add sources
#

add_library(Server STATIC "")

set(PACKET_HEADERS
    Packets/MajorUpdate.h
    Packets/CountUpdate.h
)

set(PACKET_SOURCES
    Packets/MajorUpdate.c
    Packets/CountUpdate.c
)

set(STRUCTS_HEADERS
    Structs/ClientStruct.h
    Structs/ServerStruct.h
    Structs/TimerStruct.h
    Structs/StartStruct.h
)

set(SERVER_HEADERS
    ${PACKET_HEADERS}
    ${STRUCTS_HEADERS}
    Server.h
    Client.h
    Httpd.h
    GeoIP.h
)

set(SERVER_SOURCES
    ${PACKET_SOURCES}
    Server.c
    Client.c
    Httpd.c
    GeoIP.c
)

target_sources(Server
    PRIVATE
        ${SERVER_SOURCES}
    PUBLIC
        ${SERVER_HEADERS}
)

target_link_libraries(Server
    PRIVATE
        TrackerCommon
        Util
)
