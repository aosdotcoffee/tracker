#pragma once
#include <stddef.h>

void* xmalloc(size_t size);
void* xcalloc(size_t num, size_t size);

#define ALLOC_STRUCT(name, type) type* name = (type*) xcalloc(1, sizeof(type));
