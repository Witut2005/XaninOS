
#include <libc/string.h>
#include <libc/memory.h>
#include <libc/stdlibx.h>
#include <terminal/vty.h>
#include <keyboard/scan_codes.h>
#include <stdarg.h>
#include <libc/time.h>



char* color_attributes[16] = {"black", "blue", "green", "cyan", "red", "magenta", "brown", "lgray", "dgray","lblue", "lgreen", "lcyan", "lred", "lmagenta", "yellow", "white"};

void screen_clear(void)
{
    uint16_t* screen_cleaner = (uint16_t*)VGA_TEXT_MEMORY;
    for(int i = 0; i < (80 * 28); i++)
    {
        *screen_cleaner = '\0';
        screen_cleaner++;
    }

    Screen.x = 0;
    Screen.y = 0;
}



char putchar(char character)
{

    Screen.cursor[Screen.y][Screen.x] = (uint16_t) (character + (((black << 4) | white) << 8));
        
    Screen.x++;

    if(x == 80)
    {    
        Screen.x = 0x0;
        Screen.y++;
    }
           

    return character;

}

char putchar_color(uint8_t color, char character)
{

    Screen.cursor[Screen.y][Screen.x] = (uint16_t) (character + ((((color & 0xF0) << 4) | (color & 0xF)) << 8));
        
    Screen.x++;

    if(x == 80)
    {    
        Screen.x = 0x0;
        Screen.y++;
    }
           

    return character;

}


void print_bcd_number(uint8_t x)
{

    Screen.cursor[Screen.y][Screen.x] = (uint16_t)( (((x & 0xf0) >> 4) + 48)  | (((red << 4) | white) << 8));
    Screen.x += 0x2;

    Screen.cursor[Screen.y][Screen.x] = (uint16_t)(((x & 0x0f) + 48)  | (((red << 4) | white) << 8));
    Screen.x += 0x2;


    return;

}





void xprintf(char* str, ... )
{

    char tmp[128];
    char* temporary_pointer = tmp;
    
    uint32_t number;


    memset(tmp, '\0', sizeof(tmp));

    char* stringPtr;

    va_list args;
    va_start(args,str);



    uint32_t string_counter = 0;

    uint8_t background_color = black;
    uint8_t font_color = white;


    while(str[string_counter])
    {

        for(int i = 0; i < sizeof(tmp); i++)
            tmp[i] = '\0';

        if(str[string_counter] == '%')
        {

            string_counter++;
            switch(str[string_counter])
            {
                case 'd':
                {
                    number = va_arg(args,int);

                    int_to_str(number,temporary_pointer);

                    for(int i = 0; temporary_pointer[i] != '\0'; i++)
                    {                        
                        Screen.cursor[Screen.y][Screen.x] = (uint16_t) (temporary_pointer[i] + (((background_color << 4) | font_color) << 8));
                        if(Screen.x == 80)
                        {
                            Screen.y++;
                            Screen.x = 0x0;
                        }
                        Screen.x++;
                    }

                    break;
                }

                case 'f':
                {
                    //float float_number = (float)va_arg(args,double);
                    //float_to_string(float_number,temporary_pointer);


                }

                case 'y':
                {
                    uint32_t time = va_arg(args,int);
               
                    uint32_t time_mask = 0xF0000000;
                    uint32_t time_shift = 28;

                    for(int i = 0; i < 8; i++, time_mask = time_mask >> 4, time_shift -= 4)
                    {
                        if(i == 2 || i == 4)
                        {

                            Screen.cursor[Screen.y][Screen.x] = (uint16_t) '-'  | (((background_color << 4) | font_color) << 8);
                            Screen.x++;
                        }

                        Screen.cursor[Screen.y][Screen.x] = (uint16_t)( (((time & time_mask) >> time_shift) + '0')  | (((background_color << 4) | font_color) << 8));
                        Screen.x++;
                    }

                    break;

                }

                case 't':
                {
                
                    uint16_t time = va_arg(args,int);
               
                    uint16_t time_mask = 0xF000;
                    uint16_t time_shift = 12;

                    for(int i = 0; i < 4; i++, time_mask = time_mask >> 4, time_shift -= 4)
                    {
                        if(i == 2)
                        {

                            Screen.cursor[Screen.y][Screen.x] = (uint16_t) ':'  | (((background_color << 4) | font_color) << 8);
                            Screen.x++;
                        }

                        Screen.cursor[Screen.y][Screen.x] = (uint16_t)( (((time & time_mask) >> time_shift) + '0')  | (((background_color << 4) | font_color) << 8));
                        Screen.x++;
                    }
                    break;
                }

                case 'b':
                {
                    number = va_arg(args,int);

                    temporary_pointer = bin_to_str(number,tmp);

                    for(int i = 0; temporary_pointer[i] != '\0'; i++)
                    { 
                        Screen.cursor[Screen.y][Screen.x] = (uint16_t) (temporary_pointer[i] + (((background_color << 4) | font_color) << 8));
                        if(Screen.x == 80)
                        {
                            Screen.y++;
                            Screen.x = 0x0;
                        }
                        Screen.x++;
                    }

                    break;
                }

                    

                case 's':
                {

                    stringPtr = va_arg(args,char*);

                    if(stringPtr == nullptr)
                        return;


                    for(int i = 0; stringPtr[i] != '\0'; i++)
                    {
                        if(stringPtr[i] == '\n')
                        {            
                            Screen.x = 0x0;
                            Screen.y++;
                            continue;
                        }

                        Screen.cursor[Screen.y][Screen.x] = (uint16_t) (stringPtr[i] + (((background_color << 4) | font_color) << 8));
                        if(Screen.x == 80)
                        {
                            Screen.y++;
                            Screen.x = 0x0;
                        }
                        Screen.x++;
                    

                    }

                    break;

                }


                case 'i':
                {

                    uint8_t number = (uint8_t)va_arg(args, uint32_t);

                    Screen.cursor[Screen.y][Screen.x] = (uint16_t)( (((number & 0xf0) >> 4) + '0')  | (((background_color << 4) | font_color) << 8));
                    Screen.x++;

                    Screen.cursor[Screen.y][Screen.x] = (uint16_t)(((number & 0x0f) + '0')  | (((background_color << 4) | font_color) << 8));
                    Screen.x++;
                
                    break;

                }

                case 'c':
                {

                    char character;
                    character = (char)va_arg(args,int);
                    
                    if(Screen.x == 80)
                    {
                        Screen.y++;
                        Screen.x = 0x0;
                    }

                    if(character == '\n')
                    {            
                        Screen.x = 0x0;
                        Screen.y++;
                    }

                    else if(character == '\0')
                        return;

                    else 
                    {
                        Screen.cursor[Screen.y][Screen.x] = (uint16_t) (character + (((background_color << 4) | font_color) << 8));
                        Screen.x++;
                    }

                    break;
                }

                case 'z':
                {
                    font_color = (uint8_t)va_arg(args,int);
                    background_color = (font_color & 0xf0) >> 4;
                    font_color = font_color & 0x0f;
                    break;
                }

                case 'x':
                {
                    uint32_t number_hex = va_arg(args,uint32_t);
                    int_to_hex_str(number_hex,temporary_pointer);

                    for(int i = 0; temporary_pointer[i] != '\0'; i++)
                    {
                        Screen.cursor[Screen.y][Screen.x] = (uint16_t) (temporary_pointer[i] + (((background_color << 4) | font_color) << 8));
                        if(Screen.x == 80)
                        {
                            Screen.y++;
                            Screen.x = 0x0;
                        }
                        Screen.x++;
                    }

                    
                    break;

                }
                
                case 'X':
                {
                    number = va_arg(args,int);
                    int_to_hex_str(number,temporary_pointer);
                    toupper(temporary_pointer);

                    for(int i = 0; temporary_pointer[i] != '\0'; i++)
                    {
                        Screen.cursor[Screen.y][Screen.x] = (uint16_t) (temporary_pointer[i] + (((background_color << 4) | font_color) << 8));
                        if(Screen.x == 80)
                        {
                            Screen.y++;
                            Screen.x = 0x0;
                        }
                        Screen.x++;
                    }

                    break;

                }
                
                case 'o':
                {
                    number = va_arg(args,int);
                    int_to_oct_str(number,temporary_pointer);

                    for(int i = 0; temporary_pointer[i] != '\0'; i++)
                    {
                        Screen.cursor[Screen.y][Screen.x] = (uint16_t) (temporary_pointer[i] + (((background_color << 4) | font_color) << 8));
                        if(Screen.x == 80)
                        {
                            Screen.y++;
                            Screen.x = 0x0;
                        }
                        Screen.x++;
                    }
                    break;
                }

                case 'h':
                {
                    number = (uint16_t)va_arg(args,uint32_t);
                    Screen.x = (number >> 8) & 0xFF;
                    Screen.y = number & 0xFF;
                    break;
                }

                case 'm':
                {

                    string_counter++;
                    switch(str[string_counter])
                    {
                        case 'x':
                        {

                            uint8_t number_hex = (uint8_t)va_arg(args,uint32_t);
                            xint_to_hex_str(number_hex,temporary_pointer, sizeof(uint8_t));

                            for(int i = 0; temporary_pointer[i] != '\0'; i++)
                            {
                                Screen.cursor[Screen.y][Screen.x] = (uint16_t) (temporary_pointer[i] + (((background_color << 4) | font_color) << 8));
                                
                                if(Screen.x == 80)
                                {
                                    Screen.y++;
                                    Screen.x = 0x0;
                                }
                                
                                Screen.x++;
                            }

                    
                            break;                            

                        }
                
                        case 'X':
                        {

                            uint8_t number_hex = (uint8_t)va_arg(args,uint32_t);
                            xint_to_hex_str(number_hex,temporary_pointer, sizeof(uint8_t));
                            toupper(temporary_pointer);
                            
                            for(int i = 0; temporary_pointer[i] != '\0'; i++)
                            {
                                Screen.cursor[Screen.y][Screen.x] = (uint16_t) (temporary_pointer[i] + (((background_color << 4) | font_color) << 8));
                                
                                if(Screen.x == 80)
                                {
                                    Screen.y++;
                                    Screen.x = 0x0;
                                }

                                if(Screen.y == 28)
                                {
                                    Screen.y = 0x0;
                                    break;
                                }
                                
                                Screen.x++;
                            }

                    
                            break;                            

                        }

                    }


                    break;
                }

            }

            string_counter++;
        }

        else if(str[string_counter] == '\n')
        {            
            Screen.x = 0x0;
            Screen.y++;
            string_counter++;
        }

        else if(str[string_counter] == '\r')
        {
            Screen.x = 0;
            string_counter++;
        }

        else if(str[string_counter] == '\t')
        {
            for(int i = 0; i < 3; i++)
                Screen.cursor[Screen.y][Screen.x + i] = (uint16_t)(' ' + (((background_color << 4) | font_color) << 8));

            Screen.x += 3;
            string_counter++;
        }

        else if(str[string_counter] == '\\')
        {
            Screen.cursor[Screen.y][Screen.x] = (uint16_t)('\\' + (((background_color << 4) | font_color) << 8));
            string_counter++;
        }

        else
        {
            //cursor[bufCounter] = (uint16_t) (str[string_counter] + (((background_color << 4) | font_color) << 8));

            //x++;

            Screen.cursor[Screen.y][Screen.x] = (uint16_t) (str[string_counter] + (((background_color << 4) | font_color) << 8));
            Screen.x++;
            string_counter++;
        }



    }

    va_end(args);

}



void xscanf(char* str, ... )
{

    uint32_t str_counter = 0x0;
    uint32_t counter = 0x0;

    char* string_pointer;

    va_list args;
    va_start(args, str);

    char buffer[50];

    memset(command_buffer, '\0', sizeof(command_buffer));
    memset(buffer, '\0', sizeof(buffer));

    index = 0x0;
 
    character_blocked = (char)Screen.cursor[Screen.y][Screen.x - 1];

    char* starting_screen_position = (char*)&Screen.cursor[Screen.y][Screen.x - 1];
    uint16_t* text_buffer_start = &Screen.cursor[Screen.y][Screen.x];

    uint8_t x_start = Screen.x;
    uint8_t y_start = Screen.y;

    uint8_t x_current, y_current;


    start:

    while(1)
    {

        if(KeyInfo.is_bspc)
        {
            Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) + (((black << 4) | white) << 8));

            if(&Screen.cursor[Screen.y][Screen.x - 1] == (uint16_t*)starting_screen_position)
                goto start;


            if(!Screen.x)
            {
                Screen.y--;
                Screen.x = 79;
                return;
            }

            Screen.x--;



            if(index)
                index = index - 1;

            command_buffer[index] = '\0';
            Screen.cursor[Screen.y][Screen.x] = '\0';

            Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) + (((white << 4) | white) << 8));


            // msleep(100);
            KeyInfo.is_bspc = false;
            // letters_refresh(&Screen.cursor[Screen.y][Screen.x]);
        }

        // else if(KeyInfo.scan_code == LSHIFT)
        // {
        //     goto start;
        // }

        // else if(KeyInfo.scan_code == ARROW_RIGHT || KeyInfo.scan_code == ARROW_LEFT)
        // {
            

        //     if(KeyInfo.scan_code == ARROW_LEFT)
        //     {        
        
        //         Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));

        //         if((char)Screen.cursor[Screen.y][Screen.x - 1] == character_blocked)
        //         {
        //             goto start;
        //         }

        //         Screen.x--;
        
        //         if(index)
        //             index--;

        //         Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | ((white << 4) | black) << 8);
        //     }

        //     else 
        //     {
        //         //Screen.cursor[Screen.y][Screen.x] = (uint16_t)(selected_character | ((black << 4) | white) << 8);
        
        //         Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));
        

        //         if(&Screen.cursor[Screen.y][Screen.x + 1] >= &Screen.cursor[8][79])
        //         {
        //             goto start;
        //         }

        //         Screen.x++;

        //         if(Screen.x == 80)
        //         {
        //             Screen.x = 0;
        //             Screen.y++;
        //         }

        //         index++;

        //         Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | ((white << 4) | black) << 8);    
        //     }

        //     KeyInfo.scan_code = 0x0;
                
        // }

        // else if(KeyInfo.scan_code == ARROW_UP)
        // {

        //     strcpy(keyboard_command, last_used_commands);
        //     keyboard_command[strlen(last_used_commands)] = ' ';
        //     strcpy(keyboard_command + strlen(last_used_commands) + 1, last_used_parameters);

        //     int x_new = strlen(last_used_commands) + strlen(last_used_parameters);

        //     memset(last_used_commands, '\0', sizeof(last_used_commands));
        //     memset(last_used_parameters, '\0', sizeof(last_used_parameters));
            
        //     Screen.x = 1;

        //     uint16_t first_tmp = (uint16_t)Screen.cursor[Screen.y][Screen.x];
        //     xprintf("%s", keyboard_command);


        //     index = x_new;
        //     Screen.x = x_new;

        //     Screen.cursor[Screen.y][1] = first_tmp;

        //     // cpu_halt();

        // }

        else if(KeyInfo.scan_code == ENTER)
        {
            while(str[str_counter])
            {
                if(str[str_counter] == '%')
                {
                    str_counter++;
                    switch(str[str_counter])
                    {
                        case 's':
                        {

                            string_pointer = va_arg(args, char*);
                            
                            for(int i = 0; string_pointer[i] != '\0'; i++)
                                string_pointer[i] = '\0';

                            for(int i = 0x0; keyboard_command[counter] != '\0' && keyboard_command[counter] != ' '; i++)
                            {
                                buffer[i] = keyboard_command[counter];
                                counter++;
                            }
                                
                                
                            for(int i = 0; buffer[i] != '\0' && buffer[i] != ' '; i++)
                            {
                                if((buffer[i] > 127) || (buffer[i] < 0x20))
                                {
                                    string_pointer[i] = '\0';
                                    goto end;
                                }

                                string_pointer[i] = buffer[i];
                            }

                            end:


                            
                            for(int i = 0x0; i < 50;i++)
                                buffer[i] = '\0';

                            counter++;
                            break;
                        }


                        case 'd':
                        {

                            uint32_t* number = va_arg(args, uint32_t*);
                            
                            for(int i = 0x0; keyboard_command[counter] != '\0' && keyboard_command[counter] != ' '; i++)
                            {
                                buffer[i] = keyboard_command[counter];
                                counter++;
                            }
                            
                            *number = strtoi(buffer, 10);                       

                            break;
                        }

                        case 'c':
                        {

                            char* number = va_arg(args, char*);
                            *number = keyboard_command[0]; //strtoi(buffer, 10);                     
                            break;
                        }




                        case 'x':
                        {

                            uint32_t* number = va_arg(args, uint32_t*);
                            
                            for(int i = 0x0; keyboard_command[counter] != '\0' && keyboard_command[counter] != ' '; i++)
                            {
                                buffer[i] = keyboard_command[counter];
                                counter++;
                            }
                            
                            *number = strtoi(buffer, 16);                       

                            break;
                        }
                    

                        case 'b':
                        {

                            uint32_t* number = va_arg(args, uint32_t*);
                            
                            for(int i = 0x0; keyboard_command[counter] != '\0' && keyboard_command[counter] != ' '; i++)
                            {
                                buffer[i] = keyboard_command[counter];
                                counter++;
                            }
                            
                            *number = strtoi(buffer, 2);                       

                            break;
                        }


                    }

                str_counter++;

                }

                else
                {
                    str_counter++;
                }

            }

        for(int i = 0; i < 50;i++)
            keyboard_command[i] = '\0';


        for(int i = 0x0; i < 50;i++)
            buffer[i] = 0x0;

        KeyInfo.scan_code = 0x0;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) + (((black << 4) | white) << 8));
           
        xprintf("\n");
        return;

        }

        else if(KeyInfo.character)
        {
            char tmp = getchar();
            // char tmp = KeyInfo.character;

            Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][index + x_start]) + (((black << 4) | white) << 8));
            
            for(int i = 0; i < strlen(keyboard_command); i++)
                *(text_buffer_start + i) = 0x0;

            Screen.x = x_start;
            Screen.y = y_start;

            uint8_t* tmp_buf = (uint8_t*)calloc(80);
            memcpy(tmp_buf, keyboard_command, 80);

            if(keyboard_command[index] != '\0')
            {
                for(int i = index; i < 50 - 1; i++)
                    keyboard_command[i+1] = tmp_buf[i];
            }

            free(tmp_buf);
            keyboard_command[index] = tmp;

            xprintf("%s", keyboard_command);

            index++;
            Screen.x = index + x_start;

        }    
    }



}




void xscan_range(char* string_buffer, uint32_t how_many_chars)
{

    uint32_t str_counter = 0x0;
    uint32_t counter = 0x0;

    char* string_pointer;


    char* buffer = (char*)calloc(how_many_chars);

    memset(command_buffer, '\0', sizeof(command_buffer));

    index = 0;
    uint8_t screen_offset = 0;
 
    character_blocked = (char)Screen.cursor[Screen.y][Screen.x - 1];

    char* starting_screen_position = (char*)&Screen.cursor[Screen.y][Screen.x - 1];
    uint16_t* text_buffer_start = &Screen.cursor[Screen.y][Screen.x];

    uint8_t x_start = Screen.x;
    uint8_t y_start = Screen.y;

    uint8_t x_current, y_current;


    start:

    while(1)
    {

        if(KeyInfo.is_bspc)
        {
            Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) + (((black << 4) | white) << 8));

            if(&Screen.cursor[Screen.y][Screen.x - 1] == (uint16_t*)starting_screen_position)
            {
                goto start;
            }



            if(!Screen.x)
            {
                Screen.y--;
                Screen.x = 79;
                return;
            }

            Screen.x--;



            if(index)
                index--;
            if(screen_offset)
                screen_offset--;

            command_buffer[index] = '\0';
            Screen.cursor[Screen.y][Screen.x] = '\0';

            Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) + (((white << 4) | white) << 8));


            //msleep(10);
            KeyInfo.is_bspc = false;
            letters_refresh(&Screen.cursor[Screen.y][Screen.x]);
        }

        else if(KeyInfo.scan_code == ENTER)
        {
 

            string_pointer = string_buffer;
            
            for(int i = 0; string_pointer[i] != '\0'; i++)
                string_pointer[i] = '\0';

            for(int i = 0x0; keyboard_command[counter] != '\0' && keyboard_command[counter] != ' '; i++)
            {
                buffer[i] = keyboard_command[counter];
                counter++;
            }
                
                
            for(int i = 0; buffer[i] != '\0' && buffer[i] != ' '; i++)
            {
                if((buffer[i] > 127) || (buffer[i] < 0x20))
                {
                    string_pointer[i] = '\0';
                    goto end;
                }

                string_pointer[i] = buffer[i];
            }

            end:

            for(int i = 0x0; i < 50;i++)
                buffer[i] = '\0';

        


        for(int i = 0; i < 50;i++)
            keyboard_command[i] = '\0';


        for(int i = 0x0; i < 50;i++)
            buffer[i] = 0x0;

        KeyInfo.scan_code = 0x0;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) + (((black << 4) | white) << 8));
           
        // for(int i = how_many_chars; i < sizeof(buffer); i++)


        xprintf("\n");
        return;

        }

        else if(KeyInfo.character)
        {
            char tmp = getchar();

            Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][index + x_start]) + (((black << 4) | white) << 8));
            
            for(int i = 0; i < strlen(keyboard_command); i++)
                *(text_buffer_start + i) = 0x0;

            Screen.x = x_start;
            Screen.y = y_start;

            uint8_t* tmp_buf = (uint8_t*)calloc(80);
            memcpy(tmp_buf, keyboard_command, 80);

            if(keyboard_command[index] != '\0')
            {
                for(int i = index; i < 50 - 1; i++)
                    keyboard_command[i+1] = tmp_buf[i];
            }




            if(index < how_many_chars)
            {
                keyboard_command[index] = tmp;
                index++;
            }
            
            xprintf("%s", keyboard_command);

            screen_offset++;
            Screen.x = screen_offset + x_start;

        }    
    }



}



