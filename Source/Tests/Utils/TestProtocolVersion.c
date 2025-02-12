#include <Tests/TestUtil.h>
#include <Util/Enums.h>

int main() {
    assert_false(protocol_version_is_valid(0));
    assert_false(protocol_version_is_valid(-1));
    assert_false(protocol_version_is_valid(100));
    assert_true(protocol_version_is_valid(31));
    assert_true(protocol_version_is_valid(32));
}
