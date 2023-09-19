
#include <stdint.h>
#include <lib/elf/elf.h>

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

uint32_t strlen(const char* a)
{

    if(a == NULL)
        return 0;

    uint32_t length = 0;

    for(const char* i = a; *i != '\0' ;i++)
        length++;

    return length;
}


bool bstrcmp(char* a, const char* b)
{

    uint32_t lengtha = strlen(a);
    uint32_t lengthb = strlen(b);
    
    if(lengtha != lengthb)
        return 0;

    for(char* i = a; *i != '\0' ;i++)
    {
        if(*a != *b)
            return 0;
        a++;
        b++;
    }

    
    return 1;		
}

char* reverse_string(char* str)
{
    char buf;
    char* end = str + strlen(str) - 1;

    for(char* begin = str; (uint32_t)begin < (uint32_t)end; begin++, end--)
    {
        char buf = *begin;

        *begin = *end;
        *end = buf;
    }
    return str;
}

char* int_to_str(int x, char* buf)
{
    int i = 0;

    if(!x)
    {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }

    for(i = 0; x != 0; i++)
    {
        buf[i] = (x % 10) + '0';
        x = x / 10;
    }

    buf[i] = '\0';
    buf = reverse_string(buf);
    return buf;
}

char* int_to_hex_str(uint32_t x, char* buf)
{
    char hex_values[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

    int i = 0;

    if(!x)
    {
        buf[0] = '0';
        buf[1] = '0';
        buf[2] = '\0';
        return buf;
    }
    
    for(i = 0; x != 0; i++)
    {
        buf[i] = hex_values[x % 16]; 
        x = x / 16;
    }

    buf = reverse_string(buf);
    return buf;
}

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
    char buf[20] = {0};
    print(int_to_str(value, buf));
    x = 0;
    y++;
}

void print_hex(uint32_t value)
{
    char buf[20] = {0};
    print(int_to_hex_str(value, buf));

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

void vga_screen_buffer_clear(void)
{
    uint16_t* screen_cleaner = (uint16_t*)0xb8000;
    for(int i = 0; i < (80 * 25); i++)
    {
        *screen_cleaner = '\0';
        screen_cleaner++;
    }
    y = x = 0;
}

char* elf_section_header_string_table_address_get(ElfAutoHeader* Header)
{
    ElfSectionHeaderAuto* SectionHeaders = (ElfSectionHeaderAuto*)(Header->e_shoff + (uint32_t)Header);
    return (char*)(SectionHeaders[Header->e_shstrndx].sh_offset + (uint32_t)Header);
}

// void elf_sections_load(ElfAutoHeader* Header) // NOT FINISHED
// {
//     uint8_t* address_base = (uint8_t*)Header;
//     ElfSectionHeaderAuto* SectionHeaders = (ElfSectionHeaderAuto*)(Header->e_shoff + address_base);
// }

bool elf_load_given_section(ElfAutoHeader* Header, const char* section_name)
{
    char* elf_section_names = elf_section_header_string_table_address_get(Header);
    ElfSectionHeaderAuto* Sections = (ElfSectionHeaderAuto*)(Header->e_shoff + (uint32_t)Header);

    vga_screen_buffer_clear();

    for(int i = 0; i < Header->e_shnum; i++)
    {
        if(bstrcmp(section_name, &elf_section_names[Sections[i].sh_name]))
        {
            for(int j = 0; j < Sections[i].sh_size; j++)
                ((uint8_t*)(Sections[i].sh_addr))[j] = ((uint8_t*)(Sections[i].sh_offset + (uint32_t)(Header)))[j];

            return true;
        }

    }
    return false;
}

void elf_load(void)
{
    uint8_t* data = (uint8_t*)(0x20200 + (15 * SECTOR_SIZE));
    print(data);
    
    uint8_t* write_to_memory;
    uint8_t* read_from_file;
    // uint32_t file_base = (uint32_t)data;
    uint32_t begin_of_code = (uint32_t)data;

    uint16_t phnum = *(uint16_t*)((uint8_t*)data + 0x2C);

    uint32_t p_offset;      //offset in file image
    uint32_t p_vaddr;       //virtual address of the segment in memory
    uint32_t p_filesz;      //size in bytes of segment in file image
    // uint32_t p_memsz;       //size in bytes of segment in memory
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
            // p_memsz  = *(uint32_t*)(data + 0x14);

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

    elf_load_given_section((ElfAutoHeader*)(0x20200 + (15 * SECTOR_SIZE)), ".init_array");

    void(*kernel)(void) = (void(*)(void))entry_point;

    kernel();

}


void _start(void)
{
    init_disk(ATA_FIRST_BUS, ATA_MASTER);    
        
    for(int i = 0; i < 1600; i++) // Weird reboot, shutdown bug (probably cpu want to make fun of me (ivt))
        disk_read(ATA_FIRST_BUS, ATA_MASTER, 0xA9 + 14 + i, 1, (uint16_t*)(0x20200 + ((15 + i)* SECTOR_SIZE)));

    elf_load();
}
//14
//
//28

//znajdz w elfie sekcje 
//znajdz .init_array
//zaladuj