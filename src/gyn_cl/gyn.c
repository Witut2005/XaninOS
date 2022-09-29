
#pragma once

#include <gyn_cl/gyn.h>

void gyn_interpreter(char* file_to_interpret)
{

    xin_entry* file = fopen(file_to_interpret, "r");
    char* command;

    if(file == nullptr)
    {
        xprintf("%zCouldn't open file %s\n", stderr, file_to_interpret);
        while(KeyInfo.scan_code != ENTER);
        return;
    }

        
    gyn_cl_on = true;
    for(int i = 1; getline(file, i)[0] != '\0'; i++)
    {

        for(int i = 0; i < 5; i++)
            memset(argv[i], '\0', 40);


        command = (uint8_t*)calloc(strlen(getline(file, i)) + 1);
        command = getline(file, i);

        int arg_counter = 0;
        int arg_command_counter = 0;


        for(int i = 0; command[i] != '\0'; i++)
        {
            if(command[i] == ' ')
            {
                arg_counter++;
                argv[arg_counter][arg_command_counter] = '\0';
                arg_command_counter = -1;
            }

            else
                argv[arg_counter][arg_command_counter] = command[i];
            arg_command_counter++;
        }

        scan();
        free(command);

    }

    gyn_cl_on = false;
    while(KeyInfo.scan_code != ENTER);

}
