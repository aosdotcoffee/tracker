#include <Tests/TestUtil.h>
#include <Util/Enums.h>

int main() {
    assert_false(protocol_version_is_valid(0));
    assert_false(protocol_version_is_valid(-1));
    assert_false(protocol_version_is_valid(100));
    assert_true(protocol_version_is_valid(31));
    assert_true(protocol_version_is_valid(32));

    assert_str_equals(protocol_version_to_string(VERSION_054), "0.54");
    assert_str_equals(protocol_version_to_string(VERSION_060), "0.60");
    assert_str_equals(protocol_version_to_string(VERSION_075), "0.75");
    assert_str_equals(protocol_version_to_string(VERSION_076), "0.76");
}
