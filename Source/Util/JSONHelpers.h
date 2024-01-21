#ifndef JSONHELPERS_H
#define JSONHELPERS_H

#define JSON_SET(type, obj, key, val)                        \
    {                                                        \
        struct json_object* v = json_object_new_##type(val); \
        json_object_object_add(obj, key, v);                 \
    }

#define JSON_SET_STRING(obj, key, str) JSON_SET(string, obj, key, str)
#define JSON_SET_INT(obj, key, num)    JSON_SET(int, obj, key, num)

#endif
