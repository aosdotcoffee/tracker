#include <Server/Client.h>
#include <Server/GeoIP.h>
#include <Server/Httpd.h>
#include <Server/Packets/CountUpdate.h>
#include <Server/Packets/MajorUpdate.h>
#include <Server/Server.h>
#include <Server/Structs/ServerStruct.h>
#include <Server/Structs/StartStruct.h>
#include <Util/Alloc.h>
#include <Util/ENetHelpers.h>
#include <Util/Ensure.h>
#include <Util/Log.h>
#include <Util/Nanos.h>
#include <enet6/enet.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

void server_start(server_t* server, const server_args* args)
{
    server->global_timers.start = time_now();

    /* Initialize ENet */
    LOG_STATUS("Initializing ENet");
    ENSURE(enet_initialize() == 0, "Failed to initialize ENet");
    atexit(enet_deinitialize);

    ENSURE(
        pthread_mutex_init(&server->lock, NULL) == 0, "Server mutex failed to initialize"
    );

    ENetAddress address;
    enet_address_build_any(&address, ENET_ADDRESS_TYPE_IPV6);
    address.port = args->master_port;

    LOG_STATUS("Creating master at port %d", args->master_port);
    server->host = enet_host_create(
        ENET_ADDRESS_TYPE_IPV6,
        &address,
        args->max_connections,
        args->channels,
        args->max_bandwidth_in,
        args->max_bandwidth_out
    );
    ENSURE(server->host != NULL, "Failed to create ENet host");
    ENSURE(
        enet_host_compress_with_range_coder(server->host) == 0,
        "Failed to enable range coder"
    );

    if (args->mmdb_path) {
        LOG_STATUS("Initializing MMDB");
        ENSURE(
            geoip_init(&server->mmdb, args->mmdb_path) == 0, "Failed to initialize MMDB"
        );
        server->has_mmdb = 1;
    } else {
        LOG_WARNING("No MMDB path specified, GeoIP will be disabled");
        server->has_mmdb = 0;
    }

    LOG_STATUS("Initializing server");

    server->idle_timeout = args->idle_timeout * NANO_IN_MILLI;
    server->max_connections_per_range = args->max_connections_per_range;
    server->clients = NULL;
    server->running = 1;

    httpd_start(server, args->httpd_port);
    LOG_STATUS("Master started");

    while (server->running) {
        pthread_mutex_lock(&server->lock);
        server_receive_events(server);
        server_update_clients(server);
        pthread_mutex_unlock(&server->lock);
        sleep(0);
    }

    // Server is shutting down
    FOR_PEERS(server->host, peer)
    {
        if (peer->data == NULL) {
            continue;
        }

        enet_peer_disconnect_now(peer, REASON_SERVER_SHUTTING_DOWN);
        client_destroy((client_t*) peer->data);
    }

    LOG_STATUS("Master stopped");
    httpd_stop(server);
    enet_host_destroy(server->host);
    pthread_mutex_destroy(&server->lock);
}

void server_receive_events(server_t* server)
{
    ENetEvent event;

    while (enet_host_service(server->host, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_NONE:
                LOG_STATUS("Received an ENet event type of NONE. Derp.");
                break;

            case ENET_EVENT_TYPE_CONNECT:
                server_handle_enet_connect(server, &event);
                break;

            case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
            case ENET_EVENT_TYPE_DISCONNECT:
                server_handle_enet_disconnect(server, &event);
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                server_handle_enet_receive(server, &event);
                break;
        }
    }
}

void server_update_clients(server_t* server)
{
    FOR_PEERS(server->host, peer)
    {
        if (peer->state == ENET_PEER_STATE_CONNECTED) {
            client_update((client_t*) peer->data);
        }
    }
}

void server_handle_enet_connect(server_t* server, ENetEvent* event)
{
    /* check if the client is connecting with the right version */
    if (event->data != VERSION_17 && event->data != VERSION_31 &&
        event->data != VERSION_23)
    {
        char ip_address[48];
        enet_address_get_host_ip(&event->peer->address, ip_address, 48);

        LOG_WARNING("Disconnected %s: unsupported protocol %u", ip_address, event->data);

        enet_peer_disconnect_now(event->peer, REASON_WRONG_PROTOCOL_VERSION);
        return;
    }

    uint8_t similar_peers = 0;
    if (ENETH_ADDRESS_IS_V4_MAPPED(&event->peer->address)) {
        FOR_PEERS(server->host, peer)
        {
            if (peer->state == ENET_PEER_STATE_CONNECTED &&
                peer->address.host.v6[6] == event->peer->address.host.v6[6] &&
                peer->address.host.v6[7] == event->peer->address.host.v6[7])
            {
                similar_peers++;
            }
        }
    } else {
        FOR_PEERS(server->host, peer)
        {
            if (peer->state == ENET_PEER_STATE_CONNECTED &&
                memcmp(
                    peer->address.host.v6,
                    event->peer->address.host.v6,
                    4 * sizeof(uint16_t)
                ) == 0)
            {
                similar_peers++;
            }
        }
    }

    if (similar_peers >= server->max_connections_per_range) {
        char ip_address[48];
        enet_address_get_host_ip(&event->peer->address, ip_address, 48);

        LOG_WARNING("Disconnected %s: too many active connections", ip_address);
        enet_peer_disconnect_now(event->peer, REASON_IP_LIMIT_EXCEEDED);
        return;
    }

    ALLOC_STRUCT(client, client_t);

    client->version = event->data;
    client->server = server;
    client->peer = event->peer;

    /* in future events, we can use ((client_t*) event->peer->data) to access the client
     * struct easily */
    event->peer->data = client;

    client_init(client);
    LOG_CLIENT_STATUS(
        client,
        "Connected [Country: %s] [Version: %u]",
        client->gameserver.country_code,
        client->version
    );
}

void server_handle_enet_disconnect(server_t* server, ENetEvent* event)
{
    (void) server;

    client_t* client = (client_t*) event->peer->data;
    if (client == NULL) {
        LOG_ERROR("Received a disconnect event from a peer that has no client");
        return;
    }

    if (event->type == ENET_EVENT_TYPE_DISCONNECT) {
        LOG_CLIENT_STATUS(client, "Disconnected: client closed connection");
    }

    if (event->type == ENET_EVENT_TYPE_DISCONNECT_TIMEOUT) {
        LOG_CLIENT_STATUS(client, "Disconnected: timed out");
    }

    client_destroy(client);
}

void server_handle_enet_receive(server_t* server, ENetEvent* event)
{
    (void) server;

    client_t* client = (client_t*) event->peer->data;
    if (client == NULL) {
        LOG_ERROR("Received a message from a peer that has no client");
        return;
    }

    stream_t stream = {event->packet->data, event->packet->dataLength, 0};

    /* no packet IDs, but CountUpdate is always 1 byte long */
    if (event->packet->dataLength == 1) {
        count_update_pkt* count_update = parse_count_update_packet(client, &stream);
        client_on_count_update_received(client, count_update);
        free(count_update);
    } else {
        major_update_pkt* major_update;

        switch (client->version) {
            case VERSION_31:
                major_update = parse_v31_major_update_packet(client, &stream);
                break;

            case VERSION_17:
            case VERSION_23:
                major_update = parse_v17_major_update_packet(client, &stream);
                break;

            default:
                LOG_CLIENT_ERROR(
                    client, "Received MajorUpdate on a version that doesn't support it"
                );
                goto end;
        }

        client_on_major_update_received(client, major_update);
        free(major_update);
    }

end:
    enet_packet_destroy(event->packet);
}

void server_stop(server_t* server)
{
    server->running = 0;
}
