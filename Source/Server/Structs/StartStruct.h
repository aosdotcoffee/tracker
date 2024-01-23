#ifndef STARTSTRUCT_H
#define STARTSTRUCT_H

typedef struct server_args
{
    // enet
    uint32_t max_connections;
    uint32_t max_bandwidth_in;
    uint32_t max_bandwidth_out;
    uint32_t channels;

    // master
    uint16_t master_port;
    uint64_t idle_timeout;
    char*    mmdb_path;
    uint8_t  max_connections_per_range;

    // httpd
    uint16_t httpd_port;
} server_args;

#endif
