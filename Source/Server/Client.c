#include <Server/Client.h>
#include <Server/GeoIP.h>
#include <Server/Structs/ServerStruct.h>
#include <Util/DataStream.h>
#include <Util/Enums.h>
#include <Util/Log.h>
#include <Util/Nanos.h>
#include <enet6/enet.h>
#include <string.h>

void client_init(client_t* client)
{
    client->timers.connection_established = time_now();
    client->state                         = STATE_READY;

    if (client->server->has_mmdb) {
        char ip_address[48];
        ENetAddress address;
        memcpy(&address, &client->peer->address, sizeof(ENetAddress));
        address.port = 0;

        enet_address_get_host_ip(&address, ip_address, 48);

        int result = geoip_get_alpha2_country(
        &client->server->mmdb, client->gameserver.country_code, ip_address);

        if (result != 0) {
            LOG_CLIENT_WARNING(client, "Could not look up GeoIP");
            client->gameserver.country_code[0] = '\0';
        }
    }
}

void client_destroy(client_t* client)
{
    free(client);
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

    if (count_update->current_players > client->gameserver.max_players) {
        LOG_CLIENT_WARNING(client,
                           "Discarded CountUpdate with count %u/%u",
                           count_update->current_players,
                           client->gameserver.max_players);
        return;
    }

    client->gameserver.current_players = count_update->current_players;

    LOG_CLIENT_INFO(client,
                    "Updated  : [%s] [Players: %u/%u]",
                    client->gameserver.name,
                    count_update->current_players,
                    client->gameserver.max_players);
}
