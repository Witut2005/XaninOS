
#pragma once

#include <stdint.h>
#include <network_protocols/icmp/icmp.h>

extern uint16_t endian_switch(uint16_t nb);
extern uint32_t endian_switch(uint32_t nb);
extern uint16_t endian_switch(int16_t nb);
extern uint32_t endian_switch(int32_t nb);
extern IcmpPacket* endian_switch(IcmpPacket* IcmpData);

#define BIG_ENDIAN(nb) endian_switch(nb)
#define LITTLE_ENDIAN(nb) endian_switch(nb)

