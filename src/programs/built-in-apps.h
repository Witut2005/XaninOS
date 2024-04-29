

#pragma once

#include <sys/flow/exit_codes.h>

#ifdef __cplusplus
extern "C" {
#endif

    __STATUS paging_test(void);
    __STATUS xin_check(char* path);
    __STATUS stdio_apply(void);
    __STATUS screen_saver(char*);
    __STATUS bmp_info(char*);
    __STATUS netplan_apply(void);

    __STATUS xgl_mode_set(void);
    __STATUS buffers_view(void);
    __STATUS netapi_check(char*);
    __STATUS usb_controller_info(void);
    __STATUS memory_test(void);
    __STATUS grapher(void);
    __STATUS ptetris(void);
    __STATUS explorer(char*);
    __STATUS interrupt_test(char*);
    __STATUS idt_examine(void);
    __STATUS start_screen(void);
    __STATUS start_screen(void);
    __STATUS load_file(char*, char*);
    __STATUS help(char*);
    __STATUS print_to_syslog(char*);
    __STATUS timer_test(void);
    __STATUS cat(char*);
    __STATUS zsk(char*);
    __STATUS epilepsy(void);
    __STATUS elfdump(char*);
    __STATUS hexeditor(char*, char*);
    __STATUS run16(char*);
    __STATUS run(char*);
    __STATUS cpu_info(char*);
    __STATUS calc(char*);
    __STATUS xin_paint(char*);
    __STATUS reg_dump(void);
    __STATUS load(char*);
    __STATUS disk_load(char*, char*, char*);
    __STATUS disk_write_data(char*, char*, char*);
    __STATUS execute_addr(char*);
    __STATUS pong(void);
    __STATUS loadch(char*);
    __STATUS cpp_test(void);
    __STATUS __asm_reboot(void);
    __STATUS xin_create_folder_app(char*);
    __STATUS shutdown(void);
    __STATUS xin_entry_remove_app(char*);
    __STATUS xin_folder_remove_app(char*);
    __STATUS xin_folder_change_app(char*);
    __STATUS pwd(char*);
    __STATUS xin_move_entry_app(char*, char*);
    __STATUS xin_move_entry_app(char*, char*);
    __STATUS xin_copy_app(char*, char*);
    __STATUS xin_link_create_app(char*, char*);
    __STATUS xin_link_create_app(char*, char*);
    __STATUS xin_info(char*);
    __STATUS screen_clear(void);
    __STATUS xin_list_files_app(char* []);

    __STATUS xin_note(char*);
    __STATUS logo_color_change(char*, char*);
    __STATUS nic_info(char*);
    __STATUS gyn_interpreter(char*);
    __STATUS screenshot(void);
    __STATUS xin_xpaint(char*);
    __STATUS xagame_test(void);
    __STATUS tetris(void);
    __STATUS xgl_test(void);
    __STATUS arp_table_print(char*, char*);
    __STATUS ping(char*);
    __STATUS ip_test(char*);
    __STATUS udp_test(char*);
    __STATUS edit(char*);
    __STATUS arp_check(char*);
    __STATUS dhcp_test(void);
    __STATUS nic_rename(char*, char*);
    __STATUS nic_print(void);
    __STATUS xin_create_file_app(char*);
    __STATUS disk_func(void);
    __STATUS c_test(void);
    __STATUS sprintf_test(void);
    __STATUS elf_loader_test(char*);

    __STATUS __cpp_string_test(void);
    __STATUS __cpp_lexer_test(void);
    __STATUS __cpp_xin_test(void);
    __STATUS __cpp_array_test(void);
    __STATUS __cpp_algo_test(void);
    __STATUS __cpp_vector_test(void);
    __STATUS __c_alloc_test(void);

#ifdef __cplusplus
}
#endif
