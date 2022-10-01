#pragma once

#include <terminal/interpreter.h>


#define XANIN_ADD_APP_ENTRY0(app_name, exec_name) else if(strcmp(program_name, app_name)) {exec_name();}
#define XANIN_ADD_APP_ENTRY1(app_name, exec_name) else if(strcmp(program_name, app_name)) {exec_name(program_parameters);}
#define XANIN_ADD_APP_ENTRY2(app_name, exec_name) else if(strcmp(program_name, app_name)) {exec_name(program_parameters, program_parameters1);}
#define XANIN_ADD_APP_ENTRY3(app_name, exec_name) else if(strcmp(program_name, app_name)) {exec_name(program_parameters, program_parameters1, program_parameters2);}


void scan(void)
{

    
    if(strcmp(program_name, "\0"))
    {
        logo_front_color++;
        if(logo_front_color == 16)
            logo_front_color = 1;

        app_exited = true;
    }


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

    #ifdef EPILEPSY_APP
    XANIN_ADD_APP_ENTRY0("epilepsy", epilepsy)
    #endif

    #ifdef ELF_LOADER_APP 
    XANIN_ADD_APP_ENTRY1("elft", elfreader)
    #endif

    #ifdef HEXEDITOR_APP
    XANIN_ADD_APP_ENTRY1("hexeditor", hexeditor)
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

    #ifdef STACK_FRAME_TEST_APP
    XANIN_ADD_APP_ENTRY2("stack_test", stack_frame_test)
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

    #ifdef INSTALL_APP
    XANIN_ADD_APP_ENTRY0("install", install)
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

    XANIN_ADD_APP_ENTRY1("create", __sys_xin_file_create)
    XANIN_ADD_APP_ENTRY1("mkdir", __sys_xin_folder_create)

    #ifdef SHUTDOWN_APP
    XANIN_ADD_APP_ENTRY0("shutdown", shutdown)
    #endif
    
    XANIN_ADD_APP_ENTRY1("rm", __sys_xin_entry_remove)
    XANIN_ADD_APP_ENTRY1("rd", __sys_xin_folder_remove)
    XANIN_ADD_APP_ENTRY1("cd", __sys_xin_folder_change)

    else if(strcmp(program_name,"pwd"))
    {
        xprintf("%s\n", xin_current_directory);
        while(KeyInfo.scan_code != ENTER);
    }

    // XANIN_ADD_APP_ENTRY1("pf", xin_get_file_pf_test)

    #ifdef XIN_MOVE_APP
    XANIN_ADD_APP_ENTRY2("move", __sys_xin_entry_move)
    XANIN_ADD_APP_ENTRY2("mv", __sys_xin_entry_move)
    #endif

    #ifdef XIN_COPY_APP
    XANIN_ADD_APP_ENTRY2("copy", xin_copy)
    XANIN_ADD_APP_ENTRY2("cp", xin_copy)
    #endif

    #ifdef XIN_LINK_APP
    XANIN_ADD_APP_ENTRY2("link", xin_link_create)
    XANIN_ADD_APP_ENTRY2("lk", xin_link_create)
    #endif

    #ifdef XIN_INFO_APP
    XANIN_ADD_APP_ENTRY1("xin_info", xin_entry_info)
    #endif

    XANIN_ADD_APP_ENTRY0("cls", screen_clear)
    XANIN_ADD_APP_ENTRY0("clear", screen_clear)

    #ifdef LIST_FILES_APP
    XANIN_ADD_APP_ENTRY1("ls", list_files)
    XANIN_ADD_APP_ENTRY1("dir", list_files)
    #endif

    #ifdef NOTE_APP
    XANIN_ADD_APP_ENTRY1("note", xin_note)
    #endif

    #ifdef LOGO_APP
    XANIN_ADD_APP_ENTRY1("logo", logo_color_change)
    #endif

    XANIN_ADD_APP_ENTRY0("nic_info", nic_info)

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
    XANIN_ADD_APP_ENTRY1("edit", edit)
    XANIN_ADD_APP_ENTRY1("icmp_test", icmp_test)
    XANIN_ADD_APP_ENTRY1("arp_check", arp_check)
    XANIN_ADD_APP_ENTRY0("dhcp_test", dhcp_test)

    else if(strcmp("real_mode_fswitch", program_name))
    {
        xin_entry* real_mode_enter = fopen("/fast_real_mode_enter.bin", "r");
        disk_read(ATA_FIRST_BUS, ATA_MASTER, real_mode_enter->starting_sector, 1, (uint16_t*)0x600);

        xin_entry* real_mode_return = fopen("/fast_real_mode_return.bin", "r");
        disk_read(ATA_FIRST_BUS, ATA_MASTER, real_mode_return->starting_sector, 1, (uint16_t*)0x400);
        real_mode_fswitch("0x0", "0x0");
    }

    else
    {
        xprintf("%zUnknown command",set_output_color(red,white));
        logo_front_color = red;
    }


    keyboard_handle = nullptr;
    app_exited = true;
    KeyInfo.character = 0x0;
    KeyInfo.scan_code = 0x0;

}
