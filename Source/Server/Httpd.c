#include <Server/Httpd.h>
#include <Server/Server.h>
#include <Util/Ensure.h>
#include <Util/JSONHelpers.h>
#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_types.h>
#include <json-c/json_util.h>
#include <microhttpd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>

static enum MHD_Result _httpd_process_request(void*                  cls,
                                              struct MHD_Connection* connection,
                                              const char*            url,
                                              const char*            method,
                                              const char*            version,
                                              const char*            upload_data,
                                              size_t*                upload_data_size,
                                              void**                 con_cls)
{
    (void) url;
    (void) method;
    (void) version;
    (void) upload_data;
    (void) upload_data_size;
    (void) con_cls;

    server_t*           server = (server_t*) cls;
    struct json_object* json   = json_object_new_array();

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

        char identifier[32];
        snprintf(identifier,
                 31,
                 "aos://%u:%u:0.75",
                 (uint32_t) *peer->address.host.v4,
                 client->gameserver.port);

        struct json_object* obj = json_object_new_object();
        JSON_SET_STRING(obj, "name", client->gameserver.name);
        JSON_SET_STRING(obj, "game_version", "0.75");
        JSON_SET_STRING(obj, "identifier", identifier);
        JSON_SET_STRING(obj, "map", client->gameserver.map);
        JSON_SET_STRING(obj, "gamemode", client->gameserver.gamemode);
        JSON_SET_STRING(obj, "country", "AQ");
        JSON_SET_INT(obj, "latency", 0);
        JSON_SET_INT(obj, "players_current", client->gameserver.current_players);
        JSON_SET_INT(obj, "players_max", client->gameserver.max_players);
        JSON_SET_INT(
        obj, "last_updated", client->timers.last_count_update / NANO_IN_MILLI);

        json_object_array_add(json, obj);
    }
    pthread_mutex_unlock(&server->lock);

    const char*          page = json_object_to_json_string(json);
    struct MHD_Response* response;
    int                  ret;

    /* MHD_RESPMEM_MUST_COPY: tell MHD to make its own copy of page, otherwise it would
     * free() it before we call json_object_put() */
    response =
    MHD_create_response_from_buffer(strlen(page), (void*) page, MHD_RESPMEM_MUST_COPY);

    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    json_object_put(json);

    return ret;
}

void httpd_start(server_t* server, uint16_t port)
{
    LOG_STATUS("Creating httpd at port %d", port);

    server->httpd = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD,
                                     port,
                                     NULL,
                                     NULL,
                                     &_httpd_process_request,
                                     (void*) server,
                                     MHD_OPTION_END);

    ENSURE(server->httpd != NULL, "Failed to start libmicrohttpd daemon");

    LOG_STATUS("Httpd started");
}

void httpd_stop(server_t* server)
{
    MHD_stop_daemon(server->httpd);
    LOG_STATUS("Httpd stopped");
}
