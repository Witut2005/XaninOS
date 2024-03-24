
#include <stdint.h>
#include <fs/xin.h>
#include <sys/devices/hda/disk.h>
#include <lib/libc/xanin_state.h>
#include <lib/cpu/headers/cpu_state_info.h>

#define XANIN16_LOADED_PROGRAM_ADDRESS_SEGMENT 0x0
#define XANIN16_LOADED_PROGRAM_ADDRESS_OFFSET 0x400

// void real_mode_fswitch(uint16_t segment, uint16_t offset)
__STATUS real_mode_fswitch(char* file_name)
{

    uint32_t segment = XANIN16_LOADED_PROGRAM_ADDRESS_SEGMENT;
    uint32_t offset = XANIN16_LOADED_PROGRAM_ADDRESS_OFFSET;


    XinEntry* File = fopen(file_name, "r");
    // XinEntry* RealModeReturn = fopen("/fast_real_mode_return.bin", "r");

    if ((File == NULL))// || (RealModeReturn == NULL))
    {
        xprintf("%zError", OUTPUT_COLOR_ERROR_SET);
        return XANIN_ERROR;
    }

    // __disk_sectors_read(ATA_FIRST_BUS, ATA_MASTER, RealModeReturn->first_sector, 1, (uint16_t*)0x600);

    fread(File, (segment << 4) | offset, SECTOR_SIZE); // 512Bytes should be enough

    Registers Regs;
    __asm_registers_values_get(&Regs);

    asm(
        "mov ebx, %0\n\t"
        "and ebx, 0xFFFF\n\t"

        "mov edx, %1\n\t"
        "and edx, 0xFFFF\n\t"
        "jmp 0x600"
        :
    : "r" ((uint32_t)segment), "r" ((uint32_t)offset)

        );
    return XANIN_OK;

}