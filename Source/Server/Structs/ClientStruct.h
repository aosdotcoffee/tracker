#pragma once
#include <Server/Structs/GameserverStruct.h>
#include <Server/Structs/TimerStruct.h>
#include <Util/Enums.h>
#include <enet6/enet.h>
#include <stdint.h>

typedef struct server server_t;

typedef struct client
{
    ENetPeer* peer;
    server_t* server;
    timers_t timers;
    state_t state;
    uint8_t id;
    gameserver_t gameserver;
    protocol_version_t version;
} client_t;
