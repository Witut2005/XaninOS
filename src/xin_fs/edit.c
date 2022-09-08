
#include <libc/stdiox.h>
#include <keyboard/scan_codes.h>
#include <terminal/vty.h>
#include <xin_fs/xin.h>
#include <libc/stdlibx.h>
#include <libc/string.h>

static uint32_t file_position;
static char* program_buffer;
static uint16_t* cursor;

void edit_input(xchar Input)
{

    if(Input.scan_code == ENTER)
    {
        *cursor = (uint16_t)((char)(*cursor) + (((black << 4) | white) << 8));
        xprintf("\n");
        program_buffer[file_position] = '\n';
        file_position++;
        
        cursor = cursor + 80;

        while(((uint32_t)cursor - VGA_TEXT_MEMORY) % 0xA0 != 0)
            cursor--;

        *cursor = (uint16_t)((char)(*cursor) + (((white << 4) | black) << 8));
    }

    else if(Input.scan_code == TAB_KEY)
    {
        for(int i = 0; i < 3; i++)
        {
            xprintf("%c", ' ');
            program_buffer[file_position] = ' ';
            file_position++;
        }
    }

    else if(Input.scan_code == BSPC)
    {
        *cursor = (uint16_t)((char)(*cursor) + (((black << 4) | white) << 8));

        int i;
        for(i = file_position - 1; program_buffer[i] != '\0'; i++)
            program_buffer[i] = program_buffer[i+1];
        program_buffer[i] = '\0';
        
        screen_clear();

        for(int i = 0; i < VGA_SCREEN_RESOLUTION; i++)
            xprintf("%c", program_buffer[i]);
        file_position--;

        cursor--;
        *cursor = (uint16_t)((char)(*cursor) + (((white << 4) | black) << 8));

    }

    else if(Input.scan_code == ARROW_LEFT)
    {
        *cursor = (uint16_t)((char)(*cursor) + (((black << 4) | white) << 8));
        file_position--;
        cursor--;
        *cursor = (uint16_t)((char)(*cursor) + (((white << 4) | black) << 8));
    }
    
    else if(Input.scan_code == ARROW_RIGHT)
    {
        if(program_buffer[file_position] != '\0')
        {
            *cursor = (uint16_t)((char)(*cursor) + (((black << 4) | white) << 8));
            file_position++;
            cursor++;
            *cursor = (uint16_t)((char)(*cursor) + (((white << 4) | black) << 8));
        }
    }

    else if(Input.scan_code == ARROW_UP)
    {
        *cursor = (uint16_t)((char)(*cursor) + (((black << 4) | white) << 8));

        while(((uint32_t)cursor - VGA_TEXT_MEMORY) % 0xA0 != 0)
            cursor--;
        cursor--;

        {

        int i;
        int cursor_offset = 0;
        char where_to_move;
        
        for(i = file_position; program_buffer[i] != '\n'; i--);
        where_to_move = program_buffer[i-1];

        while((char)*cursor != where_to_move) 
            cursor--;

        }
            


        while(program_buffer[file_position] != '\n')
            file_position--;
        file_position--;
        
        *cursor = (uint16_t)((char)(*cursor) + (((white << 4) | black) << 8));
    }

    else if(Input.scan_code == ARROW_DOWN)
    {
        *cursor = (uint16_t)((char)(*cursor) + (((black << 4) | white) << 8));

        cursor = cursor + VGA_WIDTH;

        while(((uint32_t)cursor - VGA_TEXT_MEMORY) % 0xA0 != 0)
            cursor--;

        {

        int i;
        int cursor_offset = 0;
        char where_to_move;
        
        for(i = file_position; program_buffer[i] != '\n'; i++);
        i++;
        for(; program_buffer[i] != '\n'; i++);
        where_to_move = program_buffer[i-1];

        while((char)*cursor != where_to_move || (char)*(cursor + 1) != '\0') 
            cursor++;

        }
            


        while(program_buffer[file_position] != '\n')
            file_position++;

        file_position++;
        
        while(program_buffer[file_position] != '\n')
            file_position++;

        file_position--;
        
        *cursor = (uint16_t)((char)(*cursor) + (((white << 4) | black) << 8));
    }
    

    else if(Input.character != '\0')
    {
        *cursor = (uint16_t)((char)(*cursor) + (((black << 4) | white) << 8));
        xprintf("%c", KeyInfo.character);

        for(int i = file_position; program_buffer[i] != '\n' && program_buffer[i] != '\0'; i++)
            program_buffer[i + 1] = program_buffer[i];


        program_buffer[file_position] = KeyInfo.character;
        
        file_position++;

        cursor++;
        *cursor = (uint16_t)((char)(*cursor) + (((white << 4) | black) << 8));
        
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
    
    program_buffer = (char*) malloc(VGA_SCREEN_RESOLUTION);
    read(file, program_buffer, VGA_SCREEN_RESOLUTION);

    file_position = strlen(program_buffer);

    for(int i = 0; i < VGA_SCREEN_RESOLUTION; i++)
        xprintf("%c", program_buffer[i]);
    

    while(KeyInfo.scan_code != F4_KEY)
        edit_input(inputg());


    file->entry_size = file_position;//ZLE moze byc przeciez np na poczatku
    fseek(file, 0x0);
    write(file, program_buffer, file_position);
    fclose(&file);

    free(program_buffer);

}