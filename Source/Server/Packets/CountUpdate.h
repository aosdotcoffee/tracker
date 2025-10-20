#pragma once
#include "Server/Structs/ClientStruct.h"
#include "Util/DataStream.h"

#include <stdint.h>

typedef struct count_update_packet
{
    uint8_t current_players;
} count_update_pkt;

count_update_pkt* parse_count_update_packet(client_t* client, stream_t* stream);
