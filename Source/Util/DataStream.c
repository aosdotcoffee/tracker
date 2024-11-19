// Copyright CircumScriptor and DarkNeutrino 2021
#include <Util/DataStream.h>
#include <Util/Mem.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void stream_create(stream_t* stream, size_t length)
{
    stream->data = make(uint8_t, length);
    stream->length = length;
    stream->pos = 0;
}

void stream_free(stream_t* stream)
{
    if (stream->data != NULL) {
        free(stream->data);
    }
}

size_t stream_left(const stream_t* stream)
{
    return (stream->pos < stream->length) ? stream->length - stream->pos : 0;
}

void stream_skip(stream_t* stream, size_t skip)
{
    stream->pos = (stream->pos + skip < stream->length) ? stream->pos + skip
                                                        : stream->length;
}

uint8_t stream_read_u8(stream_t* stream)
{
    ACCESS_CHECK(stream, 1);
    return stream->data[stream->pos++];
}

uint16_t stream_read_u16(stream_t* stream)
{
    ACCESS_CHECK(stream, 2);
    uint16_t value = 0;
    value |= ((uint16_t) stream->data[stream->pos++]);
    value |= ((uint16_t) stream->data[stream->pos++]) << 8;
    return value;
}

uint32_t stream_read_u32(stream_t* stream)
{
    ACCESS_CHECK(stream, 4);
    uint32_t value = 0;
    value |= ((uint32_t) stream->data[stream->pos++]);
    value |= ((uint32_t) stream->data[stream->pos++]) << 8;
    value |= ((uint32_t) stream->data[stream->pos++]) << 16;
    value |= ((uint32_t) stream->data[stream->pos++]) << 24;
    return value;
}

float stream_read_f(stream_t* stream)
{
    union
    {
        float f;
        uint32_t v;
    } u;
    u.v = stream_read_u32(stream);
    return u.f;
}

void stream_read_array(stream_t* stream, void* output, size_t length)
{
    ACCESS_CHECK_N(stream, length);
    memcpy(output, stream->data + stream->pos, length); // Replace me
    stream->pos += length;
}

size_t stream_read_string(stream_t* stream, char* output, size_t max_length)
{
    size_t i = 0;

    while (stream_left(stream) != 0 && i < max_length - 1) {
        char read = stream_read_u8(stream);
        if (read == '\0') {
            break;
        }

        output[i++] = read;
    }

    output[i] = '\0';

    return i;
}

void stream_write_array(stream_t* stream, const void* array, size_t length)
{
    ACCESS_CHECK_N(stream, length);
    memcpy(stream->data + stream->pos, array, length); // Replace me
    stream->pos += length;
}

void stream_write_u8(stream_t* stream, uint8_t value)
{
    ACCESS_CHECK_N(stream, 1);
    stream->data[stream->pos++] = value;
}

void stream_write_u16(stream_t* stream, uint16_t value)
{
    ACCESS_CHECK_N(stream, 2);
    stream->data[stream->pos++] = (uint8_t) value;
    stream->data[stream->pos++] = (uint8_t) (value >> 8);
}

void stream_write_u32(stream_t* stream, uint32_t value)
{
    ACCESS_CHECK_N(stream, 4);
    stream->data[stream->pos++] = (uint8_t) value;
    stream->data[stream->pos++] = (uint8_t) (value >> 8);
    stream->data[stream->pos++] = (uint8_t) (value >> 16);
    stream->data[stream->pos++] = (uint8_t) (value >> 24);
}

void stream_write_f(stream_t* stream, float value)
{
    union
    {
        float f;
        uint32_t v;
    } u;
    u.f = value;
    stream_write_u32(stream, u.v);
}
