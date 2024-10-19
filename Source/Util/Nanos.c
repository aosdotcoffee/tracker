#include <stdint.h>
#include <time.h>

uint64_t time_now(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (uint64_t) ts.tv_sec * 1000000000UL + ts.tv_nsec;
}

uint64_t time_since(uint64_t time)
{
    return time_now() - time;
}
