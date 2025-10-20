#include "GeoIP.h"

#include "Util/Log.h"

#include <maxminddb.h>
#include <string.h>

int geoip_init(MMDB_s* mmdb, const char* filename)
{
    return MMDB_open(filename, MMDB_MODE_MMAP, mmdb);
}

bool geoip_get_alpha2_country(MMDB_s* mmdb, char* out, const char* ip_address)
{
    MMDB_lookup_result_s mmdb_res;
    MMDB_entry_data_s entry_data;
    int gai_error;
    int mmdb_error;

    mmdb_res = MMDB_lookup_string(mmdb, ip_address, &gai_error, &mmdb_error);

    if (gai_error < 0 || mmdb_error != MMDB_SUCCESS || !mmdb_res.found_entry) {
        return false;
    }

    mmdb_error =
        MMDB_get_value(&mmdb_res.entry, &entry_data, "country", "iso_code", nullptr);

    if (mmdb_error != MMDB_SUCCESS || !entry_data.has_data ||

        /* Expect country code to be 2 byte UTF-8 string (not null-terminated) */
        entry_data.type != MMDB_DATA_TYPE_UTF8_STRING || entry_data.data_size != 2)
    {
        return false;
    }

    /* Convert result to null-terminated string */
    memcpy(out, entry_data.utf8_string, entry_data.data_size);
    out[2] = '\0';

    return true;
}
