
#include <xin_fs/xin_entry.h>
#include <stddef.h>
#include <stdint.h>

extern "C"
{

    size_t read(xin_entry* entry, void* buf, size_t count);
    size_t write(xin_entry* entry, void* buf, size_t count);
    void fseek(xin_entry* file, uint32_t new_position);
    uint32_t fteel(xin_entry* file);
    xin_entry* fopen(char* file_path, const char* mode);
    void fclose(xin_entry** file); 


}
