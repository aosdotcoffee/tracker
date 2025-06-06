#pragma once
#include <stdio.h> // IWYU pragma: keep

void log_print_with_time(const char* format, ...);

#define LOG__INT(msg, ...) log_print_with_time(msg "%s", __VA_ARGS__)
#define LOG__CLIENT_INT(client, msg, ...)                                 \
    if (client) {                                                         \
        char _client_ip[48];                                              \
        enet_address_get_host_ip(&client->peer->address, _client_ip, 48); \
        log_print_with_time(msg "%s", _client_ip, __VA_ARGS__);           \
    } else {                                                              \
        log_print_with_time(msg "%s", "(no client)", __VA_ARGS__);        \
    }

#define LOG__INT_WITHOUT_TIME(msg, ...) printf(msg "%s", __VA_ARGS__)
#define LOGF__INT(file, msg, ...) fprintf(file, msg "%s", __VA_ARGS__)

#define LOG_DEBUG(...)                                                                  \
    LOG__INT(                                                                           \
        "\x1b[1;30m[\x1b[1;35mDEBUG \x1b[1;30m] \x1b[0;35m" __VA_ARGS__, "\x1B[0;37m\n" \
    )

#define LOG_INFO(...)                                                                   \
    LOG__INT(                                                                           \
        "\x1b[1;30m[\x1b[1;36mINFO  \x1b[1;30m] \x1b[0;36m" __VA_ARGS__, "\x1B[0;37m\n" \
    )

#define LOG_STATUS(...)                                                                 \
    LOG__INT(                                                                           \
        "\x1b[1;30m[\x1b[1;32mSTATUS\x1b[1;30m] \x1b[0;32m" __VA_ARGS__, "\x1B[0;37m\n" \
    )

#define LOG_WARNING(...)                                                                \
    LOG__INT(                                                                           \
        "\x1b[1;30m[\x1b[1;33mWARN  \x1b[1;30m] \x1b[0;33m" __VA_ARGS__, "\x1B[0;37m\n" \
    )

#define LOG_DEBUG_WITHOUT_TIME(...) \
    LOG__INT_WITHOUT_TIME("\x1B[0;35mDEBUG: " __VA_ARGS__, "\x1B[0;37m\n")

#define LOG_INFO_WITHOUT_TIME(...) \
    LOG__INT_WITHOUT_TIME("\x1B[0;36mINFO: " __VA_ARGS__, "\x1B[0;37m\n")

#define LOG_STATUS_WITHOUT_TIME(...) \
    LOG__INT_WITHOUT_TIME("\x1B[0;32mSTATUS: " __VA_ARGS__, "\x1B[0;37m\n")

#define LOG_WARNING_WITHOUT_TIME(...) \
    LOG__INT_WITHOUT_TIME("\x1B[0;33mWARNING: " __VA_ARGS__, "\x1B[0;37m\n")

#define LOG_ERROR(...) LOGF__INT(stderr, "\x1B[0;31mERROR: " __VA_ARGS__, "\x1B[0;37m\n")

/* for convenience */
#define LOG_CLIENT_DEBUG(client, ...)                                                   \
    LOG__CLIENT_INT(                                                                    \
        client,                                                                         \
        "\x1b[1;30m[\x1b[1;35mDEBUG \x1b[1;30m] \x1b[0;37m%s \x1b[0;35m| " __VA_ARGS__, \
        "\x1B[0;37m\n"                                                                  \
    )

#define LOG_CLIENT_INFO(client, ...)                                                    \
    LOG__CLIENT_INT(                                                                    \
        client,                                                                         \
        "\x1b[1;30m[\x1b[1;36mINFO  \x1b[1;30m] \x1b[0;37m%s \x1b[0;36m| " __VA_ARGS__, \
        "\x1B[0;37m\n"                                                                  \
    )

#define LOG_CLIENT_STATUS(client, ...)                                                  \
    LOG__CLIENT_INT(                                                                    \
        client,                                                                         \
        "\x1b[1;30m[\x1b[1;32mSTATUS\x1b[1;30m] \x1b[0;37m%s \x1b[0;32m| " __VA_ARGS__, \
        "\x1B[0;37m\n"                                                                  \
    )

#define LOG_CLIENT_WARNING(client, ...)                                                 \
    LOG__CLIENT_INT(                                                                    \
        client,                                                                         \
        "\x1b[1;30m[\x1b[1;33mWARN  \x1b[1;30m] \x1b[0;37m%s \x1b[0;33m| " __VA_ARGS__, \
        "\x1B[0;37m\n"                                                                  \
    )

#define LOG_CLIENT_ERROR(client, ...)                                                   \
    LOG__CLIENT_INT(                                                                    \
        client,                                                                         \
        "\x1b[1;30m[\x1b[1;31mERROR \x1b[1;30m] \x1b[0;37m%s \x1b[0;33m| " __VA_ARGS__, \
        "\x1B[0;37m\n"                                                                  \
    )
