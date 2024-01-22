#ifndef GEOIP_H
#define GEOIP_H

int geoip_init(MMDB_s* mmdb, const char* filename);
int geoip_get_alpha2_country(MMDB_s* mmdb, char* out, const char* ip_address);

#endif
