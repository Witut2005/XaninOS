
#pragma once

#include <stdint.h>
#include <network_protocols/icmp/icmp.h>

extern uint16_t endian_switch(uint16_t nb);
extern uint32_t endian_switch(uint32_t nb);
extern IcmpPacket* endian_switch(IcmpPacket* IcmpData);


