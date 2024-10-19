#pragma once
#include <Util/Enums.h>
#include <stdint.h>

typedef struct gameserver
{
    uint8_t current_players;
    uint8_t max_players;
    uint16_t port;
    char name[MAX_SERVER_NAME_LENGTH];
    char gamemode[MAX_GAMEMODE_LENGTH];
    char map[MAX_MAP_LENGTH];
    char country_code[3];
} gameserver_t;
