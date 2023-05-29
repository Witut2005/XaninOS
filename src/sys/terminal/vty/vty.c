
#include <lib/libc/colors.h>
#include <sys/devices/keyboard/key_map.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/string.h>

#define VGA_SCREEN_RESOLUTION 4480
#define VGA_TEXT_MEMORY 0xb8000
#define VGA_HEIGHT 25
// #define NULL 0x0


bool cursor_show = false;
char selected_character; 

typedef void (*keyboard_handle_input)(void);

keyboard_handle_input keyboard_handle = NULL;
 
char program_name[1000];
char program_parameters[40];
char program_parameters1[40];
char program_parameters2[40];
char program_parameters3[40];


char last_used_commands[40];
char last_used_parameters[40];

uint8_t index;

uint8_t x = 0, y = 0;

uint8_t logo_front_color = yellow;
uint8_t logo_back_color = black;
 
struct screen_t
{
    uint16_t** cursor;
    uint8_t x;
    uint8_t y;

};

typedef struct screen_t screen_t;
screen_t Screen = {(uint16_t**)VGA_TEXT_MEMORY, 0, 0};

uint16_t* screen_rows[VGA_HEIGHT];

void screen_init(void)
{
    //screen.cursor = VGA_TEXT_MEMORY;
    
    Screen.cursor = screen_rows;

    for(int i = 0; i < VGA_HEIGHT; i++)
        Screen.cursor[i] = (uint16_t*)(VGA_TEXT_MEMORY + (80 * i * 2));  

    Screen.x = 0x0;
    Screen.y = 0x0;
    
}

void letters_refresh(uint16_t* cursor_current_positon)
{
    cursor_current_positon++;

    for(uint16_t* i = cursor_current_positon; (uint32_t)i < VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION; i++)
        *(i - 1) = *i;
}

void letters_refresh_add(uint16_t* cursor_current_positon, char character_saved)
{

    char tmp;

    for(uint16_t* i = cursor_current_positon; (uint32_t)i < VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION; i++)
    {
        tmp = *(char*)(i);
        *i = (uint16_t)((char)(character_saved) + (((black << 4) | white) << 8));
        character_saved = tmp;
    }

    *cursor_current_positon = (uint16_t)((char)(*cursor_current_positon) + (((black << 4) | white) << 8));

}

// void keyboard_refresh_add(uint8_t keyboard_index_position, char character_saved)
// {

//     char tmp;

//     uint8_t counter = keyboard_index_position;

//     for(char* i = &keyboard_command[keyboard_index_position]; counter < 50; counter++, i++)
//     {
//         tmp = *i;
//         *i = character_saved;
//         character_saved = tmp;
//     }
// }


static char app_exit_status_text[40];

// char* app_exit_status_text_get(void)
// {

//     switch(last_command_exit_status)
//     {
//         case XANIN_OK:
//         {
//             strcpy(app_exit_status_text, "ok");
//             break;
//         }

//         case XANIN_ERROR:
//         {
//             strcpy(app_exit_status_text, "error");
//             break;
//         }

//         // case XIN:
//         // {
//         //     strcpy(app_exit_status_text, "error");
//         //     break;
//         // }

//     }
    
//     return app_exit_status_text;
// }


