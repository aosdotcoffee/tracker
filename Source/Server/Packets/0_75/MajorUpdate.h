#ifndef MAJORUPDATE_H
#define MAJORUPDATE_H

#include <stdint.h>
#include <Util/DataStream.h>
#include <Util/Enums.h>
#include <Server/Structs/ClientStruct.h>

typedef struct major_update_packet
{
    uint8_t  max_players;
    uint16_t port;
    char     name[MAX_SERVER_NAME_LENGTH];
    char     gamemode[MAX_GAMEMODE_LENGTH];
    char     map[MAX_MAP_LENGTH];
} major_update_pkt;

major_update_pkt* parse_major_update_packet(client_t* client, stream_t* stream);

#endif
