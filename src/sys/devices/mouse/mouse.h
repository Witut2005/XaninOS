
#include <stdint.h>


extern uint8_t mouse_cycle;     //unsigned char
extern uint8_t mouse_uint8_t[3];    //signed char
extern uint8_t mouse_x;         //signed char
extern uint8_t mouse_y;         //signed char

// extern void mouse_handler2(void); //struct regs *a_r (not used but just there)
extern void my_mouse_handler(void);
void mouse_wait(uint8_t a_type); //unsigned char
void mouse_write(uint8_t a_write); //unsigned char
uint8_t mouse_read(void);
void mouse_install(void);