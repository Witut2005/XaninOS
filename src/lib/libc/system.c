#include <lib/libc/stdlibx.h>
#include <lib/libc/string.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/file.h>

char *system_variable_get(char *variable)
{
    XinEntry *File = fopen("/etc/var/variables.conf", "r");
    char *line = getline(File, 0);

    for (int i = 0; line != NULL;)
    {
        if (bstrncmp(line, variable, strlen(variable) - 1))
        {
            char *buf = calloc(strlen(line) - strlen(variable));
            strcpy(buf, &line[strlen(variable) + 1]);
            free(line);
            return buf;
        }
        line = getline(File, ++i);
    }

    fclose(&File);
    free(line);
    return NULL;
}
