
#include <stdint.h>

#define SECTOR_SIZE 0x200

enum ATA_REGISTERS
{
      
    ATA_MASTER = 0x1,
    ATA_SLAVE = 0x0,

    ATA_FIRST_BUS = 0x1F0,
    ATA_SECONDARY_BUS = 0x170,
        

    ATA_DATA_REGISTER = 0x0,

    ATA_ERROR_REGISTER = 0x1,
    ATA_FEATURES_REGISTER = 0x1,

    ATA_SECTOR_COUNT_REGISTER = 0x2,

    ATA_SECTOR_NUMBER_LOW = 0x3,
    ATA_SECTOR_NUMBER_MID = 0x4,
    ATA_SECTOR_NUMBER_HIGH = 0x5,

    ATA_DRIVE_REGISTER = 0x6,
    ATA_STATUS_REGISTER = 0x7,
    ATA_COMMAND_REGISTER = 0x7,

    ATA_CONTROL_REGISTER = 0x206

};



enum ATA_COMMANDS
{

    ATA_IDENTIFY = 0xEC,
    ATA_READ = 0x20,
    ATA_EXTENDED_READ = 0x24,
    ATA_WRITE = 0x30,
    ATA_EXTENDED_WRITE = 0x34,
    ATA_FLUSH = 0xE7

};

uint16_t* print_ptr = (uint16_t*)0xb8000;
uint32_t y = 0;
uint32_t x = 0;


void print(const char* aha)
{
    while(*aha != '\0')
    {
        print_ptr[y * 80 + x] = *aha | (0xF0 << 8);
        x++;
        aha++;
    }

    x = 0;
    y++;

}

void print_decimal(uint32_t value)
{
    *print_ptr = value % 10 + '0' + (0xF0 << 8);
    x = 0;
    y++;
}

void print_hex(uint32_t value)
{

    if(!value)
        print_ptr[x + y * 80] = '0' | (0xF0 << 8);

    while(value)
    {
        if(value % 16 <= 9)
            print_ptr[x + y * 80] = value % 16 + '0' | (0xF0 << 8);
        else 
            print_ptr[x + y * 80] = value % 16 - 0xa + 'A' | (0xF0 << 8);
        x++;
        value = value / 16;
    }
    x = 0;
    y++;
}


static void io_wait(void)
{
    asm("out 0x80, al" :: "a"(0x0));
}

static void outbIO(uint16_t port,uint8_t al)
{
    asm("outb dx,al" :: "d" (port), "a"(al));
}

static inline void outwIO(uint16_t port,uint16_t ax)
{
    asm("out dx,ax" :: "d" (port), "a"(ax));
}

static inline void outdIO(uint16_t port,uint32_t eax)
{
    asm("out dx,eax" :: "d" (port), "a"(eax));
}

static inline uint8_t inbIO(uint16_t port)
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

static inline uint16_t inwIO(uint16_t port)
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


static inline uint32_t indIO(uint16_t port)
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



void init_disk(uint16_t base, uint8_t master)
{

    uint8_t disk_status;

    
    outbIO(base + ATA_DRIVE_REGISTER, master == ATA_MASTER ? 0xA0 : 0xB0);
    outbIO(base + ATA_CONTROL_REGISTER, 0x0); 

    outbIO(base + ATA_DRIVE_REGISTER, master == ATA_MASTER ? 0xA0 : 0xB0);
    disk_status = inbIO(ATA_COMMAND_REGISTER);

    if(disk_status == 0xFF)
    {
        print("NO DISK\n");
        return;
    }

    outbIO(base + ATA_DRIVE_REGISTER, master == ATA_MASTER ? 0xA0 : 0xB0);
    outbIO(base + ATA_SECTOR_COUNT_REGISTER, 0x0);
    outbIO(base + ATA_SECTOR_NUMBER_LOW, 0x0);
    outbIO(base + ATA_SECTOR_NUMBER_MID, 0x0);
    outbIO(base + ATA_SECTOR_NUMBER_HIGH, 0x0);
    
    outbIO(base + ATA_COMMAND_REGISTER, ATA_IDENTIFY);
	 
    disk_status = inbIO(base + ATA_COMMAND_REGISTER);
	  
    while(disk_status & 0x81 == 80)
		    disk_status = inbIO(base + ATA_COMMAND_REGISTER);  
    
    for(int i = 0; i < 256; i++)
        inwIO(base + ATA_DATA_REGISTER);    

}



void disk_read(uint16_t base, uint8_t master, uint32_t sector_number, 
                                uint16_t how_many_sectors, uint16_t* where)
{


    uint8_t disk_status;

    outbIO(base + ATA_DRIVE_REGISTER, ((master == ATA_MASTER ? 0x40 : 0x50)));
    outbIO(base + ATA_SECTOR_COUNT_REGISTER, how_many_sectors >> 8);
    
    /* lba4 - lba6 */
    outbIO(base + ATA_SECTOR_NUMBER_LOW, 0);
	outbIO(base + ATA_SECTOR_NUMBER_MID, 0);
	outbIO(base + ATA_SECTOR_NUMBER_HIGH, 0);

    outbIO(base + ATA_SECTOR_COUNT_REGISTER, how_many_sectors & 0xFF);
	  
    /* lba1 - lba3 */
    outbIO(base + ATA_SECTOR_NUMBER_LOW, sector_number & 0xFF);
	outbIO(base + ATA_SECTOR_NUMBER_MID, (sector_number >> 8) & 0xFF);
	outbIO(base + ATA_SECTOR_NUMBER_HIGH, (sector_number >> 16) & 0xFF);
	
	outbIO(base + ATA_COMMAND_REGISTER, ATA_EXTENDED_READ);

    disk_status = inbIO(base + ATA_STATUS_REGISTER);

    while((disk_status & 0x81) == 0x80)
        disk_status = inbIO(base + ATA_STATUS_REGISTER);


    for(int i = 0; i < 4; i++)
        inbIO(base + ATA_STATUS_REGISTER);

    for(int i = 0; i < 4; i++)
        io_wait();

    for(int i = 0; i < how_many_sectors; i++)
    {
        for(int j = 0; j < 256; j++)
        {
            uint16_t readed_data = inwIO(base + ATA_DATA_REGISTER);
        
      	    *where = readed_data;
      	    where++;
        
        }
    }

}


enum ELF_FIELD
{
    X86_E_MACHINE = 0x3,
    ELF_HEADER_SIZE = 0x34,
    PT_LOAD = 0x1
};




void elf_load(void)
{
    uint8_t* data = (uint8_t*)(0x20200 + (15 * SECTOR_SIZE));
    print(data);
    
    uint8_t* write_to_memory;
    uint8_t* read_from_file;
    uint32_t file_base = (uint32_t)data;
    uint32_t begin_of_code = (uint32_t)data;

    uint16_t phnum = *(uint16_t*)((uint8_t*)data + 0x2C);

    uint32_t p_offset;      //offset in file image
    uint32_t p_vaddr;       //virtual address of the segment in memory
    uint32_t p_filesz;      //size in bytes of segment in file image
    uint32_t p_memsz;       //size in bytes of segment in memory
    uint32_t entry_point = *(uint32_t*)((uint8_t*)data + 0x18);


    data += 0x34;

    // print("kernel loaded");

    print_decimal(phnum);

    while(phnum)
    {

        print("loop iterator");
        
        if(*(uint32_t*)data == PT_LOAD)
        {
            print("PT LOAD FINDED");

            p_offset = *(uint32_t*)(data + 0x4);
            p_offset = p_offset + begin_of_code;
            p_vaddr  = *(uint32_t*)(data + 0x8);
            p_filesz = *(uint32_t*)(data + 0x10);
            p_memsz  = *(uint32_t*)(data + 0x14);

            print_hex(p_offset);
            print_hex(p_vaddr);

            // while(1);

            read_from_file = (uint8_t*)p_offset;
            write_to_memory = (uint8_t*)p_vaddr;

        
            for(int i = 0; i < p_filesz; i++, write_to_memory++, read_from_file++)
                *write_to_memory = *read_from_file;
            
        }

        data += 0x20;
        phnum--;

    }
    
    print("kernel loaded");

    void(*kernel)(void) = entry_point;

    kernel();

}


void _start(void)
{
    init_disk(ATA_FIRST_BUS, ATA_MASTER);    
        
    for(int i = 0; i < 1600; i++) // Weird reboot, shutdown bug (probably cpu want to make fun of me (ivt))
        disk_read(ATA_FIRST_BUS, ATA_MASTER, 0xA9 + i, 1, (uint16_t*)(0x20200 + ((15 + i)* SECTOR_SIZE)));

    elf_load();
}
