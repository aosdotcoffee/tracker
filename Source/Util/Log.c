#include <stdarg.h>
#include <stdio.h>
#include <time.h>

void log_print_with_time(const char* format, ...)
{
    time_t    t = time(NULL);
    struct tm tm;
#ifdef WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    char   s[64];
    size_t ret = strftime(s, sizeof(s), "%d/%m %H:%M:%S", &tm);
    if (ret == 0) {
        return;
    }

    va_list args;
    va_start(args, format);
    char f_message[1024];
    vsnprintf(f_message, 1024, format, args);
    va_end(args);

    printf("\x1b[37m%s\x1B[0;37m %s", s, f_message);
}
