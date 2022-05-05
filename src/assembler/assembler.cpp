
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
    void xprintf(char* str, ... );
    
}
struct screen_t
{
    
    uint16_t** cursor;
    uint8_t x;
    uint8_t y;
};

typedef struct screen_t screen_t;

extern screen_t Screen;

char current_instruction[30];
char operand1[5] = {'\0'}, operand2[5] = {'\0'};

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
        read(file, current_instruction, 30);
        //current_instruction[3] = '\0';
        
        

        for(int j = 0; j < 1; j++) 
        {
            //if(strcmp(current_instruction, (char*)opcodes[j].instruction_name))
            {
                xprintf("%s\n", current_instruction);
                

                for(int k = 0; k < opcodes[j].operands; k++)
                {

                    int counter;

                    if(!k)
                    {
                        for(counter = 4; current_instruction[counter] != ','; counter++)
                            operand1[counter - 4] = current_instruction[counter];
                        counter++;
                    }

                    else
                    {
                        for(int l = counter; current_instruction[counter] != ' ' && 
                                    current_instruction[counter] != '\0'; counter++)
                            operand2[counter - l] = current_instruction[counter];
                    }

                }

                xprintf("operand1: %s\n", operand1);
                xprintf("operand2: %s\n", operand2);


                write(asm_output, (uint8_t*)&opcodes[j].opcode, 1);
            


            }
        }
    }

}

}
