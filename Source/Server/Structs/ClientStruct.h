#ifndef CLIENTSTRUCT_H
#define CLIENTSTRUCT_H

#include <Server/Structs/GameserverStruct.h>
#include <Server/Structs/TimerStruct.h>
#include <Util/Enums.h>
#include <enet/enet.h>
#include <stdint.h>

typedef struct server server_t;

typedef struct client
{
    ENetPeer*      peer;
    server_t*      server;
    timers_t       timers;
    state_t        state;
    uint8_t        id;
    gameserver_t   gameserver;
} client_t;

#endif
