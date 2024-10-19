#pragma once
#include <maxminddb.h>

bool geoip_init(MMDB_s* mmdb, const char* filename);
bool geoip_get_alpha2_country(MMDB_s* mmdb, char* out, const char* ip_address);
