#pragma once
#include <Server/Structs/ClientStruct.h>
#include <Server/Structs/TimerStruct.h>
#include <enet6/enet.h>
#include <maxminddb.h>
#include <pthread.h>
#include <signal.h>

typedef struct server
{
    ENetHost* host;
    client_t* clients;
    global_timers_t global_timers;
    struct MHD_Daemon* httpd;
    MMDB_s mmdb;
    uint8_t has_mmdb;
    pthread_mutex_t lock;
    volatile sig_atomic_t running; // volatile keyword is required to have an access to
                                   // this variable in any thread

    uint64_t idle_timeout;
    uint8_t max_connections_per_range;
} server_t;
