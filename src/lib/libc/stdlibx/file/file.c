#include <fs/xin.h>
#include <lib/libc/file.h>
#include <lib/libc/memory.h>

#warning "TODO what if not opened";
char* getline(XinEntry* File, int line_id)
{
    if (File == NULL) return NULL;
    fread(File, NULL, File->size); // loads all data to buffer

    char* file_data = (char*)(File->FileInfo->buffer);
    int current_line = 0;
    int line_size = 0;

    for (; *file_data != '\0'; file_data++)
    {
        if (current_line == line_id)
        {
            for (int i = 0; file_data[i] != '\n' && file_data[i] != '\0'; i++, line_size++)
                ;

            char* line = (char*)calloc(line_size + sizeof('\0'));
            memcpy(line, file_data, line_size);
            return line;
        }
        if (*file_data == '\n')
        {
            current_line++;
        }
    }

    return NULL;
}
