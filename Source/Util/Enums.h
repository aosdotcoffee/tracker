// Copyright DarkNeutrino 2021
#ifndef ENUMS_H
#define ENUMS_H

#define NANO_IN_SECOND 1000000000
#define NANO_IN_MILLI  1000000
#define NANO_IN_MINUTE 60000000000

#define MAX_SERVER_NAME_LENGTH 32
#define MAX_GAMEMODE_LENGTH    8
#define MAX_MAP_LENGTH         20

typedef enum {
    STATE_READY,
    STATE_HAS_SERVER,
    STATE_DISCONNECTED,
} state_t;

typedef enum {
    VERSION_0_75 = 31,
} protocol_version_t;

typedef enum {
    REASON_BANNED                 = 1,
    REASON_IP_LIMIT_EXCEEDED      = 2,
    REASON_WRONG_PROTOCOL_VERSION = 3,
    REASON_IDLE_FOR_TOO_LONG      = 4,
} disconnected_reason_t;

#endif /* ENUMS_H */
