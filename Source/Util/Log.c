#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

void log_print_with_time(const char* format, ...)
{
    struct timespec ts;

    [[maybe_unused]]
    int ts_ret = timespec_get(&ts, TIME_UTC);
    assert(ts_ret != 0);

    char s[64];
    size_t ret = strftime(s, sizeof(s), "%d/%m %H:%M:%S", gmtime(&ts.tv_sec));
    if (ret == 0) {
        return;
    }

    va_list args;
    va_start(args, format);
    char f_message[1024];
    vsnprintf(f_message, 1024, format, args);
    va_end(args);

    fprintf(stderr, "\x1b[37m%s\x1B[0;37m %s", s, f_message);
}
