

#include <assembler/assembler.hpp>

extern "C" uint32_t strcmp(char* a, char* b);

 
struct screen_t
{
    
    uint16_t** cursor;
    uint8_t x;
    uint8_t y;
};

typedef struct screen_t screen_t;

extern screen_t Screen;

char current_instruction[4];

void assemble()
{

    while(Screen.y < 28)
    {

        for(int i = 0; i < 3; i++)
        {
           current_instruction[i] = (char)Screen.cursor[Screen.y][i]; 
        }
    
        current_instruction[3] = '\0';
            
        for(int i = 0; i < 20; i++) 
        {
            if(strcmp(current_instruction, (char*)opcodes[i].instruction_name))
            {

            }
        
        }
    }

}
