// Copyright DarkNeutrino 2021
#pragma once

#define VERSION  \
    "UNRELEASED" \
    " Compiled on " __DATE__ " " __TIME__

#include <Server/Structs/ServerStruct.h>
#include <Server/Structs/StartStruct.h>
#include <enet6/enet.h>
#include <pthread.h>

#define FOR_PEERS(host, peer) \
    for (ENetPeer* peer = host->peers; peer < &host->peers[host->peerCount]; ++peer)

void server_start(server_t* server, const server_args* args);
void server_receive_events(server_t* server);
void server_update_clients(server_t* server);
void server_handle_enet_connect(server_t* server, ENetEvent* event);
void server_handle_enet_receive(server_t* server, ENetEvent* event);
void server_handle_enet_disconnect(server_t* server, ENetEvent* event);
void server_stop(server_t* server);
