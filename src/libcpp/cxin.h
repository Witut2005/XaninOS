
#include <xin_fs/xin_entry.h>
#include <stddef.h>
#include <stdint.h>

extern "C"
{

    namespace xin
    {

    size_t read(xin_entry* entry, void* buf, size_t count);
    size_t write(xin_entry* entry, void* buf, size_t count);
    void create_file(char* entry_name);

    }

    xin_entry* xin_change_directory(char *new_directory);
    char* xin_get_current_directory(void);
    void fseek(xin_entry* file, uint32_t new_position);
    uint32_t fteel(xin_entry* file);
    xin_entry* fopen(char* file_path, const char* mode);
    void fclose(xin_entry** file); 


}
