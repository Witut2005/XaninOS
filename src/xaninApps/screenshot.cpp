

#include <libcpp/cxin.h>
#include <libcpp/string.h>
#include <libcpp/time.hpp>
#include <libcpp/iostream.h>
#include <keyboard/scan_codes.h>

#define VGA_TEXT_MEMORY 0xb8000
#define VGA_SCREEN_RESOLUTION 4480

extern "C" 
{

void screenshot(void)
{

    std::string current_directory = xin_get_current_directory();

    std::string filename = "/screenshot/";
    xin_change_directory(filename.c_str());
    
    CmosTime time = std::chrono::system_clock::now();
    xin::create_file(std::chrono::time_to_string(time).c_str());    

    xin_entry* file = fopen(std::chrono::time_to_string(time).c_str(), "rw");

    uint8_t* data = (uint8_t*)VGA_TEXT_MEMORY;

    for(int i = 0; i < VGA_SCREEN_RESOLUTION; i++)
        xin::write(file, &data[i], sizeof(uint8_t));

    xin_change_directory(current_directory.c_str());
    while(getscan() != ENTER);

}

}