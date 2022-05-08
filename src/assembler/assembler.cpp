
//#include <libcpp/utility.h>
#include <stddef.h>
#include <assembler/assembler.hpp>
#include <xin_fs/xin_entry.h>
#include <libcpp/cstdio.h>
#include <libcpp/hal.h>
#include <libcpp/algorithm.h>

extern "C"
{
    uint32_t strcmp(char* a, char* b);
    xin_entry* fopen(char* file_name, const char* mode);
    void create_file(char* entry_name);
    size_t write(xin_entry* entry, void* buf, size_t count);
    void create(char* file_name);
    size_t read(xin_entry *entry, void *buf, size_t count);
    void fseek(xin_entry *file, uint32_t new_position);
    //void xprintf(char* str, ... );
    
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

struct Test{

    int val;

    Test(int x)
    {
        val = x;
    }

};


void assembler(char* file_name)
{

    int arr[5] = {0,1,2,3,4};
    //Test test = for_each(arr, &arr[5], Test::Test());

    Test test(10);

    xprintf("test: %d", test.val);

    

    while(1); 
 
    xin_entry* file = fopen(file_name, "r");
    xin_entry* asm_output = fopen("a.out", "w");


    if(file == nullptr)
        return;


    for(int i = 0; i < 28; i++)
    {

        fseek(file, 80 * i);
        read(file, current_instruction, 30);
        //current_instruction[3] = '\0';
        
        

        for(int j = 0; j < 2; j++) 
        {
      //      if(strcmp(current_instruction, (char*)opcodes[j].instruction_name))
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

                        operand1[counter - 1] = '\0';
                    }

                    else
                    {
                        int l;

                        for(l = counter; current_instruction[counter] != ' ' && 
                                    current_instruction[counter] != '\0'; counter++)
                            operand2[counter - l] = current_instruction[counter];
                        
                        operand2[counter - l + 1] = '\0'; 

                    }

                }

                xprintf("operand1: %s\n", operand1);
                xprintf("operand2: %s\n", operand2);

                operands_get(opcodes[j].opcode, operand1, operand2);

                write(asm_output, machine_code, 2);
                return;        
            }
        
    
        }
        
    }

}

}
