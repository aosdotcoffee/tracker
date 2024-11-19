#include <Tests/GeoIP/TestGeoIP.h>

int main(void) {
    GEOIP_TEST_DECLS();

    /* US DoD */
    GEOIP_ASSERT_COUNTRY("2608:144:2d::", "US");
    GEOIP_ASSERT_COUNTRY("2608:148:2c::", "US");
    GEOIP_ASSERT_COUNTRY("2608:148:2d::", "US");
    GEOIP_ASSERT_COUNTRY("2608:149:2c::", "US");
}
