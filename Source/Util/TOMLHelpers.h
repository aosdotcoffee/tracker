#pragma once
#include <Util/Log.h>
#include <tomlc99/toml.h> // IWYU pragma: keep

#define TOMLH_READ_FROM_FILE(toml, path)                                       \
    {                                                                          \
        char toml##_error[200];                                                \
        FILE* toml##_fp = fopen(path, "re");                                   \
        if (!toml##_fp) {                                                      \
            LOG_ERROR("Cannot find TOML file %s", path);                       \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
        toml = toml_parse_file(toml##_fp, toml##_error, sizeof(toml##_error)); \
        fclose(toml##_fp);                                                     \
        if (!toml) {                                                           \
            LOG_ERROR("Failed to read %s.", path);                             \
            LOG_ERROR("%s", toml##_error);                                     \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
    }

#define TOMLH_GET_TABLE(toml, table, table_name)             \
    {                                                        \
        table = toml_table_in(toml, table_name);             \
        if (!table) {                                        \
            LOG_ERROR("Cannot find table [%s]", table_name); \
            exit(EXIT_FAILURE);                              \
        }                                                    \
    }

#define TOMLH_GET_VAR(type, table, out, name, cast, fallback, optional) \
    {                                                                   \
        toml_datum_t _tomlh_datum = toml_##type##_in(table, name);      \
        if (!_tomlh_datum.ok) {                                         \
            if (!optional) {                                            \
                LOG_ERROR("Cannot find [" #type "]" name " in config"); \
                exit(EXIT_FAILURE);                                     \
            }                                                           \
            out = fallback;                                             \
        } else {                                                        \
            out = cast;                                                 \
        }                                                               \
    }

#define TOMLH_GET_INT(table, out, name, fallback, optional) \
    TOMLH_GET_VAR(int, table, out, name, (int) _tomlh_datum.u.i, fallback, optional);

#define TOMLH_GET_INT_ARRAY(table, out, name, size, fallback, optional)       \
    {                                                                         \
        toml_array_t* _tomlh_array = toml_array_in(table, name);              \
        size_t _tomlh_array_size = 0;                                         \
        if (_tomlh_array) {                                                   \
            _tomlh_array_size = toml_array_nelem(_tomlh_array);               \
        }                                                                     \
        for (size_t i = 0; i < size; i++) {                                   \
            if (i < _tomlh_array_size) {                                      \
                toml_datum_t _tomlh_array_elm = toml_int_at(_tomlh_array, i); \
                if (!_tomlh_array_elm.ok) {                                   \
                    LOG_ERROR("Failed to read " #name "[%zu] from TOML", i);  \
                    exit(EXIT_FAILURE);                                       \
                }                                                             \
                out[i] = (int) _tomlh_array_elm.u.i;                          \
            } else {                                                          \
                if (!optional) {                                              \
                    LOG_ERROR("Config option " name "[%i] not found", i);     \
                    exit(EXIT_FAILURE);                                       \
                }                                                             \
                out[i] = fallback[i];                                         \
            }                                                                 \
        }                                                                     \
    }

#define TOMLH_GET_STRING_ARRAY_AS_DL(table, out, size_out, name, optional)     \
    {                                                                          \
        toml_array_t* _tomlh_array = toml_array_in(table, name);               \
        if (!_tomlh_array) {                                                   \
            if (optional) {                                                    \
                size_out = 0;                                                  \
                goto _tomlh_done_reading;                                      \
            } else {                                                           \
                LOG_ERROR("Failed to read string array '%s' from TOML", name); \
                exit(EXIT_FAILURE);                                            \
            }                                                                  \
        }                                                                      \
        size_out = toml_array_nelem(_tomlh_array);                             \
        for (size_t i = 0; i < size_out; i++) {                                \
            toml_datum_t _tomlh_array_elm = toml_string_at(_tomlh_array, i);   \
            if (!_tomlh_array_elm.ok) {                                        \
                LOG_ERROR("Failed to read " #name "[%zu] from TOML", i);       \
                exit(EXIT_FAILURE);                                            \
            }                                                                  \
            string_node_t* _tomlh_string = make(string_node_t);                \
            _tomlh_string->string = (char*) _tomlh_array_elm.u.s;              \
            DL_APPEND(out, _tomlh_string);                                     \
        }                                                                      \
    }                                                                          \
    _tomlh_done_reading:

#define TOMLH_GET_STRING(table, out, name, fallback, optional) \
    TOMLH_GET_VAR(string, table, out, name, _tomlh_datum.u.s, fallback, optional);

#define TOMLH_GET_BOOL(table, out, name, fallback, optional) \
    TOMLH_GET_VAR(bool, table, out, name, (int) _tomlh_datum.u.b, fallback, optional);
