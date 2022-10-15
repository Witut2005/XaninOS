
/* macros */

#define PIC1_COMMAND_REG 0x20
#define PIC1_STATUS_REG 0x20

#define PIC1_DATA_REG 0x21    
#define PIC1_MASK_REG 0x21

#define PIC2_COMMAND 0xA0
#define PIC2_STATUS_REG 0xA0

#define PIC2_DATA 0xA1    
#define PIC_MASK_REG 0xA1

#define KEYBOARD_ENCODER 0x60
#define ONBOARD_KEY_CONTROLLER 0x64
#define KEYBOARD_STATUS_REG 0x64

#define KEYBOARD_DATA_REG 0x60


#define VGA_TEXT_MEMORY 0xb8000

#include <xaninApps/register_dump.c>

/* KEYBOARD HANDLER DATA */
uint8_t keyStatus;

int reg_dump(void);
