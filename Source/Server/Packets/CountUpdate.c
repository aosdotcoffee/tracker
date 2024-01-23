#include <Server/Structs/ClientStruct.h>
#include <Server/Packets/CountUpdate.h>
#include <Util/DataStream.h>
#include <Util/Alloc.h>

count_update_pkt* parse_count_update_packet(client_t* client, stream_t* stream) {
    ALLOC_STRUCT(packet, count_update_pkt);

    (void) client;
    packet->current_players = stream_read_u8(stream);

    return packet;
}
