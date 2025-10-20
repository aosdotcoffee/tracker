#include "TestGeoIP.h"

int main(void)
{
    GEOIP_TEST_DECLS();

    /* US DoD */
    GEOIP_ASSERT_COUNTRY("132.87.254.0", "US");
    GEOIP_ASSERT_COUNTRY("132.92.253.0", "US");
    GEOIP_ASSERT_COUNTRY("132.96.198.0", "US");
    GEOIP_ASSERT_COUNTRY("55.15.0.0", "US");

    /* Oracle Cloud */
    GEOIP_ASSERT_COUNTRY("141.148.198.0", "IN");
}
