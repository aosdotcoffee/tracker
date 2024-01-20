// Copyright DarkNeutrino 2021
#ifndef SERVER_H
#define SERVER_H

#define VERSION  \
    "UNRELEASED" \
    " Compiled on " __DATE__ " " __TIME__

#include <Server/Structs/ServerStruct.h>
#include <Server/Structs/StartStruct.h>
#include <Util/Enums.h>
#include <enet/enet.h>
#include <pthread.h>

#ifndef DEFAULT_SERVER_PORT
    #define DEFAULT_SERVER_PORT 32886
#endif

void server_start(server_t* server, const server_args* args);
void server_receive_events(server_t* server);
void server_update_clients(server_t* server);
void server_handle_enet_connect(server_t* server, ENetEvent* event);
void server_handle_enet_receive(server_t* server, ENetEvent* event);
void server_handle_enet_disconnect(server_t* server, ENetEvent* event);
void stop_server(server_t* server);

#endif /* SERVER_H */
