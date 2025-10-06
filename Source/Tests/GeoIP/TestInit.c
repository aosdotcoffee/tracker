#include "Server/GeoIP.h"
#include <assert.h>

int main(void) {
    MMDB_s mmdb;
    assert(geoip_init(&mmdb, "../../GeoLite2-Country.mmdb") == MMDB_SUCCESS);
}
