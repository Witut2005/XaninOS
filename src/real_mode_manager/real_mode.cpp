#include <stdint.h>

class real_mode_manager
{
    public:
        void enter(uint16_t address)
        {
            asm("jmp $");
        }
};

void _start(void)
{
    asm("jmp $");
}
