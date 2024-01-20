#include <Server/Client.h>
#include <Server/Structs/ServerStruct.h>
#include <Util/DataStream.h>
#include <Util/Enums.h>
#include <Util/Log.h>
#include <Util/Nanos.h>
#include <enet/enet.h>
#include <string.h>

void client_init(client_t* client)
{
    client->timers.connection_established = time_now();
    client->state                         = STATE_READY;
}

void client_destroy(client_t* client)
{
    (void) client;
}

void client_update(client_t* client)
{
    /* idle for too long without announcing a server */
    if (client->state == STATE_READY &&
        time_since(client->timers.connection_established) >= client->server->idle_timeout)
    {
        LOG_CLIENT_STATUS(client,
                          "idle for too long without announcing a server, disconnecting");
        enet_peer_disconnect_now(client->peer, REASON_IDLE_FOR_TOO_LONG);
    }
}

void client_on_major_update_received(client_t* client, major_update_pkt* major_update)
{
    if (!major_update) {
        LOG_ERROR("Invalid MajorUpdate received");
        return;
    }

    client->gameserver.current_players = 0;
    client->gameserver.max_players     = major_update->max_players;
    client->gameserver.port            = major_update->port;
    memcpy(client->gameserver.name, major_update->name, MAX_SERVER_NAME_LENGTH);
    memcpy(client->gameserver.gamemode, major_update->gamemode, MAX_GAMEMODE_LENGTH);
    memcpy(client->gameserver.map, major_update->map, MAX_MAP_LENGTH);

    LOG_CLIENT_INFO(client,
                    "Announced: "
                    "[%s] [Game mode: %s] [Map: %s] [Players: 0/%u] [Port: %u]",
                    major_update->name,
                    major_update->gamemode,
                    major_update->map,
                    major_update->max_players,
                    major_update->port);

    client->state = STATE_HAS_SERVER;
}

void client_on_count_update_received(client_t* client, count_update_pkt* count_update)
{
    if (client->state != STATE_HAS_SERVER) {
        LOG_CLIENT_WARNING(client,
                           "Received a CountUpdate, but no server was announced.");
        return;
    }

    LOG_CLIENT_INFO(client,
                      "Updated  : [%s] [Players: %u/%u]",
                      client->gameserver.name,
                      count_update->current_players,
                      client->gameserver.max_players);

    (void) count_update;
}
