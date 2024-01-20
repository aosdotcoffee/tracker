#include <Server/Server.h>
#include <Server/Structs/ServerStruct.h>
#include <Server/Structs/StartStruct.h>
#include <Util/TOMLHelpers.h>
#include <tomlc99/toml.h>

const char* TRACKER_BANNER = "\n"
        "                \x1b[1;32m▐           ▌        \n"
        "                \x1b[1;32m▜▀ ▙▀▖▝▀▖▞▀▖▌▗▘▞▀▖▙▀▖\n"
        "                \x1b[1;36m▐ ▖▌  ▞▀▌▌ ▖▛▚ ▛▀ ▌  \n"
        "                \x1b[1;36m ▀ ▘  ▝▀▘▝▀ ▘ ▘▝▀▘▘  \n"
        "\x1b[0;0m";

int main(void)
{
    server_args args;

    toml_table_t* parsed;
    toml_table_t* tracker_table;
    toml_table_t* enet_table;
    TOMLH_READ_FROM_FILE(parsed, "config.toml");

    /* [tracker] */
    TOMLH_GET_TABLE(parsed, tracker_table, "tracker");
    TOMLH_GET_INT(tracker_table, args.port, "port", DEFAULT_SERVER_PORT, 0);

    /* [enet] */
    TOMLH_GET_TABLE(parsed, enet_table, "enet");
    TOMLH_GET_INT(enet_table, args.max_connections, "max_connections", 128, 1);
    TOMLH_GET_INT(enet_table, args.max_bandwidth_in, "max_bandwidth_in", 0, 1);
    TOMLH_GET_INT(enet_table, args.max_bandwidth_out, "max_bandwidth_out", 0, 1);
    TOMLH_GET_INT(enet_table, args.channels, "channels", 2, 1);

    server_t server = {0};

    if (isatty(1)) {
        puts(TRACKER_BANNER);
    }

    server_start(&server, &args);

    toml_free(parsed);

    return 0;
}
