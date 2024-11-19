#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#define assert_str_equals(a, b) \
    assert(strcmp((a), (b)) == 0)

#define assert_str_empty(a) \
    assert(strlen(a) == 0)

#define assert_true(expr) \
    assert((expr) == true)

#define assert_false(expr) \
    assert((expr) == false)

#define assert_null(expr) \
    assert((expr) == NULL)

[[maybe_unused]]
static void _dbgln_u8(uint8_t value) {
    fprintf(stderr, "%" PRIu8, value);
}

[[maybe_unused]]
static void _dbgln_u16(uint16_t value) {
    fprintf(stderr, "%" PRIu16, value);
}

[[maybe_unused]]
static void _dbgln_u32(uint32_t value) {
    fprintf(stderr, "%" PRIu32, value);
}

[[maybe_unused]]
static void _dbgln_u64(uint64_t value) {
    fprintf(stderr, "%" PRIu64, value);
}

#define dbgln(expr) \
    { \
        auto dbgln_value = (expr); \
        fputs("\x1b[1;34m[*]\x1b[0m " #expr " \x1b[1;30m=\x1b[0;0m ", stderr); \
        _Generic((expr), \
            uint8_t: _dbgln_u8, \
            uint16_t: _dbgln_u16, \
            uint32_t: _dbgln_u32, \
            uint64_t: _dbgln_u64 \
        )(dbgln_value); \
        fputs("\n", stderr); \
    }
