#pragma once
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h> // IWYU pragma: keep

[[nodiscard]]
inline static void* xmalloc_sub(size_t size)
{
    void* allocated = malloc(size);
    assert(allocated != nullptr);
    return allocated;
}

[[nodiscard]]
inline static void* xcalloc_sub(size_t count, size_t cell_size)
{
    void* allocated = calloc(count, cell_size);
    assert(allocated != nullptr);
    return allocated;
}

#define mem_zero(v) memset(v, 0, sizeof(*v))

#define make(T, ...)                                   \
    ((T*) (__VA_OPT__(!) true ? xmalloc_sub(sizeof(T)) \
                              : xcalloc_sub(__VA_ARGS__ + 0, sizeof(T))))
