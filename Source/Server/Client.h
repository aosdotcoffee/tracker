#pragma once
#include <Server/Packets/CountUpdate.h>
#include <Server/Packets/MajorUpdate.h>
#include <Server/Structs/ClientStruct.h>

void client_init(client_t* client);
void client_update(client_t* client);
void client_destroy(client_t* client);
void client_on_major_update_received(client_t* client, major_update_pkt* major_update);
void client_on_count_update_received(client_t* client, count_update_pkt* count_update);
