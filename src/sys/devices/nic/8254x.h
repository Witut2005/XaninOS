
#include <stdint.h>
#include <sys/devices/pci/pci.h>

extern uint32_t i8254x_iobase_get(void);
extern pci_device* i8254x_pci_info_get(void);
extern uint8_t* i8254x_mac_get(void);
extern uint32_t i8254x_receive_descriptors_buffer_get(void);
extern uint32_t i8254x_transmit_descriptors_buffer_get(void);
extern void i8254x_packet_send(uint32_t address, uint16_t length);
extern void i8254x_init(uint8_t vector);
extern uint8_t* i8254x_packet_receive(void);
