

#include <fs/xin.h>
#include <lib/libcpp/string.h>
#include <lib/libcpp/time.hpp>
#include <lib/libcpp/iostream.h>
#include <sys/devices/keyboard/scan_codes.h>
#include <lib/libc/memory.h>
#include <lib/libc/stdiox.h>

extern "C" 
{

int screenshot(void)
{

    char* current_directory = (char*)calloc(40);

    xin::xin_get_current_directory(current_directory);

    std::string filename = "/screenshot/";
    xin::xin_folder_change(filename.c_str());
    
    CmosTime time = std::chrono::system_clock::now();
    xin::xin_file_create(std::chrono::time_to_string(time).c_str());    

    XinEntry* file = xin::fopen(std::chrono::time_to_string(time).c_str(), "rw");

    uint8_t* data = (uint8_t*)VGA_TEXT_MEMORY;
    xin::fwrite(file, data, VGA_SCREEN_RESOLUTION);
    xin::fclose(&file);


    xin::xin_folder_change(current_directory);
    free(current_directory);

    return 0;

}

}