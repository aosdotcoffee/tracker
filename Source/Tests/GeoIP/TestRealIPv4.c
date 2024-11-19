#include <Tests/GeoIP/TestGeoIP.h>

int main(void) {
    GEOIP_TEST_DECLS();

    /* US DoD */
    GEOIP_ASSERT_COUNTRY("132.87.254.0", "US");
    GEOIP_ASSERT_COUNTRY("132.92.253.0", "US");
    GEOIP_ASSERT_COUNTRY("132.96.198.0", "US");
    GEOIP_ASSERT_COUNTRY("55.15.0.0", "US");
}
