#include "Httpd.h"

#include "Server.h"
#include "Structs/ClientStruct.h"
#include "Util/ENetHelpers.h"
#include "Util/Ensure.h"
#include "Util/Enums.h"
#include "Util/JSONHelpers.h"
#include "Util/Log.h"

#include <enet6/enet.h>
#include <json-c/json_object.h>
#include <json-c/json_types.h>
#include <microhttpd.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static enum MHD_Result _httpd_process_request(
    void* cls,
    struct MHD_Connection* connection,
    const char* url,
    const char* method,
    const char* http_version,
    const char* upload_data,
    size_t* upload_data_size,
    void** con_cls
)
{
    (void) url;
    (void) method;
    (void) http_version;
    (void) upload_data;
    (void) upload_data_size;
    (void) con_cls;

    server_t* server = (server_t*) cls;
    struct json_object* json = json_object_new_array();

    pthread_mutex_lock(&server->lock);
    /* iterate through all clients that have a server */
    FOR_PEERS(server->host, peer)
    {
        client_t* client = (client_t*) peer->data;

        /* we only want peers that have a server */
        if (peer->state != ENET_PEER_STATE_CONNECTED || client->state != STATE_HAS_SERVER)
        {
            continue;
        }

        ENetAddress address;
        memcpy(&address, &peer->address, sizeof(ENetAddress));
        address.port = 0;

        char identifier[55];
        if (ENETH_ADDRESS_IS_V4_MAPPED(&address)) {
            /* if mapped v4 */
            uint32_t address_decimal = 0;

            address_decimal |= ((uint32_t) (peer->address.host.v6[6] & 0xFF00) >> 8);
            address_decimal |= ((uint32_t) peer->address.host.v6[6] & 0x00FF) << 8;
            address_decimal |= ((uint32_t) (peer->address.host.v6[7] & 0xFF00) >> 8)
                            << 16;
            address_decimal |= ((uint32_t) peer->address.host.v6[7] & 0x00FF) << 24;

            snprintf(
                identifier, 31, "aos://%u:%u", address_decimal, client->gameserver.port
            );
        } else {
            /* if plain v6 */
            char ipv6_address[40];

            ENSURE(
                enet_address_get_host_ip(&address, ipv6_address, sizeof(ipv6_address)) ==
                    0,
                "Failed to get host IPv6 address"
            );

            snprintf(
                identifier,
                sizeof(identifier),
                "aos://[%s]:%u",
                ipv6_address,
                client->gameserver.port
            );
        }

        struct json_object* item = json_object_new_object();
        JSON_SET_STRING(item, "name", client->gameserver.name);

        const char* version = protocol_version_to_string(client->version);
        JSON_SET_STRING(item, "game_version", version);
        JSON_SET_STRING(item, "identifier", identifier);
        JSON_SET_STRING(item, "map", client->gameserver.map);
        JSON_SET_STRING(item, "game_mode", client->gameserver.gamemode);
        JSON_SET_STRING(item, "country", client->gameserver.country_code);
        JSON_SET_INT(item, "latency", 0);
        JSON_SET_INT(item, "players_current", client->gameserver.current_players);
        JSON_SET_INT(item, "players_max", client->gameserver.max_players);
        JSON_SET_INT(
            item, "last_updated", client->timers.last_count_update / NANO_IN_SECOND
        );

        json_object_array_add(json, item);
    }
    pthread_mutex_unlock(&server->lock);

    const char* page =
        (json_object_to_json_string_ext(json, JSON_C_TO_STRING_NOSLASHESCAPE));

    struct MHD_Response* response;
    int ret;

    /* MHD_RESPMEM_MUST_COPY: tell MHD to make its own copy of page, otherwise it would
     * free() it before we call json_object_put() */
    response = MHD_create_response_from_buffer(
        strlen(page), (void*) page, MHD_RESPMEM_MUST_COPY
    );

    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    json_object_put(json);

    return ret;
}

void httpd_start(server_t* server, uint16_t port)
{
    LOG_STATUS("Creating httpd at port %d", port);

    int flags = MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_DUAL_STACK;

    server->httpd = MHD_start_daemon(
        flags, port, nullptr, nullptr, &_httpd_process_request, (void*) server, MHD_OPTION_END
    );

    ENSURE(server->httpd != nullptr, "Failed to start libmicrohttpd daemon");

    LOG_STATUS("Httpd started");
}

void httpd_stop(server_t* server)
{
    MHD_stop_daemon(server->httpd);
    LOG_STATUS("Httpd stopped");
}
