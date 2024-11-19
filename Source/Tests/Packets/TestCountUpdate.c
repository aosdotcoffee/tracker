#include <Server/Packets/CountUpdate.h>
#include <Tests/TestUtil.h>
#include <Util/DataStream.h>

int main(void) {
    stream_t stream;
    stream_create(&stream, 1);
    stream_write_u8(&stream, 25);
    stream_shrinkwrap(&stream);

    {
        auto packet = parse_count_update_packet(NULL, &stream);
        assert(packet);
        assert(packet->current_players == 25);
    }

    {
        stream.length = 0;
        auto packet = parse_count_update_packet(NULL, &stream);
        assert_null(packet);
    }

    stream_free(&stream);
}
