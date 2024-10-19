#pragma once

#define ENETH_ADDRESS_IS_V4_MAPPED(address)                      \
    ((address)->host.v6[0] == 0 && (address)->host.v6[1] == 0 && \
     (address)->host.v6[2] == 0 && (address)->host.v6[3] == 0 && \
     (address)->host.v6[4] == 0 && (address)->host.v6[5] == 0xFFFF)
