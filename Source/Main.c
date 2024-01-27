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
    TOMLH_GET_INT(master_table, args.idle_timeout, "idle_timeout", 15000, 1);
    TOMLH_GET_STRING(master_table, args.mmdb_path, "mmdb_path", NULL, 1);
    TOMLH_GET_INT(
    master_table, args.max_connections_per_range, "max_connections_per_range", 10, 1);

    /* [httpd] */
    TOMLH_GET_TABLE(parsed, httpd_table, "httpd");
    TOMLH_GET_INT(httpd_table, args.httpd_port, "port", 0, 0);

    signal(SIGTERM, _on_signal);
    signal(SIGINT, _on_signal);

    server_start(&g_server, &args);

    // server stopped
    free(args.mmdb_path);
    toml_free(parsed);

    return 0;
}
