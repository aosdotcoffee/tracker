#include <Server/Server.h>
#include <Server/Structs/ServerStruct.h>
#include <Server/Structs/StartStruct.h>
#include <Util/TOMLHelpers.h>
#include <signal.h>
#include <tomlc99/toml.h>

static server_t g_server = {0};

static void _on_signal(int signal)
{
    (void) signal;

    fputs("\r", stdout);
    LOG_STATUS("Shutting down");
    server_stop(&g_server);
}

int main(void)
{
    server_args args;

    toml_table_t* parsed;
    toml_table_t* master_table;
    toml_table_t* httpd_table;
    TOMLH_READ_FROM_FILE(parsed, "config.toml");

    /* [master] */
    TOMLH_GET_TABLE(parsed, master_table, "master");
    TOMLH_GET_INT(master_table, args.master_port, "port", 0, 0);
    TOMLH_GET_INT(master_table, args.max_connections, "max_connections", 128, 1);
    TOMLH_GET_INT(master_table, args.max_bandwidth_in, "max_bandwidth_in", 0, 1);
    TOMLH_GET_INT(master_table, args.max_bandwidth_out, "max_bandwidth_out", 0, 1);
    TOMLH_GET_INT(master_table, args.channels, "channels", 2, 1);

    /* [httpd] */
    TOMLH_GET_TABLE(parsed, httpd_table, "httpd");
    TOMLH_GET_INT(httpd_table, args.httpd_port, "port", 0, 0);

    const char* TRACKER_BANNER = "\n"
                                 "                \x1b[1;32m▐           ▌        \n"
                                 "                \x1b[1;32m▜▀ ▙▀▖▝▀▖▞▀▖▌▗▘▞▀▖▙▀▖\n"
                                 "                \x1b[1;36m▐ ▖▌  ▞▀▌▌ ▖▛▚ ▛▀ ▌  \n"
                                 "                \x1b[1;36m ▀ ▘  ▝▀▘▝▀ ▘ ▘▝▀▘▘  \n"
                                 "\x1b[0;0m";

    if (isatty(1)) {
        puts(TRACKER_BANNER);
    }

    signal(SIGTERM, _on_signal);
    signal(SIGINT, _on_signal);

    server_start(&g_server, &args);

    // server stopped
    toml_free(parsed);

    return 0;
}
