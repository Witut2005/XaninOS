
#pragma once

#include <gyn_cl/gyn.h>

void gyn_interpreter(char* file_to_interpret)
{

    xin_entry* file = fopen(file_to_interpret, "r");
    char command[80];

    if(file == nullptr)
    {
        xprintf("%zCouldn't open file %s\n", stderr, file_to_interpret);
        while(KeyInfo.scan_code != ENTER);
        return;
    }

        
    gyn_cl_on = true;
    for(int i = 0; i < VGA_HEIGHT; i++)
    {

        for(int i = 0; i < 5; i++)
            memset(argv[i], '\0', 40);

        read(file, command, sizeof(command));

        int arg_counter = 0;
        int arg_command_counter = 0;

        for(int i = 0; i < VGA_WIDTH; i++, arg_command_counter++)
        {
            
            if(command[i] == ' ' || command[i] == '\0')
            {
                arg_counter++;
                argv[arg_counter][arg_command_counter] = '\0';
                arg_command_counter = -1;
            }
            
            else
                argv[arg_counter][arg_command_counter] = command[i];

        }

        /*
        for(int i = 0; i < 3; i++)
            xprintf("%s(%d)\n", argv[i], strlen(argv[i]));
        */

        if(strcmp(argv[0], "goto"))
        {
            fseek(file, VGA_WIDTH * strtoi(argv[1], 10));
            continue;
        }

        scan();

    }

    gyn_cl_on = false;
    while(KeyInfo.scan_code != ENTER);

}