
#include <lib/libc/stdiox.h>
#include <sys/devices/keyboard/scan_codes.h>
#include <sys/terminal/vty/vty.h>
#include <fs/xin.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/string.h>
#include <lib/libc/memory.h>
#include <sys/input/input.h>


#define MOVE_CURSOR_TO_FIRST_CHARACTER(EditState) while(((uint32_t)EditState->cursor - VGA_TEXT_MEMORY) % 0xA0 != 0) \
            EditState->cursor--

#define MOVE_CURSOR_TO_END_OF_LINE(EditState) while((char)(*EditState->cursor) != '\0') \
                EditState->cursor++

#define MOVE_CURSOR_TO_NEXT_ROW(EditState) EditState->cursor = EditState->cursor + VGA_WIDTH
#define MOVE_CURSOR_TO_PREVIOUS_ROW(EditState) EditState->cursor = EditState->cursor - VGA_WIDTH
#define CURSOR_SELECT_MODE_SET(EditState) *(EditState)->cursor = (uint16_t)((char)(*(EditState)->cursor) + (((white << 4) | black) << 8));
#define CURSOR_NORMAL_MODE_SET(EditState) *(EditState)->cursor = (uint16_t)((char)(*(EditState)->cursor) + (((black << 4) | white) << 8));

struct EditInfo
{
    uint32_t file_position;
    uint16_t* cursor;
    uint32_t column;
    uint32_t current_line;
    uint32_t total_lines;
    uint32_t number_of_sectors;
    char* program_buffer;
    char* begin_of_current_text;
};

typedef struct EditInfo EditInfo;


int edit_get_begin_of_printed_text(EditInfo* EditState)
{
    int tmp = EditState->current_line - (VGA_HEIGHT-1);
    int j = 0;

    for(int i = 0; i < tmp; i++)
    {
        while(EditState->program_buffer[j] != '\n')                
            j++;
        j++;
    }

    return j;

}

void edit_input(xchar Input, XinEntry* File, EditInfo* EditState)
{
    
    File->FileInfo->tmp_size = strlen(EditState->program_buffer);
    EditState->program_buffer[strlen(EditState->program_buffer)] = '\0';

    if(int_to_sectors(File->FileInfo->tmp_size) > EditState->number_of_sectors)
    {
        EditState->number_of_sectors++;
        EditState->program_buffer = File->FileInfo->buffer = realloc(File->FileInfo->buffer, EditState->number_of_sectors * SECTOR_SIZE);
    }

    int x_save = Screen.x; 
    int y_save = Screen.y;

    if(KeyInfo.is_ctrl == true)
    {
        if(Input.character == '$')
        {
            while(EditState->program_buffer[EditState->file_position] != '\n' && EditState->program_buffer[EditState->file_position] != '\0')
                EditState->file_position++;

            MOVE_CURSOR_TO_END_OF_LINE(EditState);

        }

        else if(Input.character == '0')
        {
            while(EditState->program_buffer[EditState->file_position-1] != '\n' && EditState->file_position != 0)
                EditState->file_position--;

            MOVE_CURSOR_TO_FIRST_CHARACTER(EditState);
        }
    }

    else if(Input.scan_code == ENTER)
    {
        char* tmp = (char*)calloc(strlen(EditState->program_buffer)) + 1;
        memcpy(tmp, EditState->program_buffer, strlen(EditState->program_buffer) + 1);

        int i =  EditState->file_position;
        for(; i < strlen(tmp) + 1; i++)
            EditState->program_buffer[i + 1] = tmp[i];

        EditState->program_buffer[EditState->file_position] = '\n';

        EditState->file_position++;
        EditState->total_lines++;
        EditState->current_line++;

        if(EditState->current_line >= VGA_HEIGHT)
        {            
            MOVE_CURSOR_TO_FIRST_CHARACTER(EditState);
        }

        else
        {
            MOVE_CURSOR_TO_NEXT_ROW(EditState);
            MOVE_CURSOR_TO_FIRST_CHARACTER(EditState);
        }
    }

    else if(Input.scan_code == TAB_KEY)
    {
        char* tmp = (char*)calloc(strlen(EditState->program_buffer)) + 3;
        memcpy(tmp, EditState->program_buffer, strlen(EditState->program_buffer) + 3);

        int i =  EditState->file_position;
        for(; i < strlen(tmp) + 3; i++)
            EditState->program_buffer[i + 3] = tmp[i];

        EditState->program_buffer[EditState->file_position] = ' ';
        EditState->program_buffer[EditState->file_position + 1] = ' ';
        EditState->program_buffer[EditState->file_position + 2] = ' ';

        EditState->file_position = EditState->file_position + 3;;

        EditState->cursor = EditState->cursor + 3;
    }

    else if(Input.scan_code == BSPC)
    {
        if(!EditState->file_position)
            return;

        int i;
        char character_to_delete = EditState->program_buffer[EditState->file_position - 1];

        for(i = EditState->file_position - 1; EditState->program_buffer[i] != '\0'; i++)
            EditState->program_buffer[i] = EditState->program_buffer[i+1];

        if(character_to_delete == '\n')
        {
            EditState->current_line--;
            EditState->total_lines--;
            MOVE_CURSOR_TO_PREVIOUS_ROW(EditState);
            MOVE_CURSOR_TO_END_OF_LINE(EditState);
        }
        
        else
            EditState->cursor--;

        EditState->program_buffer[i] = '\0';
        
        EditState->file_position--;

    }

    else if(Input.scan_code == ARROW_LEFT)
    {
    
        if(!EditState->file_position)
            return;

        if(EditState->program_buffer[EditState->file_position - 1] == '\n')
        {
            if(EditState->current_line < VGA_HEIGHT)
                MOVE_CURSOR_TO_PREVIOUS_ROW(EditState);

            MOVE_CURSOR_TO_END_OF_LINE(EditState);

            EditState->current_line--;
        }
        
        else 
            EditState->cursor--;
        
        EditState->file_position--;
    }
    
    else if(Input.scan_code == ARROW_RIGHT)
    {


        if(EditState->program_buffer[EditState->file_position] != '\0')
        {
            
            if(EditState->program_buffer[EditState->file_position] == '\n')
            {
                EditState->current_line++;
            
                if(EditState->current_line < VGA_HEIGHT)
                    MOVE_CURSOR_TO_NEXT_ROW(EditState);
                MOVE_CURSOR_TO_FIRST_CHARACTER(EditState);
            
            }

            else 
                EditState->cursor++;
            
            EditState->file_position++;
        }
    }


    else if(Input.scan_code == ARROW_UP)
    {

        if(!EditState->current_line)
            return;

        if(EditState->current_line < VGA_HEIGHT)
        {
            MOVE_CURSOR_TO_FIRST_CHARACTER(EditState);
            MOVE_CURSOR_TO_PREVIOUS_ROW(EditState);
        }

        while(EditState->program_buffer[EditState->file_position-1] != '\n')
            EditState->file_position--;
        EditState->file_position--;

        while((char)*EditState->cursor != '\0')
            EditState->cursor++;
        
        EditState->current_line--;
    }

    else if(Input.scan_code == ARROW_DOWN)
    {

        {
            int i = EditState->file_position;
            for(; EditState->program_buffer[i] != '\n' && EditState->program_buffer[i] != '\0'; i++);
            if(EditState->program_buffer[i] == '\0')
                return;
            if(i >= strlen(EditState->program_buffer))
                return;
        }


        EditState->current_line++;        

        if(EditState->current_line < VGA_HEIGHT)
            MOVE_CURSOR_TO_NEXT_ROW(EditState);

        MOVE_CURSOR_TO_FIRST_CHARACTER(EditState);

        int i;

        while(EditState->program_buffer[EditState->file_position] != '\n')
            EditState->file_position++;
        EditState->file_position++;

    }

    else if(Input.scan_code == F4_KEY || Input.scan_code == F4_KEY_RELEASE || Input.scan_code == ESC)
        return;
    

    else if(Input.character != '\0')
    {

        CURSOR_NORMAL_MODE_SET(EditState);

        char* tmp = (char*)calloc(strlen(EditState->program_buffer)) + 1;
        memcpy(tmp, EditState->program_buffer, strlen(EditState->program_buffer) + 1);

        int i =  EditState->file_position;
        for(; i < strlen(tmp) + 1; i++)
            EditState->program_buffer[i + 1] = tmp[i];

        EditState->program_buffer[EditState->file_position] = Input.character;

        EditState->file_position++;

        EditState->cursor++;
        CURSOR_SELECT_MODE_SET(EditState);
        free(tmp);
        
    }


}


int edit(char* file_name)
{

    screen_clear();
    XinEntry* file = xin_find_entry(file_name);

    if(file == NULL)
    {
        xprintf("Couldn't open file %s\n", file_name);
        xprintf("Do want to create it?\nY/n ");

        char selected_option = inputg().character;
        if(selected_option == 'n' || selected_option == 'N')
            return XANIN_ERROR;
        file = fopen(file_name, "rw");
    }

    else
        file = fopen(file_name, "rw");
    
    screen_clear();


    fread(file, NULL, file->size );

    EditInfo EditState = {0, (uint16_t*)VGA_TEXT_MEMORY, 0, 0, 0, xin_get_file_size_in_sectors(file), 
                            file->FileInfo->buffer, file->FileInfo->buffer};

    xprintf("%s", EditState.program_buffer);
    
    for(int i = 0; EditState.program_buffer[i] != '\0'; i++)
    {
        if(EditState.program_buffer[i] == '\n')
            EditState.total_lines++;
    }

    CURSOR_SELECT_MODE_SET(&EditState);

    while(KeyInfo.scan_code != F4_KEY && KeyInfo.scan_code != F4_KEY_RELEASE && KeyInfo.scan_code != ESC)
    {
        edit_input(inputg(), file, &EditState);
        EditState.begin_of_current_text = &EditState.program_buffer[edit_get_begin_of_printed_text(&EditState)];
        screen_clear();
        xprintf("%s", EditState.begin_of_current_text);
        CURSOR_SELECT_MODE_SET(&EditState);

        if(Screen.cursor[VGA_HEIGHT-1][70] == BLANK_SCREEN_CELL)
        { 
            cursor_set_position(70, VGA_HEIGHT - 1); 
            for(int i = 0; i < 5; i++)
                Screen.cursor[Screen.y][Screen.x + i] = BLANK_SCREEN_CELL;

            xprintf("%d/%d", EditState.current_line, EditState.total_lines);
        }

    }

    fclose_with_given_size(&file, strlen(EditState.program_buffer) + 1);//we need to include '\0' character


    return XANIN_OK;

}