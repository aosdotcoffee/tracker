#include <Util/Log.h>
#include <errno.h>
#include <maxminddb.h>
#include <string.h>

int geoip_init(MMDB_s* mmdb, const char* filename)
{
    int status = MMDB_open(filename, MMDB_MODE_MMAP, mmdb);

    if (status != MMDB_SUCCESS) {
        LOG_ERROR("Cannot open MMDB file %s - %s", filename, MMDB_strerror(status));

        if (status == MMDB_IO_ERROR) {
            LOG_ERROR("IO Error: %s", strerror(errno));
        }

        return -1;
    }

    return 0;
}

int geoip_get_alpha2_country(MMDB_s* mmdb, char* out, const char* ip_address)
{
    MMDB_lookup_result_s mmdb_res;
    MMDB_entry_data_s entry_data;
    int gai_error;
    int mmdb_error;

    mmdb_res = MMDB_lookup_string(mmdb, ip_address, &gai_error, &mmdb_error);

    if (gai_error < 0) {
        return -1;
    }

    if (mmdb_error != MMDB_SUCCESS) {
        return -1;
    }

    if (!mmdb_res.found_entry) {
        return -1;
    }

    mmdb_error = MMDB_get_value(
        &mmdb_res.entry, &entry_data, "registered_country", "iso_code", NULL
    );

    if (mmdb_error != MMDB_SUCCESS) {
        return -1;
    }

    if (!entry_data.has_data) {
        return -1;
    }

    /* Expect country code to be 2 byte UTF-8 string (not null-terminated) */
    if (entry_data.type != MMDB_DATA_TYPE_UTF8_STRING || entry_data.data_size != 2) {
        return -1;
    }

    /* Convert result to null-terminated string */
    memcpy(out, entry_data.utf8_string, entry_data.data_size);
    out[2] = '\0';

    return 0;
}
