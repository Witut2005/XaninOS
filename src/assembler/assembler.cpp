
#include <stddef.h>
#include <assembler/assembler.hpp>
#include <xin_fs/xin_entry.h>

extern "C"
{
    uint32_t strcmp(char* a, char* b);
    xin_entry* fopen(char* file_name, const char* mode);
    void create_file(char* entry_name);
    size_t write(xin_entry* entry, void* buf, size_t count);
    void create(char* file_name);
    size_t read(xin_entry *entry, void *buf, size_t count);
    void fseek(xin_entry *file, uint32_t new_position);
}
struct screen_t
{
    
    uint16_t** cursor;
    uint8_t x;
    uint8_t y;
};

typedef struct screen_t screen_t;

extern screen_t Screen;

char current_instruction[4];

extern "C"
{

void assembler(char* file_name)
{

    xin_entry* file = fopen(file_name, "r");
    xin_entry* asm_output = fopen("a.out", "w");


    if(file == nullptr)
        return;


    for(int i = 0; i < 28; i++)
    {

        fseek(file, 80 * i);
        read(file, current_instruction, 3);
        
        current_instruction[3] = '\0';
            
        for(int j = 0; j < 20; j++) 
        {
            if(strcmp(current_instruction, (char*)opcodes[j].instruction_name))
            {
                fseek(asm_output, i);
                write(asm_output, (uint8_t*)&opcodes[j].opcode, 1);
            }
        }
    }

}

}