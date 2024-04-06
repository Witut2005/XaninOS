#pragma once

#include <sys/input/input.h>
#include <programs/built-in-apps.h>
#include <fs/loaders/elf/elf_loader.hpp>

extern "C"
{

    int argc;
    char* argv[5];
    int last_command_exit_status;

#define XANIN_ADD_APP_ENTRY0(app_name, exec_name)                                                                      \
    else if (bstrcmp(argv[0], app_name))                                                                               \
    {                                                                                                                  \
        last_command_exit_status = exec_name();                                                                        \
    }
#define XANIN_ADD_APP_ENTRY1(app_name, exec_name)                                                                      \
    else if (bstrcmp(argv[0], app_name))                                                                               \
    {                                                                                                                  \
        last_command_exit_status = exec_name(argv[1]);                                                                 \
    }
#define XANIN_ADD_APP_ENTRY2(app_name, exec_name)                                                                      \
    else if (bstrcmp(argv[0], app_name))                                                                               \
    {                                                                                                                  \
        last_command_exit_status = exec_name(argv[1], argv[2]);                                                        \
    }
#define XANIN_ADD_APP_ENTRY3(app_name, exec_name)                                                                      \
    else if (bstrcmp(argv[0], app_name))                                                                               \
    {                                                                                                                  \
        last_command_exit_status = exec_name(argv[1], argv[2], argv[3]);                                               \
    }

    extern bool gyn_cl_on;

    bool check_external_apps(void)
    {
        std::string external_apps_folder = "/external_apps/";
        std::string app_path = std::string("/external_apps/") + std::string(argv[0]);

        dbg_info(DEBUG_LABEL_XANIN, (std::string("Opening external file ") + app_path).c_str());
        ElfLoader(app_path.c_str()).execute();

        return true;
    }

    extern uint32_t stdio_refresh_rate; // USE HERE SYSCALL
    extern void stdio_refresh(address_t* args);

    void scan(void)
    {

        all_intervals_clear();                                 // clear kernel intervals
        interval_set(stdio_refresh, stdio_refresh_rate, NULL); // refresh interval

        last_command_exit_status = XANIN_OK;

        bool stdio_mode_overriden = false;

        if (__input_is_normal_key_pressed(KBP_LEFT_CONTROL))
        {
            puts_warning("Stdio mode override: l(legacy)/t(terminal)\n");
            char stdio_selected_option = getchar();

            if (stdio_selected_option == 'l')
            {
                stdio_mode_set(STDIO_MODE_CANVAS);
                screen_clear();
            }
            stdio_mode_overriden = true;
        }

        // legacy reasons
        // KeyInfo.scan_code = 0;
        // KeyInfo.character = 0;
        argc = 0;

        for (int i = 0; i < 5; i++)
        {
            if (!strlen(argv[i]))
                break;
            argc++;
        }

        if (bstrcmp(argv[0], "nicho"))
        {
        }

        XANIN_ADD_APP_ENTRY1("xin_check", xin_check)
            XANIN_ADD_APP_ENTRY0("sprintf_test", sprintf_test)
            XANIN_ADD_APP_ENTRY0("disk", disk_func)
            XANIN_ADD_APP_ENTRY0("paging_test", paging_test)
            XANIN_ADD_APP_ENTRY0("stdio_apply", stdio_apply)
            XANIN_ADD_APP_ENTRY0("ssaver", start_screen)
            XANIN_ADD_APP_ENTRY1("bmp_info", bmp_info)
            XANIN_ADD_APP_ENTRY0("netplan_apply", netplan_apply)
            XANIN_ADD_APP_ENTRY0("xgl_mode_set", xgl_mode_set)
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
            XANIN_ADD_APP_ENTRY1("help", help)
            XANIN_ADD_APP_ENTRY1("printk", print_to_syslog)
            XANIN_ADD_APP_ENTRY1("h", help)
            XANIN_ADD_APP_ENTRY0("timer_test", timer_test)
            XANIN_ADD_APP_ENTRY1("cat", cat)
            XANIN_ADD_APP_ENTRY1("zsk", zsk)
            XANIN_ADD_APP_ENTRY0("epilepsy", epilepsy)
            XANIN_ADD_APP_ENTRY1("elfdump", elfdump)
            XANIN_ADD_APP_ENTRY2("hexeditor", hexeditor)
            XANIN_ADD_APP_ENTRY1("run16", run16)
            XANIN_ADD_APP_ENTRY1("run", run)
            XANIN_ADD_APP_ENTRY0("cpptest", cpp_test)
            XANIN_ADD_APP_ENTRY0("ctest", c_test)
            XANIN_ADD_APP_ENTRY1("cpu_info", cpu_info)
            XANIN_ADD_APP_ENTRY1("calc", calc)
            XANIN_ADD_APP_ENTRY1("paint", xin_paint)
            XANIN_ADD_APP_ENTRY0("reg_dump", reg_dump)
            XANIN_ADD_APP_ENTRY1("load", load)
            XANIN_ADD_APP_ENTRY3("disk_load", disk_load)
            XANIN_ADD_APP_ENTRY3("disk_sectors_write", disk_write_data)
            XANIN_ADD_APP_ENTRY1("execute_addr", execute_addr)
            XANIN_ADD_APP_ENTRY0("pong", pong)
            XANIN_ADD_APP_ENTRY1("loadch", loadch)

            XANIN_ADD_APP_ENTRY0("reboot", reboot)
            XANIN_ADD_APP_ENTRY1("create", xin_create_file_app)
            XANIN_ADD_APP_ENTRY1("mkdir", xin_create_folder_app)
            XANIN_ADD_APP_ENTRY0("shutdown", shutdown)
            XANIN_ADD_APP_ENTRY1("rm", xin_entry_remove_app)
            XANIN_ADD_APP_ENTRY1("rd", xin_folder_remove_app)
            XANIN_ADD_APP_ENTRY1("cd", xin_folder_change_app)

            XANIN_ADD_APP_ENTRY1("pwd", pwd)

            XANIN_ADD_APP_ENTRY2("move", xin_move_entry_app)
            XANIN_ADD_APP_ENTRY2("mv", xin_move_entry_app)
            XANIN_ADD_APP_ENTRY2("copy", xin_copy_app)
            XANIN_ADD_APP_ENTRY2("cp", xin_copy_app)
            XANIN_ADD_APP_ENTRY2("link", xin_link_create_app)
            XANIN_ADD_APP_ENTRY2("lk", xin_link_create_app)
            XANIN_ADD_APP_ENTRY1("xin_info", xin_info)
            XANIN_ADD_APP_ENTRY0("cls", screen_clear)
            XANIN_ADD_APP_ENTRY0("clear", screen_clear)
        else if (bstrcmp(argv[0], "ls"))
        {
            xin_list_files_app(argv);
        }
        // XANIN_ADD_APP_ENTRY1("note", xin_note)
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

            // else if(bstrcmp("real_mode_fswitch", argv[0]))
            // {
            //     XinEntry* real_mode_enter = fopen("/fast_real_mode_enter.bin", "r");
            //     disk_sectors_read(ATA_FIRST_BUS, ATA_MASTER, real_mode_enter->first_sector, 1, (uint16_t*)0x600);

            //     XinEntry* real_mode_return = fopen("/fast_real_mode_return.bin", "r");
            //     disk_sectors_read(ATA_FIRST_BUS, ATA_MASTER, real_mode_return->first_sector, 1, (uint16_t*)0x400);
            //     real_mode_fswitch("0x0", "0x0");
            // }

        else
        {
            if (check_external_apps())
            {
                xprintf("%zunknown command: %s\n", OUTPUT_COLOR_ERROR_SET, argv[0]);
                last_command_exit_status = XANIN_ERROR;
            }

            keyboard_handle = NULL;
            app_exited = true;
            return;
        }

        // if(last_command_exit_status != XANIN_OK)
        // {
        //     xprintf("Command execution status: %z%s", OUTPUT_COLOR_ERROR_SET, app_exit_status_text_get());
        //     screen_background_color_set(red);
        //     msleep(800);
        // }

        keyboard_handle = NULL;
        app_exited = true;

        if (stdio_mode_overriden) // i want to be careful
            __xtf_buffer_clear(__vty_get());

        // terminal_destroy(app_terminal, kernel_terminal);
    }

} // extern "C"