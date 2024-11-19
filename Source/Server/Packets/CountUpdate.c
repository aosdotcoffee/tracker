#include <Server/Packets/CountUpdate.h>
#include <Server/Structs/ClientStruct.h>
#include <Util/DataStream.h>
#include <Util/Log.h>
#include <Util/Mem.h>

count_update_pkt* parse_count_update_packet(client_t* client, stream_t* stream)
{
    auto packet = make(count_update_pkt);

    if (stream->length != 1) {
        LOG_CLIENT_WARNING(client, "Invalid CountUpdate length");
        return NULL;
    }

    (void) client;
    packet->current_players = stream_read_u8(stream);

    return packet;
}
