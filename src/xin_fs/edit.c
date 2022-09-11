
#include <libc/stdiox.h>
#include <keyboard/scan_codes.h>
#include <terminal/vty.h>
#include <xin_fs/xin.h>
#include <libc/stdlibx.h>
#include <libc/string.h>
#include <libc/memory.h>

static uint32_t file_position;
static char* program_buffer;
static uint16_t* cursor;
static int column, current_line;

void edit_input(xchar Input)
{
    *cursor = (uint16_t)((char)(*cursor) + (((white << 4) | black) << 8));

    if(Input.scan_code == ENTER)
    {
        *cursor = (uint16_t)((char)(*cursor) + (((black << 4) | white) << 8));

        char* tmp = (char*)calloc(strlen(program_buffer)) + 1;
        memcpy(tmp, program_buffer, strlen(program_buffer) + 1);

        int i =  file_position;
        for(; i < strlen(tmp) + 1; i++)
            program_buffer[i + 1] = tmp[i];

        program_buffer[file_position] = '\n';

        file_position++;
        
        cursor = cursor + VGA_WIDTH;

        while(((uint32_t)cursor - VGA_TEXT_MEMORY) % 0xA0 != 0)
            cursor--;
        
        screen_clear();
        xprintf("%s", program_buffer);

        current_line++;
        *cursor = (uint16_t)((char)(*cursor) + (((white << 4) | black) << 8));
    }

    else if(Input.scan_code == TAB_KEY)
    {
        *cursor = (uint16_t)((char)(*cursor) + (((black << 4) | white) << 8));

        char* tmp = (char*)calloc(strlen(program_buffer)) + 3;
        memcpy(tmp, program_buffer, strlen(program_buffer) + 3);

        int i =  file_position;
        for(; i < strlen(tmp) + 3; i++)
            program_buffer[i + 3] = tmp[i];

        program_buffer[file_position] = ' ';
        program_buffer[file_position + 1] = ' ';
        program_buffer[file_position + 2] = ' ';

        file_position = file_position + 3;;

        screen_clear();
        xprintf("%s", program_buffer);


        cursor = cursor + 3;
        *cursor = (uint16_t)((char)(*cursor) + (((white << 4) | black) << 8));
    }

    else if(Input.scan_code == BSPC)
    {
        if(!file_position)
            return;

        *cursor = (uint16_t)((char)(*cursor) + (((black << 4) | white) << 8));

        int i;
        char character_to_delete = program_buffer[file_position - 1];

        for(i = file_position - 1; program_buffer[i] != '\0'; i++)
            program_buffer[i] = program_buffer[i+1];

        if(character_to_delete == '\n')
        {
            current_line--;
            cursor =  cursor - VGA_WIDTH;
            while(((uint32_t)cursor - VGA_TEXT_MEMORY) % 0xA0 != 0)
                cursor--;
        
            while((char)(*cursor) != '\0')
                cursor++;
        }
        
        else
            cursor--;

        program_buffer[i] = '\0';
        
        screen_clear();

        xprintf("%s", program_buffer);
        file_position--;

        *cursor = (uint16_t)((char)(*cursor) + (((white << 4) | black) << 8));

    }

    else if(Input.scan_code == ARROW_LEFT)
    {
    
        if(!file_position)
            return;
        *cursor = (uint16_t)((char)(*cursor) + (((black << 4) | white) << 8));
        
        if(program_buffer[file_position - 1] == '\n')
        {
            current_line--;
            cursor =  cursor - VGA_WIDTH;
            while(((uint32_t)cursor - VGA_TEXT_MEMORY) % 0xA0 != 0)
                cursor--;
        
            while((char)(*cursor) != '\0')
                cursor++;
        }
        
        else 
            cursor--;
        
        file_position--;
        *cursor = (uint16_t)((char)(*cursor) + (((white << 4) | black) << 8));

    }
    
    else if(Input.scan_code == ARROW_RIGHT)
    {


        if(program_buffer[file_position] != '\0')
        {
            *cursor = (uint16_t)((char)(*cursor) + (((black << 4) | white) << 8));
            
            if(program_buffer[file_position] == '\n')
            {
                current_line++;
                while(((uint32_t)cursor - VGA_TEXT_MEMORY) % 0xA0 != 0)
                    cursor--;
                cursor = cursor + VGA_WIDTH;
            }
            
            else 
                cursor++;
            
            file_position++;
            *cursor = (uint16_t)((char)(*cursor) + (((white << 4) | black) << 8));
        }
    }


    else if(Input.scan_code == ARROW_UP)
    {

        if(!current_line)
            return;

        *cursor = (uint16_t)((char)(*cursor) + (((black << 4) | white) << 8));

        while(((uint32_t)cursor - VGA_TEXT_MEMORY) % 0xA0 != 0)
            cursor--;

    

        int i;
        int cursor_offset = 0;
        char where_to_move;
        
        for(i = file_position - 1; program_buffer[i] != '\n'; i--);
        where_to_move = program_buffer[i];


        cursor = cursor - VGA_WIDTH;
        while(program_buffer[file_position-1] != '\n')
            file_position--;
        file_position--;

        while((char)*cursor != '\0')
            cursor++;
        
        current_line--;
        *cursor = (uint16_t)((char)(*cursor) + (((white << 4) | black) << 8));
    }

    else if(Input.scan_code == ARROW_DOWN)
    {

        {
            int i = file_position;
            for(; program_buffer[i] != '\n'; i++);
            if(program_buffer[i+1] == '\0')
                return;
            if(i >= strlen(program_buffer))
                return;
        }

        *cursor = (uint16_t)((char)(*cursor) + (((black << 4) | white) << 8));

        cursor = cursor + VGA_WIDTH;

        while(((uint32_t)cursor - VGA_TEXT_MEMORY) % 0xA0 != 0)
            cursor--;


        int i;
        char where_to_move;
        
        for(i = file_position + 1; program_buffer[i] != '\n'; i++);
        where_to_move = program_buffer[i];

 
        while(program_buffer[file_position] != '\n')
            file_position++;
        file_position++;

        current_line++;        
        *cursor = (uint16_t)((char)(*cursor) + (((white << 4) | black) << 8));
    }

    else if(Input.scan_code == F4_KEY || Input.scan_code == F4_KEY_RELEASE)
        return;
    

    else if(Input.character != '\0')
    {
        *cursor = (uint16_t)((char)(*cursor) + (((black << 4) | white) << 8));


        char* tmp = (char*)calloc(strlen(program_buffer)) + 1;
        memcpy(tmp, program_buffer, strlen(program_buffer) + 1);

        int i =  file_position;
        for(; i < strlen(tmp) + 1; i++)
            program_buffer[i + 1] = tmp[i];

        program_buffer[file_position] = KeyInfo.character;

        file_position++;

        cursor++;
        screen_clear();
        xprintf("%s", program_buffer);
        *cursor = (uint16_t)((char)(*cursor) + (((white << 4) | black) << 8));
        free(tmp);
        
    }


}


int edit(char* file_name)
{

    screen_clear();

    xin_entry* file = fopen(file_name, "r");
    cursor = (uint16_t*)VGA_TEXT_MEMORY;
    *cursor = (uint16_t)(*cursor + (((white << 4) | black) << 8));

    
    if(file == nullptr)
    {
        xprintf("Couldn't open file %s\n", file_name);
        while(KeyInfo.scan_code != ENTER);
    }
    
    program_buffer = (char*) calloc(VGA_SCREEN_RESOLUTION);
    read(file, program_buffer, VGA_SCREEN_RESOLUTION);

    file_position = 0x0;

    xprintf("%s", program_buffer);
    

    while(KeyInfo.scan_code != F4_KEY && KeyInfo.scan_code != F4_KEY_RELEASE)
        edit_input(inputg());



    file_position = strlen(program_buffer);
    file->entry_size = file_position;

    fseek(file, 0x0);
    write(file, program_buffer, strlen(program_buffer));

    screen_clear();
    xprintf("%s", program_buffer);
    while(KeyInfo.scan_code != ENTER);
    // for(int i = 1; i <= 3; i++)
    //     xprintf("%d. %s", i, getline(file, i));


    free(program_buffer);

}