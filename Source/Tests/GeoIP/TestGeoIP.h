#pragma once
#include "Server/GeoIP.h"
#include "Tests/TestUtil.h"

#define GEOIP_TEST_DECLS() \
    MMDB_s mmdb; \
    geoip_init(&mmdb, "../../GeoLite2-Country.mmdb"); \
    char code[] = "\0\0\0";

#define GEOIP_ASSERT_NONE(address) \
    assert_false(geoip_get_alpha2_country(&mmdb, code, address)); \
    assert_str_empty(code);

#define GEOIP_ASSERT_COUNTRY(address, country) \
    assert(geoip_get_alpha2_country(&mmdb, code, address) == true); \
    assert_str_equals(code, country); \
    memset(code, 0, 3);
