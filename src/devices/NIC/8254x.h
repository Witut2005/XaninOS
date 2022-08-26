
#include <stdint.h>

extern uint32_t i8254x_iobase_get(void);
extern void i8254x_packet_send(uint32_t address, uint16_t length);
extern uint32_t* i8254x_class_return(void);
