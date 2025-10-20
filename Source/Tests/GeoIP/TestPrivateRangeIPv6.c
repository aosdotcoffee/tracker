#include "TestGeoIP.h"

int main(void)
{
    GEOIP_TEST_DECLS();

    GEOIP_ASSERT_NONE("::");
    GEOIP_ASSERT_NONE("::1");
    GEOIP_ASSERT_NONE("fe80::");
    GEOIP_ASSERT_NONE("fe80::1");
    GEOIP_ASSERT_NONE("fd00::1");
    GEOIP_ASSERT_NONE("fdff::1");
}
