

#include <stdint.h>
#include <sys/devices/apic/apic_registers.h>
#include <sys/devices/hda/disk.h>
#include <fs/xin.h>
#include <lib/libc/stdiox.h>

#define IVT_MEMORY_LOCATION NULL

struct Registers
{
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
};

typedef struct Registers Registers;
extern void __asm_registers_values_get(Registers*);

void outbIO(uint16_t port,uint8_t al)
{
    asm("outb dx,al" :: "d" (port), "a"(al));
}

void outwIO(uint16_t port,uint16_t ax)
{
    asm("out dx,ax" :: "d" (port), "a"(ax));
}

void outdIO(uint16_t port,uint32_t eax)
{
    asm("out dx,eax" :: "d" (port), "a"(eax));
}

void interrupt_disable(void)
{
    asm("cli");
}

void interrupt_enable(void)
{
    asm("sti");
}

uint8_t inbIO(uint16_t port)
{
    uint8_t ret;

    
    asm("in al, %1;"
        "mov %0, al;"
        : "=r"(ret)
        : "r"(port)
        :"al"
        );

    return ret;
    

}

uint16_t inwIO(uint16_t port)
{

    uint16_t ret;
    
    asm("in ax, %1;"
        "mov %0, ax;"
        : "=r"(ret)
        : "r"(port)
        :"ax"
        );

    return ret;

}


uint32_t indIO(uint16_t port)
{
    uint32_t ret;
    
    asm("in eax, %1;"
        "mov %0, eax;"
        : "=r"(ret)
        : "r"(port)
        :"eax"
        );
    return ret;

}

//https://wiki.osdev.org/Text_Mode_Cursor
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outbIO(0x3D4, 0x0A);
	outbIO(0x3D5, (inbIO(0x3D5) & 0xC0) | cursor_start);
 
	outbIO(0x3D4, 0x0B);
	outbIO(0x3D5, (inbIO(0x3D5) & 0xE0) | cursor_end);
}

void disable_cursor(void)
{
    outbIO(0x3D4, 0x0A);
	outbIO(0x3D5, 0x20);
}



//to move to cerain cell this cell cant be NULL
void update_cursor(uint32_t x, uint32_t y)
{
	uint32_t pos = (y * VGA_WIDTH) + x;
 
	outbIO(0x3D4, 0x0F);
	outbIO(0x3D5, (uint8_t) (pos & 0xFF));
	outbIO(0x3D4, 0x0E);
	outbIO(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

uint16_t get_cursor_position(void)
{
    uint16_t pos = 0;
    outbIO(0x3D4, 0x0F);
    pos |= inbIO(0x3D5);
    outbIO(0x3D4, 0x0E);
    pos |= ((uint16_t)inbIO(0x3D5)) << 8;
    return pos;
}

void eoi_send(void)
{

    if(*(uint32_t*)APIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER & (1 << 8))
    {
        *(uint32_t*)APIC_EOI_REGISTER = 0x0;
    }

    else
        asm("out 0x20, al" :: "a"(0x20));

}

void io_wait(void)
{
    asm("out 0x80, al" :: "a"(0x0));
}

void real_mode_enter(uint16_t segment, uint16_t offset, uint32_t return_address)
{

    disk_read(ATA_FIRST_BUS, ATA_MASTER, 1, 1, (uint16_t*)0x600);
    disk_read(ATA_FIRST_BUS, ATA_MASTER, 1000, 2, (uint16_t*)0); // load ivt from file

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

void real_mode_enter_no_return(uint16_t segment, uint16_t offset)
{
    
    disk_read(ATA_FIRST_BUS, ATA_MASTER, xin_find_entry("/enter_real_mode.bin")->first_sector, 1, (uint16_t*)0x600);
    disk_read(ATA_FIRST_BUS, ATA_MASTER, xin_find_entry("/ivt")->first_sector, 2, (uint16_t*)IVT_MEMORY_LOCATION); // load ivt from file

    asm (
        "mov ebx, %0\n\t"
        "and ebx, 0xFFFF\n\t"

        "mov edx, %1\n\t"
        "and edx, 0xFFFF\n\t"
        "jmp 0x600"
        :
        : "r" ((uint32_t)segment), "r" ((uint32_t)offset)

        );

    // void (*enter16)(void) = (void(*)(void))0x600;
    // enter16();
}


void rdmsr(uint32_t msr_id, uint32_t low, uint32_t high)
{
    asm("rdmsr" : "=a"(low), "=d"(high) : "c"(msr_id));
}


void wrmsr(uint32_t msr_id, uint32_t low, uint32_t high)
{
    asm("wrmsr" : "=a"(low), "=d"(high) : "c"(msr_id));
}


void pic_mask_set(uint16_t port, uint8_t value)
{

    if(port == 0x60)
        outbIO(0x60, value);
    else
        outbIO(0x20, value);

}

void pic_disable(void)
{
    outbIO(0xa1, 0xff);
    outbIO(0x21, 0xff);
}

void sse_enable(void)
{
    asm(
        "mov eax, cr0\n\t"
        "and ax, 0xFFFB\n\t"
        "or ax, 0x2\n\t"
        "mov cr0, eax\n\t"
        "mov eax, cr4\n\t"
        "mov ax, 3 << 9\n\t"
        "mov cr4, eax\n\t"
        "ret"
    );
}

void pic_mode_disable(void)
{ 
    outbIO(0x22, 0x70); //select interrupt mode control register
    outbIO(0x23, 0x1);  //force nmi and intr signals through the apic        
}

static inline void sti(void)
{
    asm("sti");
}

static inline void cli(void)
{
    asm("cli");
}

static inline void stc(void)
{
    asm("stc");
}

static inline void clc(void)
{
    asm("clc");
}

static inline void std(void)
{
    asm("std");
}

static inline void cld(void)
{
    asm("cld");
}

static inline void cmc(void)
{
    asm("cmc");
}

static inline void cpu_halt(void)
{
    asm("cli");
    asm("hlt");
}

void registers_values_get(Registers* Reg)
{
    // __asm_registers_values_get(Reg);
}
