
#include <stdint.h>
#include <fs/xin.h>
#include <sys/devices/hda/disk.h>
#include <lib/libc/xanin_state.h>
#include <lib/cpu/headers/cpu_state_info.h>

void real_mode_fswitch(uint16_t segment, uint16_t offset)
{
    segment = 0x0;
    offset = 0x400;

    // XinEntry* real_mode_enter = fopen("/fast_real_mode_enter.bin", "r");
    // disk_read(ATA_FIRST_BUS, ATA_MASTER, real_mode_enter->first_sector, 1, (uint16_t*)0x600);

    XinEntry* real_mode_return = fopen("/fast_real_mode_return.bin", "r");
    disk_read(ATA_FIRST_BUS, ATA_MASTER, real_mode_return->first_sector, 1, (uint16_t*)0x600);
    
    Registers Regs;
    __asm_registers_values_get(&Regs);
    
    asm (
    "mov ebx, %0\n\t"
    "and ebx, 0xFFFF\n\t"

    "mov edx, %1\n\t"
    "and edx, 0xFFFF\n\t"
    "jmp 0x600"
    :
    : "r" ((uint32_t)segment), "r" ((uint32_t)offset)

    );
}