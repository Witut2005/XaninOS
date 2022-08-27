#pragma once

#include <stdint.h>

extern uint8_t* mac_get(void);
extern void send_packet(uint32_t address, uint16_t lenght);
extern uint8_t* receive_packet(void);
