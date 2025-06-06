#include <Server/Packets/MajorUpdate.h>
#include <Tests/TestUtil.h>
#include <Util/DataStream.h>
#include <Util/Mem.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

static void _expand_stream(stream_t* stream) {
    stream->pos = 0;
    stream->length = 128;
}

static void _test_normal_packet(stream_t* stream) {
    uint16_t max_players = 32;
    uint16_t port = 32887;
    const char* name = "aloha.pk tower of babel";
    const char* gamemode = "babel";
    const char* map = "cheesus";

    /* create packet */
    stream_write_u8(stream, max_players);
    stream_write_u16(stream, port);
    stream_write_string(stream, name);
    stream_write_string(stream, gamemode);
    stream_write_string(stream, map);
    stream_shrinkwrap(stream);

    auto packet = parse_v31_major_update_packet(NULL, stream);
    assert(packet);
    assert(packet->max_players == max_players);
    assert(packet->port == port);
    assert_str_equals(packet->name, name);
    assert_str_equals(packet->gamemode, gamemode);
    assert_str_equals(packet->map, map);
}

static void _test_short_packet(stream_t* stream) {
    uint16_t max_players = 8;
    uint16_t port = 32883;
    const char* name = "test server";

    /* create packet */
    stream_write_u8(stream, max_players);
    stream_write_u16(stream, port);
    stream_write_string(stream, name);
    stream_shrinkwrap(stream);

    auto packet = parse_v31_major_update_packet(NULL, stream);
    assert(packet);
    assert(packet->max_players == max_players);
    assert(packet->port == port);
    assert_str_equals(packet->name, name);
    assert_str_empty(packet->gamemode);
    assert_str_empty(packet->map);
}

static void _test_empty_stream(void) {
    stream_t stream;
    stream_create(&stream, 1);
    stream.length = 0;

    assert_null(parse_v31_major_update_packet(NULL, &stream));
    stream_free(&stream);
}

static void _test_invalid_short(void) {
    stream_t stream;
    stream_create(&stream, 4);
    stream_write_u32(&stream, 0x00123456);
    stream.pos = 0;
    stream.length = 3;

    assert_null(parse_v31_major_update_packet(NULL, &stream));
    stream_free(&stream);
}

static void _test_invalid_long(stream_t* stream) {
    uint16_t max_players = 32;
    uint16_t port = 3;
    const char* name = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbb";
    const char* gamemode = "veryverylonggamemode";
    const char* map = "alsoaverylongmapyeahhhhhhh";

    /* create packet */
    stream_write_u8(stream, max_players);
    stream_write_u16(stream, port);
    stream_write_string(stream, name);
    stream_write_string(stream, gamemode);
    stream_write_string(stream, map);
    stream_shrinkwrap(stream);

    auto packet = parse_v31_major_update_packet(NULL, stream);
    assert(packet);
    assert(packet->max_players == max_players);
    assert(packet->port == port);
    assert_str_equals(packet->name, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    assert_str_equals(packet->gamemode, "veryver");
    assert_str_equals(packet->map, "alsoaverylongmapyeah");
}

static void _test_all_null(stream_t* stream) {
    stream_zero(stream);

    auto packet = parse_v31_major_update_packet(NULL, stream);
    assert_null(packet);
}

int main(void) {
    stream_t stream;
    stream_create(&stream, 128);

    _test_normal_packet(&stream);
    _expand_stream(&stream);

    _test_short_packet(&stream);
    _expand_stream(&stream);

    _test_empty_stream();
    _test_invalid_short();

    _test_invalid_long(&stream);
    _expand_stream(&stream);

    _test_all_null(&stream);
    _expand_stream(&stream);

    stream_free(&stream);
}
