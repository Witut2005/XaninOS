#pragma once

#include <terminal/interpreter.h>
#include <terminal/terminal.h>

int last_command_exit_status;
extern terminal_t* kernel_terminal;

#define XANIN_ADD_APP_ENTRY0(app_name, exec_name) else if(strcmp(program_name, app_name)) {last_command_exit_status = exec_name();}
#define XANIN_ADD_APP_ENTRY1(app_name, exec_name) else if(strcmp(program_name, app_name)) {last_command_exit_status = exec_name(program_parameters);}
#define XANIN_ADD_APP_ENTRY2(app_name, exec_name) else if(strcmp(program_name, app_name)) {last_command_exit_status = exec_name(program_parameters, program_parameters1);}
#define XANIN_ADD_APP_ENTRY3(app_name, exec_name) else if(strcmp(program_name, app_name)) {last_command_exit_status = exec_name(program_parameters, program_parameters1, program_parameters2);}

extern bool gyn_cl_on;
bool is_external_app = false;

void check_external_apps(void)
{
    char* external_apps_folder = (char*)calloc(ARRAY_LENGTH("/external_apps/"));
    memcpy(external_apps_folder, "/external_apps/", ARRAY_LENGTH("/external_apps/"));

    char* app = (char*)calloc(512);
    memcpy(app, external_apps_folder, strlen(external_apps_folder));

    for(int i = 0; program_name[i] != '\0'; i++)
        app[ARRAY_LENGTH("/external_apps/") + i - 1] = program_name[i];

    XinEntry* file = fopen(app, "r");

    if(file != nullptr && file->entry_size != 0)
    {
        elfreader(app);
        is_external_app = true;
    }
    free(app);
}


void scan(void)
{

    screen_clear();
    // terminal_t* app_terminal = terminal_create();
    // terminal_set(kernel_terminal, app_terminal);
    
    // for(int i = 0; i < VGA_SCREEN_RESOLUTION; i++)
    //     app_terminal->buffer[i] = 0x0;

    if(strcmp(program_name, "\0"))
    {
        if(!is_logo_color_blocked)
        {
            logo_front_color++;
            if(logo_front_color == 16)
                logo_front_color = 1;
        }
    }

    XANIN_ADD_APP_ENTRY0("memtest", memory_test)
    // XANIN_ADD_APP_ENTRY0("grapher", grapher)
    XANIN_ADD_APP_ENTRY0("petris", ptetris)
    XANIN_ADD_APP_ENTRY0("ptetris", ptetris)
    XANIN_ADD_APP_ENTRY1("explorer", explorer)

    XANIN_ADD_APP_ENTRY0("start_screen", start_screen)
    XANIN_ADD_APP_ENTRY0("screensaver", start_screen)
    XANIN_ADD_APP_ENTRY2("load_file", load_file)

    #ifdef HELP_APP
    XANIN_ADD_APP_ENTRY1("help", help)
    XANIN_ADD_APP_ENTRY1("h", help)
    #endif

    #ifdef TIMER_TEST_APP
    XANIN_ADD_APP_ENTRY0("timer_test", timer_test)
    #endif

    #ifdef CAT_APP
    XANIN_ADD_APP_ENTRY1("cat", cat)
    #endif

    XANIN_ADD_APP_ENTRY1("zsk", zsk)

    #ifdef EPILEPSY_APP
    XANIN_ADD_APP_ENTRY0("epilepsy", epilepsy)
    #endif

    #ifdef ELF_LOADER_APP 
    XANIN_ADD_APP_ENTRY1("elft", elfreader)
    XANIN_ADD_APP_ENTRY1("elf", elfreader)
    XANIN_ADD_APP_ENTRY1("elfdump", elfdump)
    #endif

    #ifdef HEXEDITOR_APP
    XANIN_ADD_APP_ENTRY2("hexeditor", hexeditor)
    #endif

    #ifdef RUN16_APP
    XANIN_ADD_APP_ENTRY1("run16", run16)
    #endif

    #ifdef RUN_APP
    XANIN_ADD_APP_ENTRY1("run", run)
    #endif

    #ifdef CPP_TEST_APP
    XANIN_ADD_APP_ENTRY0("test", cpp_test)
    #endif

    #ifdef CPU_INFO_APP
    XANIN_ADD_APP_ENTRY1("cpu_info", cpu_info)
    #endif

    #ifdef CALC_APP
    XANIN_ADD_APP_ENTRY1("calc", calc)
    #endif

    #ifdef PAINT_APP
    XANIN_ADD_APP_ENTRY1("paint", xin_paint)
    #endif

    #ifdef REGISTER_DUMP_APP
    XANIN_ADD_APP_ENTRY0("reg_dump", reg_dump)
    #endif

    #ifdef LOAD_APP
    XANIN_ADD_APP_ENTRY1("load", load)
    #endif

    #ifdef DISK_LOAD_APP
    XANIN_ADD_APP_ENTRY3("disk_load", disk_load)
    #endif

    #ifdef DISK_WRITE_APP
    XANIN_ADD_APP_ENTRY3("disk_write", disk_write_data)
    #endif

    #ifdef EXECUTE_ADDR_APP
    XANIN_ADD_APP_ENTRY1("execute_addr", execute_addr)
    #endif

    #ifdef KEYBOARD_TEST_APP
    XANIN_ADD_APP_ENTRY0("key-test", keyboard_test)
    #endif

    #ifdef PONG_APP
    XANIN_ADD_APP_ENTRY0("pong", pong)
    #endif

    #ifdef LOADCH_APP
    XANIN_ADD_APP_ENTRY1("loadch", loadch)
    #endif

    #ifdef DEVICE_INFO_APP
    XANIN_ADD_APP_ENTRY0("dev-info", get_device_info)
    #endif

    #ifdef REBOOT_APP
    XANIN_ADD_APP_ENTRY0("reboot", reboot)
    #endif

    // XANIN_ADD_APP_ENTRY1("create", __sys_xin_file_create)
    XANIN_ADD_APP_ENTRY1("create", xin_create_file_app)
    XANIN_ADD_APP_ENTRY1("mkdir", xin_create_folder_app)

    #ifdef SHUTDOWN_APP
    XANIN_ADD_APP_ENTRY0("shutdown", shutdown)
    #endif
    
    XANIN_ADD_APP_ENTRY1("rm", XinEntry_remove_app)
    XANIN_ADD_APP_ENTRY1("rd", xin_folder_remove_app)
    XANIN_ADD_APP_ENTRY1("cd", xin_folder_change_app)

    else if(strcmp(program_name,"pwd"))
    {
        xprintf("%s\n", xin_current_directory);
        while(KeyInfo.scan_code != ENTER);
    }

    // XANIN_ADD_APP_ENTRY1("pf", xin_get_file_pf_test)

    #ifdef XIN_MOVE_APP
    XANIN_ADD_APP_ENTRY2("move", xin_move_entry_app)
    XANIN_ADD_APP_ENTRY2("mv", xin_move_entry_app)
    #endif

    #ifdef XIN_COPY_APP
    XANIN_ADD_APP_ENTRY2("copy", xin_copy_app)
    XANIN_ADD_APP_ENTRY2("cp", xin_copy_app)
    #endif

    #ifdef XIN_LINK_APP
    XANIN_ADD_APP_ENTRY2("link", xin_link_create_app)
    XANIN_ADD_APP_ENTRY2("lk", xin_link_create_app)
    #endif

    #ifdef XIN_INFO_APP
    XANIN_ADD_APP_ENTRY1("xin_info", xin_info)
    #endif

    XANIN_ADD_APP_ENTRY0("cls", screen_clear)
    XANIN_ADD_APP_ENTRY0("clear", screen_clear)

    #ifdef LIST_FILES_APP
    
    else if(strcmp(program_name, "ls"))
    {
        xin_list_files_app(argv);
    }

    // XANIN_ADD_APP_ENTRY1("ls", __sys_xin_list_files)
    // XANIN_ADD_APP_ENTRY1("dir", __sys_xin_list_files)
    #endif

    #ifdef NOTE_APP
    XANIN_ADD_APP_ENTRY1("note", xin_note)
    #endif

    #ifdef LOGO_APP
    XANIN_ADD_APP_ENTRY2("logo", logo_color_change)
    #endif

    XANIN_ADD_APP_ENTRY1("nic_info", nic_info)

    XANIN_ADD_APP_ENTRY1("gyn", gyn_interpreter)

    XANIN_ADD_APP_ENTRY0("screenshot", screenshot)

    XANIN_ADD_APP_ENTRY1("xpaint", xin_xpaint)

    XANIN_ADD_APP_ENTRY0("xgm", xagame_test)
    XANIN_ADD_APP_ENTRY0("tetris", tetris)
    XANIN_ADD_APP_ENTRY0("xgl_test", xgl_test)

    XANIN_ADD_APP_ENTRY1("arp", arp_table_print)
    XANIN_ADD_APP_ENTRY1("ping", ping)
    XANIN_ADD_APP_ENTRY1("ip_test", ip_test)
    XANIN_ADD_APP_ENTRY1("udp_test", udp_test)
    XANIN_ADD_APP_ENTRY1("sll_test", sll_test)
    XANIN_ADD_APP_ENTRY1("edit", edit)
    XANIN_ADD_APP_ENTRY1("icmp_test", icmp_test)
    XANIN_ADD_APP_ENTRY1("arp_check", arp_check)
    XANIN_ADD_APP_ENTRY0("dhcp_test", dhcp_test)
    XANIN_ADD_APP_ENTRY2("netdevice_rename", network_device_rename)
    XANIN_ADD_APP_ENTRY0("nic_print", nic_print)

    else if(strcmp("real_mode_fswitch", program_name))
    {
        XinEntry* real_mode_enter = fopen("/fast_real_mode_enter.bin", "r");
        disk_read(ATA_FIRST_BUS, ATA_MASTER, real_mode_enter->starting_sector, 1, (uint16_t*)0x600);

        XinEntry* real_mode_return = fopen("/fast_real_mode_return.bin", "r");
        disk_read(ATA_FIRST_BUS, ATA_MASTER, real_mode_return->starting_sector, 1, (uint16_t*)0x400);
        real_mode_fswitch("0x0", "0x0");
    }

    else
    {

        check_external_apps();
        
        if(!is_external_app)
        {
            xprintf("%zUnknown command: %s", stderr, program_name);
            screen_background_color_set(red);
            msleep(400);
            last_command_exit_status = XANIN_ERROR;
        }
        is_external_app = false;
    }


    keyboard_handle = nullptr;
    app_exited = true;
    KeyInfo.character = 0x0;
    KeyInfo.scan_code = 0x0;

    // terminal_destroy(app_terminal, kernel_terminal);

}
