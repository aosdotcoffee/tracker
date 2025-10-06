#include "Server/Packets/MajorUpdate.h"
#include "Server/Structs/ClientStruct.h"
#include "Util/DataStream.h"
#include "Util/Enums.h"
#include "Util/Log.h"
#include "Util/Mem.h"
#include <stdlib.h>

major_update_pkt* parse_v31_major_update_packet(client_t* client, stream_t* stream)
{
    /* max_players (1B) + port (2B) + name (at least null terminator, 1B) */
    if (stream_left(stream) < 4) {
        LOG_CLIENT_WARNING(client, "Malformed MajorUpdate packet: packet too small");
        return NULL;
    }

    auto packet = make(major_update_pkt);

    packet->max_players = stream_read_u8(stream);
    packet->port = stream_read_u16(stream);

    size_t name_size = stream_read_string(stream, packet->name, MAX_SERVER_NAME_LENGTH);
    /* at least give me the server name! */
    if (name_size == 0) {
        LOG_CLIENT_WARNING(client, "Malformed MajorUpdate packet: server has no name");
        free(packet);
        return NULL;
    }

    /* it's ok if these are empty */
    stream_read_string(stream, packet->gamemode, MAX_GAMEMODE_LENGTH);
    stream_read_string(stream, packet->map, MAX_MAP_LENGTH);

    return packet;
}

major_update_pkt* parse_v17_major_update_packet(client_t* client, stream_t* stream)
{
    auto packet = make(major_update_pkt);
    /* v17 does not support these */
    packet->gamemode[0] = '\0';
    packet->map[0] = '\0';
    /* you can't set a different port anyway */
    packet->port = 32887;

    /* max_players (1B) + name (at least null terminator, 1B) */
    if (stream_left(stream) < 3) {
        LOG_CLIENT_WARNING(client, "Malformed MajorUpdate packet: packet too small");
        free(packet);
        return NULL;
    }

    packet->max_players = stream_read_u8(stream);

    size_t name_size = stream_read_string(stream, packet->name, MAX_SERVER_NAME_LENGTH);
    if (name_size == 0) {
        LOG_CLIENT_WARNING(client, "Malformed MajorUpdate packet: server has no name");
        free(packet);
        return NULL;
    }

    return packet;
}
