
#include "endian.h"
#include <stdarg.h>
#include <stdint.h>
#include <lib/libc/math.h>
#include <lib/libc/colors.h>
#include <lib/libc/memory.h>
#include <lib/libc/string.h>
#include <lib/libc/stdlibx.h>
#include <lib/screen/screen.h>
#include <sys/devices/com/com.h>
#include <lib/libcpp/container/vector/vector.hpp>

#define ASCII_CASE_OFFSET 32 
#define EXIT_ON_EQUALS_ZERO(val, return_value) if(val == 0) {return return_value;}

static uint32_t string_errno;

extern "C"
{
    #warning "TODO what if not opened";
    char* getline(XinEntry* File, int line_id)
    {
        if (File == nullptr) return nullptr;
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
} // extern "C"