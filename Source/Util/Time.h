#pragma once
#include <stdint.h>
#include <threads.h>
#ifdef _WIN32
    #include <synchapi.h>
    #include <time.h>
#else
    #include <time.h>
#endif
#include <assert.h>

enum : uint64_t {
    TIME_NANOS_IN_MICRO = 1000,
    TIME_NANOS_IN_MILLI = 1000000,
    TIME_NANOS_IN_SECOND = 1000000000,
    TIME_NANOS_IN_MINUTE = 60000000000,
    TIME_MICROS_IN_MILLI = 1000,
};

[[nodiscard]]
static inline uint64_t time_now(void)
{
    struct timespec ts;

    [[maybe_unused]]
    int ret = timespec_get(&ts, TIME_UTC);
    assert(ret != 0);
    return ((uint64_t) ts.tv_sec) * TIME_NANOS_IN_SECOND + ts.tv_nsec;
}

[[nodiscard]]
static inline uint64_t time_since(uint64_t time)
{
    return time_now() - time;
}

static inline void time_sleep_millisec(uint64_t duration)
{
#ifdef _WIN32
    Sleep(duration);
#else
    struct timespec tv;
    tv.tv_sec = 0;
    tv.tv_nsec = duration * TIME_NANOS_IN_MILLI;
    (void) thrd_sleep(&tv, nullptr);
#endif
}
