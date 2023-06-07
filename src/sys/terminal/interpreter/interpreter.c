#pragma once

extern int cpp_test(void);

#include <programs/dev_tools.c>
#include <programs/print_to_syslog.c>
#include <sys/terminal/interpreter/interpreter.h>
#include <sys/terminal/interface/terminal.h>
#include <programs/nic_info.c>
#include <sys/gyn_cl/gyn.c>
#include <programs/xin_xpaint.c>
#include <programs/xgl_test.c>
#include <programs/arp_table_print.c>
#include <programs/ping.c>
#include <programs/ip_test.c>
#include <programs/udp_test.c>
#include <lib/libc/real_mode_fswitch.h>
#include <programs/arp_check.c>
#include <programs/dhcp_test.c>
#include <programs/nic_rename.c>
#include <programs/nic_print.c>
#include <programs/zsk.c>
#include <programs/explorer.c>
#include <programs/memory_allocator_test.c>
#include <programs/grapher/grapher.c>
#include <programs/usb_controller_info.c>
#include <fs/loaders/elf/elf_loader.c>
#include <fs/loaders/elf/elfdump.c>
#include <app_config.h>
#include <lib/libc/stdiox.h>
#include <programs/help.c>
#include <programs/netapi_check.c>
#include <programs/netplan_apply.c>
#include <programs/interrupt_test.c>
#include <programs/idt_examine.c>
#include <programs/load_file.c>
#include <programs/file_system/copy_file.c>
#include <programs/file_system/create_file.c>
#include <programs/file_system/create_folder.c>
#include <programs/file_system/entry_remove.c>
#include <programs/file_system/folder_change.c>
#include <programs/file_system/folder_remove.c>
#include <programs/file_system/link_create.c>
#include <programs/file_system/list_files.c>
#include <programs/file_system/move.c>
#include <programs/tetris/tetris.c>
#include <programs/start_screen.c>
#include <programs/shutdown.c>
#include <programs/device_info.c>
#include <programs/load.c>
#include <programs/loadch.c>
#include <programs/epilepsy.c>
#include <programs/keyboard_test.c>
#include <programs/logo.c>
#include <programs/register_dump.c>
#include <programs/cpu_info.c>
#include <programs/calc.c>
#include <programs/pong/pong.c>
#include <programs/timer_test.c>
#include <programs/zsk.c>
#include <programs/execute_addr.c>
#include <programs/disk_load.c>
#include <programs/disk_write.c>
#include <programs/run.c>
#include <programs/run16.c>
#include <programs/hexeditor/hexeditor.c>
#include <programs/mouse.c>
#include <programs/xin_note.c>
#include <programs/xin_paint.c>
#include <programs/file_system/cat.c>
#include <programs/file_system/xin_info.c>
#include <programs/buffers.c>
#include <programs/terminal_test.c>

int argc;
char* argv[5];
int last_command_exit_status;
extern terminal_t* kernel_terminal;

#define XANIN_ADD_APP_ENTRY0(app_name, exec_name) else if(bstrcmp(program_name, app_name)) {last_command_exit_status = exec_name();}
#define XANIN_ADD_APP_ENTRY1(app_name, exec_name) else if(bstrcmp(program_name, app_name)) {last_command_exit_status = exec_name(program_parameters);}
#define XANIN_ADD_APP_ENTRY2(app_name, exec_name) else if(bstrcmp(program_name, app_name)) {last_command_exit_status = exec_name(program_parameters, program_parameters1);}
#define XANIN_ADD_APP_ENTRY3(app_name, exec_name) else if(bstrcmp(program_name, app_name)) {last_command_exit_status = exec_name(program_parameters, program_parameters1, program_parameters2);}

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

    if(strlen(app) > XIN_MAX_PATH_LENGTH)
        return;

    XinEntry* file = fopen(app, "r");

    if(file != NULL && file->size != 0)
    {
        elfreader(app);
        is_external_app = true;
    }
    free(external_apps_folder);
    free(app);
}


void scan(void)
{

    screen_clear();
    all_intervals_clear(); // clear kernel intervals
    last_command_exit_status = XANIN_OK;
    
    //legacy reasons
    KeyInfo.scan_code = 0;
    KeyInfo.character = 0;
    argc = 0;
    
    for(int i = 0; i < 5; i++)
    {
        if(!strlen(argv[i]))
            break;
        argc++;
    }


    // terminal_t* app_terminal = terminal_create();
    // terminal_set(kernel_terminal, app_terminal);
    
    // for(int i = 0; i < VGA_SCREEN_RESOLUTION; i++)
    //     app_terminal->buffer[i] = 0x0;

    if(bstrcmp(program_name, "\0"))
    {
        if(!is_logo_color_blocked)
        {
            logo_front_color++;
            if(logo_front_color == 16)
                logo_front_color = 1;
        }
    }

    XANIN_ADD_APP_ENTRY1("dev_tools", dev_tools)
    XANIN_ADD_APP_ENTRY0("netplan_apply", netplan_apply)
    XANIN_ADD_APP_ENTRY0("ttest", terminal_test)
    XANIN_ADD_APP_ENTRY0("buffers", buffers_view)
    XANIN_ADD_APP_ENTRY1("netapi_check", netapi_check)
    XANIN_ADD_APP_ENTRY0("usb_info", usb_controller_info)
    XANIN_ADD_APP_ENTRY0("memtest", memory_test)
    XANIN_ADD_APP_ENTRY0("grapher", grapher)
    XANIN_ADD_APP_ENTRY0("petris", ptetris)
    XANIN_ADD_APP_ENTRY0("ptetris", ptetris)
    XANIN_ADD_APP_ENTRY1("explorer", explorer)
    XANIN_ADD_APP_ENTRY1("int", interrupt_test)
    XANIN_ADD_APP_ENTRY0("idt", idt_examine)
    XANIN_ADD_APP_ENTRY0("start_screen", start_screen)
    XANIN_ADD_APP_ENTRY0("screensaver", start_screen)
    XANIN_ADD_APP_ENTRY2("load_file", load_file)
    XANIN_ADD_APP_ENTRY0("mouse_test", mouse_test)
    XANIN_ADD_APP_ENTRY1("help", help)
    XANIN_ADD_APP_ENTRY1("printk", print_to_syslog)
    XANIN_ADD_APP_ENTRY1("h", help)
    XANIN_ADD_APP_ENTRY0("timer_test", timer_test)
    XANIN_ADD_APP_ENTRY1("cat", cat)
    XANIN_ADD_APP_ENTRY1("zsk", zsk)
    XANIN_ADD_APP_ENTRY0("epilepsy", epilepsy)
    XANIN_ADD_APP_ENTRY1("elft", elfreader)
    XANIN_ADD_APP_ENTRY1("elf", elfreader)
    XANIN_ADD_APP_ENTRY1("elfdump", elfdump)
    XANIN_ADD_APP_ENTRY2("hexeditor", hexeditor)
    XANIN_ADD_APP_ENTRY1("run16", run16)
    XANIN_ADD_APP_ENTRY1("run", run)
    XANIN_ADD_APP_ENTRY0("cpptest", cpp_test)
    XANIN_ADD_APP_ENTRY1("cpu_info", cpu_info)
    XANIN_ADD_APP_ENTRY1("calc", calc)
    XANIN_ADD_APP_ENTRY1("paint", xin_paint)
    XANIN_ADD_APP_ENTRY0("reg_dump", reg_dump)
    XANIN_ADD_APP_ENTRY1("load", load)
    XANIN_ADD_APP_ENTRY3("disk_load", disk_load)
    XANIN_ADD_APP_ENTRY3("disk_write", disk_write_data)
    XANIN_ADD_APP_ENTRY1("execute_addr", execute_addr)
    XANIN_ADD_APP_ENTRY0("key-test", keyboard_test)
    XANIN_ADD_APP_ENTRY0("pong", pong)
    XANIN_ADD_APP_ENTRY1("loadch", loadch)
    XANIN_ADD_APP_ENTRY0("dev-info", get_device_info)

    else if(bstrcmp(program_name, "reboot"))
    {

            
        static uint16_t idt_16[3] = {0x0, 0x0, 0x0};

        free(kernel_terminal);
        screen_clear();

        disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 5, (uint16_t*)(0x800));
        disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a, 20, (uint16_t*)(0x1800));
        interrupt_enable();

        asm("lidt %0" :: "m"(idt_16));
        
        uint8_t s = 12;
        s = s / 0;


    }
    XANIN_ADD_APP_ENTRY1("create", xin_create_file_app)
    XANIN_ADD_APP_ENTRY1("mkdir", xin_create_folder_app)
    XANIN_ADD_APP_ENTRY0("shutdown", shutdown)
    XANIN_ADD_APP_ENTRY1("rm", xin_entry_remove_app)
    XANIN_ADD_APP_ENTRY1("rd", xin_folder_remove_app)
    XANIN_ADD_APP_ENTRY1("cd", xin_folder_change_app)

    else if(bstrcmp(program_name,"pwd"))
    {
        xprintf("%s\n", xin_current_directory);
        while(inputg().scan_code != ENTER);
    }

    XANIN_ADD_APP_ENTRY2("move", xin_move_entry_app)
    XANIN_ADD_APP_ENTRY2("mv", xin_move_entry_app)
    XANIN_ADD_APP_ENTRY2("copy", xin_copy_app)
    XANIN_ADD_APP_ENTRY2("cp", xin_copy_app)
    XANIN_ADD_APP_ENTRY2("link", xin_link_create_app)
    XANIN_ADD_APP_ENTRY2("lk", xin_link_create_app)
    XANIN_ADD_APP_ENTRY1("xin_info", xin_info)
    XANIN_ADD_APP_ENTRY0("cls", screen_clear)
    XANIN_ADD_APP_ENTRY0("clear", screen_clear)
    else if(bstrcmp(program_name, "ls"))
    {
        xin_list_files_app(argv);
    }
    XANIN_ADD_APP_ENTRY1("note", xin_note)
    XANIN_ADD_APP_ENTRY2("logo", logo_color_change)
    XANIN_ADD_APP_ENTRY1("nic_info", nic_info)
    XANIN_ADD_APP_ENTRY1("gyn", gyn_interpreter)
    XANIN_ADD_APP_ENTRY0("screenshot", screenshot)
    XANIN_ADD_APP_ENTRY1("xpaint", xin_xpaint)
    XANIN_ADD_APP_ENTRY0("xgm", xagame_test)
    XANIN_ADD_APP_ENTRY0("tetris", tetris)
    XANIN_ADD_APP_ENTRY0("xgl_test", xgl_test)
    XANIN_ADD_APP_ENTRY2("arp", arp_table_print)
    XANIN_ADD_APP_ENTRY1("ping", ping)
    XANIN_ADD_APP_ENTRY1("ip_test", ip_test)
    XANIN_ADD_APP_ENTRY1("udp_test", udp_test)
    XANIN_ADD_APP_ENTRY1("edit", edit)
    XANIN_ADD_APP_ENTRY1("arp_check", arp_check)
    XANIN_ADD_APP_ENTRY0("dhcp_test", dhcp_test)
    XANIN_ADD_APP_ENTRY2("nic_rename", nic_rename)
    XANIN_ADD_APP_ENTRY0("nic_print", nic_print)
    XANIN_ADD_APP_ENTRY0("grapher", grapher)

    // else if(bstrcmp("real_mode_fswitch", program_name))
    // {
    //     XinEntry* real_mode_enter = fopen("/fast_real_mode_enter.bin", "r");
    //     disk_read(ATA_FIRST_BUS, ATA_MASTER, real_mode_enter->first_sector, 1, (uint16_t*)0x600);

    //     XinEntry* real_mode_return = fopen("/fast_real_mode_return.bin", "r");
    //     disk_read(ATA_FIRST_BUS, ATA_MASTER, real_mode_return->first_sector, 1, (uint16_t*)0x400);
    //     real_mode_fswitch("0x0", "0x0");
    // }

    else
    {

        check_external_apps();
        
        if(!is_external_app)
        {
            xprintf("%zunknown command: %s\n", stderr, program_name);
            screen_background_color_set(red);
            msleep(400);
            last_command_exit_status = XANIN_ERROR;
        }

        is_external_app = false;
        keyboard_handle = NULL;
        app_exited = true;
        return;
    }

    // if(last_command_exit_status != XANIN_OK)
    // {
    //     xprintf("Command execution status: %z%s", stderr, app_exit_status_text_get());
    //     screen_background_color_set(red);
    //     msleep(800);
    // }


    keyboard_handle = NULL;
    app_exited = true;

    // terminal_destroy(app_terminal, kernel_terminal);

}
