#pragma once
#include "Structs/ServerStruct.h"
#include <stdint.h>

void httpd_start(server_t* server, uint16_t port);
void httpd_stop(server_t* server);
