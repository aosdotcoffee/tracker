#include <Server/Packets/MajorUpdate.h>
#include <Server/Structs/ClientStruct.h>
#include <Util/Alloc.h>
#include <Util/DataStream.h>
#include <Util/Log.h>

major_update_pkt* parse_v31_major_update_packet(client_t* client, stream_t* stream)
{
    ALLOC_STRUCT(packet, major_update_pkt);

    /* max_players (1B) + port (2B) + name (at least null terminator, 1B) */
    if (stream_left(stream) < 4) {
        LOG_CLIENT_WARNING(client, "Malformed MajorUpdate packet: packet too small");
        free(packet);
        return NULL;
    }

    packet->max_players = stream_read_u8(stream);
    packet->port        = stream_read_u16(stream);

    size_t name_size = stream_read_string(stream, packet->name, 32);
    /* at least give me the server name! */
    if (name_size == 0) {
        LOG_CLIENT_WARNING(client, "Malformed MajorUpdate packet: server has no name");
        free(packet);
        return NULL;
    }

    /* it's ok if these are empty */
    stream_read_string(stream, packet->gamemode, 8);
    stream_read_string(stream, packet->map, 20);

    return packet;
}

major_update_pkt* parse_v17_major_update_packet(client_t* client, stream_t* stream)
{
    ALLOC_STRUCT(packet, major_update_pkt);

    /* max_players (1B) + name (at least null terminator, 1B) */
    if (stream_left(stream) < 3) {
        LOG_CLIENT_WARNING(client, "Malformed MajorUpdate packet: packet too small");
        free(packet);
        return NULL;
    }

    packet->max_players = stream_read_u8(stream);
    packet->port        = 32887; // this version of the protocol doesn't send the port!

    stream_read_string(stream, packet->name, 32);

    return packet;
}
