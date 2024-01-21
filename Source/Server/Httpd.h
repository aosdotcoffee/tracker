#ifndef WEB_H
#define WEB_H

#include <Server/Structs/ServerStruct.h>

void httpd_start(server_t* server, uint16_t port);
void httpd_stop(server_t* server);

#endif
