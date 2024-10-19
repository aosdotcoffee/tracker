#pragma once
#include <Util/Log.h>

#define ENSURE(condition, message)                      \
    {                                                   \
        if ((condition) == 0) {                         \
            LOG_ERROR("ASSERTION FAILED: " #condition); \
            LOG_ERROR("%s", message);                   \
            abort();                                    \
        }                                               \
    }
