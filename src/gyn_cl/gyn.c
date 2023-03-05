
#pragma once

#include <gyn_cl/gyn.h>

bool gyn_cl_on = false;

int gyn_interpreter(char* file_to_interpret)
{
    gyn_cl_on = true;
    XinEntry* file = fopen(file_to_interpret, "r");
    uint8_t* data = (uint8_t*)calloc(SECTOR_SIZE);
    fread(file, data, SECTOR_SIZE);
    char* command;

    if(file == NULL)
    {
        xprintf("%zCouldn't open file %s\n", stderr, file_to_interpret);
        while(inputg().scan_code != ENTER);
        return XANIN_ERROR;
    }

    for(int i = 1; getline(file, i) != NULL; i++)
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

        // xprintf("%s %s", argv[0], argv[1]);
        // while(getscan() != ENTER);

        scan();
        
        if(last_command_exit_status == XANIN_ERROR)
        {
            xprintf("\n%zGYN COMMAND PARSING ERROR: %s\n", stderr, command);
            while(inputg().scan_code != ENTER);
            fclose(&file);
            gyn_cl_on = false;
            return XANIN_ERROR;
        }
        
        free(command);

    }

    while(inputg().scan_code != ENTER);
    fclose(&file);
    gyn_cl_on = false;
    return XANIN_OK;

}
