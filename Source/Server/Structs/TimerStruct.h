#pragma once
#include <stdint.h>

typedef struct timers
{
    uint64_t connection_established;
    uint64_t last_major_update;
    uint64_t last_count_update;
} timers_t;

typedef struct global_timers
{
    uint64_t start;
    uint64_t tick_time_budget;
} global_timers_t;
