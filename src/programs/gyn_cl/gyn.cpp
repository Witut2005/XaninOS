
#include "./gyn.hpp"
#include <lib/libcpp/container/map.hpp>

#define XANIN_ADD_APP_ENTRY0(app_name, exec_name) \
    else if (bstrcmp(argv[0], app_name))          \
    {                                             \
        last_command_exit_status = exec_name();   \
    }
#define XANIN_ADD_APP_ENTRY1(app_name, exec_name)      \
    else if (bstrcmp(argv[0], app_name))               \
    {                                                  \
        last_command_exit_status = exec_name(argv[1]); \
    }
#define XANIN_ADD_APP_ENTRY2(app_name, exec_name)               \
    else if (bstrcmp(argv[0], app_name))                        \
    {                                                           \
        last_command_exit_status = exec_name(argv[1], argv[2]); \
    }
#define XANIN_ADD_APP_ENTRY3(app_name, exec_name)                        \
    else if (bstrcmp(argv[0], app_name))                                 \
    {                                                                    \
        last_command_exit_status = exec_name(argv[1], argv[2], argv[3]); \
    }


//wrzuc xanin internal apps do plikow
//_start
int gyn_interpreter(char** argv)
{
    
    std::UnorderedMap<std::string, void(*)(char**, int)> XaninInternalCommands = { 
        {"stdio_apply", stdio_apply},
        {"ssaver", screen_saver},
        {"bmp_info", bmp_info},
        {"dev_tools", dev_tools},
        {"netplan_apply", netplan_apply},
        {"xgl_mode_set", xgl_mode_set},
        {"buffers", buffers_view},
        {"netapi_check", netapi_check},
        {"usb_info", usb_controller_info},
        {"memtest", memory_test},
        {"grapher", grapher},
        {"petris", ptetris},
        {"ptetris", ptetris},
        {"explorer", explorer},
        {"int", interrupt_test},
        {"idt", idt_examine},
        {"start_screen", start_screen},
        {"screensaver", start_screen},
        {"load_file", load_file},
        {"help", help},
        {"printk", print_to_syslog},
        {"h", help},
        {"timer_test", timer_test},
        {"cat", cat},
        {"zsk", zsk},
        {"epilepsy", epilepsy},
        {"elft", elfreader},
        {"elf", elfreader},
        {"elfdump", elfdump},
        {"hexeditor", hexeditor},
        {"run16", run16},
        {"run", run},
        {"cpptest", cpp_test},
        {"cpu_info", cpu_info},
        {"calc", calc},
        {"paint", xin_paint},
        {"reg_dump", reg_dump},
        {"load", load},
        {"disk_load", disk_load},
        {"disk_write", disk_write_data},
        {"execute_addr", execute_addr},
        {"pong", pong},
        {"loadch", loadch},
        {"reboot", reboot},
        {"create", xin_create_file_app},
        {"mkdir", xin_create_folder_app},
        {"shutdown", shutdown},
        {"rm", xin_entry_remove_app},
        {"rd", xin_folder_remove_app},
        {"cd", xin_folder_change_app},
        {"pwd", pwd},
        {"move", xin_move_entry_app},
        {"mv", xin_move_entry_app},
        {"copy", xin_copy_app},
        {"cp", xin_copy_app},
        {"link", xin_link_create_app},
        {"lk", xin_link_create_app},
        {"xin_info", xin_info},
        {"cls", screen_clear},
        {"clear", screen_clear},
        {"ls", xin_list_files_app},
        {"note", xin_note},
        {"logo", logo_color_chang},
        {"nic_info", nic_info},
        {"gyn", gyn_interpreter},
        {"screenshot", screenshot},
        {"xpaint", xin_xpaint},
        {"xgm", xagame_test},
        {"tetris", tetris},
        {"xgl_test", xgl_test},
        {"arp", arp_table_print},
        {"ping", ping},
        {"ip_test", ip_test},
        {"udp_test", udp_test},
        {"edit", edit},
        {"arp_check", arp_check},
        {"dhcp_test", dhcp_test},
        {"nic_rename", nic_rename},
        {"nic_print", nic_print},
        {"grapher", grapher}
    };

    // XaninInternalCommands[argv[0]]() 

}