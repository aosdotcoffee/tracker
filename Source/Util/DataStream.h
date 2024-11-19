// Copyright CircumScriptor and DarkNeutrino 2021
#pragma once
#include <stddef.h>
#include <stdint.h>

#ifndef NDEBUG
    #include <stdio.h> // IWYU pragma: keep
    #define LOG_OOB_ACCESS(stream, size) \
        fputs("\x1b[1;31mERROR:\x1b[0;1m " \
              "Out-of-bounds access on DataStream object\x1b[0;0m\n", \
              stderr); \
        fprintf(stderr, \
                "       \x1b[1m" \
                "Trying to access offset [%zu-%zu] on stream of size [%zu]" \
                "\x1b[0m\n", stream->pos, stream->pos + size, stream->length)
#else
    #define LOG_OOB_ACCESS(stream, size)
#endif

#define ACCESS_CHECK(stream, size) \
    if (stream->pos + size > stream->length) { \
        LOG_OOB_ACCESS(stream, size);          \
        return 0;                              \
    }

#define ACCESS_CHECK_N(stream, size)           \
    if (stream->pos + size > stream->length) { \
        LOG_OOB_ACCESS(stream, size);          \
        return;                                \
    }

typedef struct stream
{
    uint8_t* data;
    size_t length;
    size_t pos;
} stream_t;

void stream_create(stream_t* stream, size_t length);
void stream_free(stream_t* stream);
size_t stream_left(const stream_t* stream);
void stream_skip(stream_t* stream, size_t skip);
uint8_t stream_read_u8(stream_t* stream);
uint16_t stream_read_u16(stream_t* stream);
uint32_t stream_read_u32(stream_t* stream);
float stream_read_f(stream_t* stream);
void stream_read_array(stream_t* stream, void* output, size_t length);
size_t stream_read_string(stream_t* stream, char* output, size_t max_length);
void stream_write_u8(stream_t* stream, uint8_t value);
void stream_write_u16(stream_t* stream, uint16_t value);
void stream_write_u32(stream_t* stream, uint32_t value);
void stream_write_f(stream_t* stream, float value);
void stream_write_array(stream_t* stream, const void* array, size_t length);
void stream_write_string(stream_t* stream, const char* string);
