// Copyright DarkNeutrino 2021
#pragma once
#include <stddef.h>
#include <stdint.h>
#include <string.h>

enum : uint64_t {
    NANO_IN_SECOND = 1000000000,
    NANO_IN_MILLI = 1000000,
    NANO_IN_MINUTE = 60000000000,
};

enum : uint16_t {
    MAX_SERVER_NAME_LENGTH = 32,
    MAX_GAMEMODE_LENGTH = 8,
    MAX_MAP_LENGTH = 21,
};

typedef enum {
    STATE_READY,
    STATE_HAS_SERVER,
    STATE_DISCONNECTED,
} state_t;

typedef enum {
    VERSION_054 = 17,
    VERSION_060 = 23,
    VERSION_075 = 31,
    VERSION_076 = 32,
    // "BetterGrandySpades"
    VERSION_BGS_01 = 133,
} protocol_version_t;

static inline bool protocol_version_is_valid(int version) {
    switch ((protocol_version_t) version) {
    case VERSION_054:
    case VERSION_060:
    case VERSION_075:
    case VERSION_076:
    case VERSION_BGS_01:
        return true;
    }

    return false;
}

static inline const char* protocol_version_to_string(protocol_version_t version) {
    static char version_string[7];

    if (!protocol_version_is_valid(version)) {
        return strcpy(version_string, "0.00");
    }

    switch (version) {
        case VERSION_054:
            return strcpy(version_string, "0.54");
        case VERSION_060:
            return strcpy(version_string, "0.60");
        case VERSION_075:
            return strcpy(version_string, "0.75");
        case VERSION_076:
            return strcpy(version_string, "0.76");
        case VERSION_BGS_01:
            return strcpy(version_string, "bgs0.1");
    }

#ifdef unreachable
    unreachable();
#else
    return NULL;
#endif
}

typedef enum {
    REASON_BANNED = 1,
    REASON_IP_LIMIT_EXCEEDED = 2,
    REASON_WRONG_PROTOCOL_VERSION = 3,
    REASON_IDLE_FOR_TOO_LONG = 4,
    REASON_SERVER_SHUTTING_DOWN = 5,
} disconnected_reason_t;
