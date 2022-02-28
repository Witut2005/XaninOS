#include <fat/fat_driver.h>
#include <lib/stdiox.h>
#include <lib/algorithm.h>
#include <lib/string.h>
#include <lib/exit_process.h>

void cdf(char* folder_name)
{

    root_directory_entry* folder = find_file(folder_name);

    if(folder == nullptr)
    {
        xprintf("%zNO SUCH FOLDER\n", set_output_color(red,white));
        while(keyboard_scan_code != ENTER);
        exit_process();
    }

    fat.current_folder = folder->starting_cluster;

    xprintf("folder: %d\n", fat.current_folder);

    while(keyboard_scan_code != ENTER);
    exit_process();

}
