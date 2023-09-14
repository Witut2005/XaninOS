#pragma once

extern int cpp_test(void);


#include <programs/fs/copy_file.c>
#include <programs/fs/create_file.c>
#include <programs/fs/create_folder.c>
#include <programs/fs/entry_remove.c>
#include <programs/fs/folder_change.c>
#include <programs/fs/folder_remove.c>
#include <programs/fs/link_create.c>
#include <programs/fs/list_files.c>
#include <programs/fs/move.c>
#include <programs/misc/tetris/tetris.c>
#include <programs/misc/start_screen.c>
#include <programs/power/shutdown.c>
#include <programs/internals/load.c>
#include <programs/internals/loadch.c>
#include <programs/misc/epilepsy.c>
#include <programs/tests/keyboard_test.c>
#include <programs/misc/logo.c>
#include <programs/internals/register_dump.c>
#include <programs/internals/cpu_info.c>
#include <programs/misc/calc.c>
#include <programs/misc/pong/pong.c>
#include <programs/tests/timer_test.c>
#include <programs/misc/zsk.c>

#include <fs/loaders/bin/bit32/run.c>
#include <fs/loaders/bin/bit16/run16.c>
#include <fs/loaders/bin/bit32/execute_addr.c>

#include <programs/developer/disk_load.c>
#include <programs/developer/disk_write.c>
#include <programs/fs/hexeditor/hexeditor.c>

#include <programs/fs/xin_note.c>
#include <programs/fs/xin_paint.c>
#include <programs/fs/cat.c>
#include <programs/fs/xin_info.c>
#include <programs/internals/buffers.c>
#include <programs/tests/terminal_test.c>
#include <programs/misc/ssaver.c>
#include <programs/xgl/xgl_mode_set.c>

int argc;
char* argv[5];
int last_command_exit_status;
extern terminal_t* kernel_terminal;

#define XANIN_ADD_APP_ENTRY0(app_name, exec_name) else if(bstrcmp(argv[0], app_name)) {last_command_exit_status = exec_name();}
#define XANIN_ADD_APP_ENTRY1(app_name, exec_name) else if(bstrcmp(argv[0], app_name)) {last_command_exit_status = exec_name(argv[1]);}
#define XANIN_ADD_APP_ENTRY2(app_name, exec_name) else if(bstrcmp(argv[0], app_name)) {last_command_exit_status = exec_name(argv[1], argv[2]);}
#define XANIN_ADD_APP_ENTRY3(app_name, exec_name) else if(bstrcmp(argv[0], app_name)) {last_command_exit_status = exec_name(argv[1], argv[2], argv[3]);}

extern bool gyn_cl_on;
bool is_external_app = false;

void check_external_apps(void)
{
    char* external_apps_folder = (char*)calloc(ARRAY_LENGTH("/external_apps/"));
    memcpy(external_apps_folder, "/external_apps/", ARRAY_LENGTH("/external_apps/"));

    char* app = (char*)calloc(512);
    memcpy(app, external_apps_folder, strlen(external_apps_folder));

    for(int i = 0; argv[0][i] != '\0'; i++)
        app[ARRAY_LENGTH("/external_apps/") + i - 1] = argv[0][i];

    if(strlen(app) > XIN_MAX_PATH_LENGTH)
    {
        free(external_apps_folder);
        free(app);
        return;
    }

    XinEntry* file = fopen(app, "r");

    if(file != NULL && file->size != 0)
    {
        elfreader(app);
        is_external_app = true;
    }
    free(external_apps_folder);
    free(app);
}

extern uint32_t stdio_refresh_rate; // USE HERE SYSCALL

void scan(void)
{

    all_intervals_clear(); // clear kernel intervals
    interval_set(stdio_refresh, stdio_refresh_rate, NULL); // refresh interval

    last_command_exit_status = XANIN_OK;

    bool stdio_mode_overriden = false;

    if(KeyInfo.is_ctrl)
    {
        puts_warning("Stdio mode override: l(legacy)/t(terminal)\n");
        char stdio_selected_option = getchar();

        if(stdio_selected_option == 'l')
        {
            stdio_mode_set(STDIO_MODE_CANVAS);
            screen_clear();
        }
        stdio_mode_overriden = true;
    }
    
    
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

    if(bstrcmp(argv[0], "\0"))
    {
        if(!is_logo_color_blocked)
        {
            logo_front_color++;
            if(logo_front_color == 16)
                logo_front_color = 1;
        }
    }

    XANIN_ADD_APP_ENTRY0("xt_calls", terminal_calls_test)
    XANIN_ADD_APP_ENTRY0("stdio_apply", stdio_apply)
    XANIN_ADD_APP_ENTRY1("ssaver", screen_saver)
    XANIN_ADD_APP_ENTRY1("bmp_info", bmp_info)
    XANIN_ADD_APP_ENTRY1("dev_tools", dev_tools)
    XANIN_ADD_APP_ENTRY0("netplan_apply", netplan_apply)
    XANIN_ADD_APP_ENTRY0("xgl_mode_set", xgl_mode_set)
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

    else if(bstrcmp(argv[0], "reboot"))
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

    else if(bstrcmp(argv[0],"pwd"))
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
    else if(bstrcmp(argv[0], "ls"))
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
    XANIN_ADD_APP_ENTRY1("realfs", real_mode_fswitch)

    // else if(bstrcmp("real_mode_fswitch", argv[0]))
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
            xprintf("%zunknown command: %s\n", stderr, argv[0]);
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

    if(stdio_mode_overriden) // i want to be careful
        __xtf_buffer_clear(__vty_get());

    // terminal_destroy(app_terminal, kernel_terminal);

}
