

#include <libc/stdlibx.h>
#include <libc/string.h>
#include <xin_fs/xin.h>
#include <libc/stdiox.h>

void system_variable_get(char** buf, char* value)
{
    XinEntry* file = fopen("/config/variables.conf", "r");

    char* line = calloc(512);
    //for(int i = 1; getline(file,i) != NULL; i++)
    {
        char* line = getline(file, 1);
        xprintf("\n%s", line);

        if(strncmp(line, value, strlen(value) - 1))
        {
            for(int j = strlen(value); j < strlen(line); j++)
                (*buf)[j - strlen(value)] = line[j];
            free(line);
            return;
        }
    }
    
    free(line);
    strcpy(*buf, "none");

}
