

#include <xin_fs/xin.h>
#include <libcpp/string.h>
#include <libcpp/time.hpp>
#include <libcpp/iostream.h>
#include <keyboard/scan_codes.h>
#include <libcpp/cmemory.h>

#define VGA_TEXT_MEMORY 0xb8000
#define VGA_SCREEN_RESOLUTION 4480

extern "C" 
{

int screenshot(void)
{

    char* current_directory = (char*)calloc(40);
    memcpy((uint8_t*)current_directory, (uint8_t*)xin::xin_get_current_directory(), 40);

    std::string filename = "/screenshot/";
    xin::xin_folder_change(filename.c_str());
    
    CmosTime time = std::chrono::system_clock::now();
    xin::xin_create_file(std::chrono::time_to_string(time).c_str());    

    xin_entry* file = xin::fopen(std::chrono::time_to_string(time).c_str(), "rw");

    uint8_t* data = (uint8_t*)VGA_TEXT_MEMORY;
    xin::fwrite(file, data, VGA_SCREEN_RESOLUTION);
    xin::fclose(&file);


    xin::xin_folder_change(current_directory);
    free(current_directory);

    return 0;

}

}