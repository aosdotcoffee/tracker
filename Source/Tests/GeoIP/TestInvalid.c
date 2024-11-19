#include <Tests/GeoIP/TestGeoIP.h>

int main(void) {
    GEOIP_TEST_DECLS();

    GEOIP_ASSERT_NONE("notanip");
    GEOIP_ASSERT_NONE("111111");
    GEOIP_ASSERT_NONE("127.0.");

    /* ranges are not valid */
    GEOIP_ASSERT_NONE("127.0.0.1/24");
}
