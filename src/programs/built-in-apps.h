

#pragma once

#include <sys/flow/exit_codes.h>

#ifdef __cplusplus
extern "C" {
#endif

STATUS paging_test(void);
STATUS xin_check(char* path);
STATUS stdio_apply(void);
STATUS screen_saver(char*);
STATUS bmp_info(char*);
STATUS netplan_apply(void);

STATUS xgl_mode_set(void);
STATUS buffers_view(void);
STATUS netapi_check(char*);
STATUS usb_controller_info(void);
STATUS memory_test(void);
STATUS grapher(void);
STATUS ptetris(void);
STATUS explorer(char*);
STATUS interrupt_test(char*);
STATUS idt_examine(void);
STATUS start_screen(void);
STATUS start_screen(void);
STATUS load_file(char*, char*);
STATUS help(char*);
STATUS print_to_syslog(char*);
STATUS cat(char*);
STATUS zsk(char*);
STATUS epilepsy(void);
STATUS elfdump(char*);
STATUS hexeditor(char*, char*);
STATUS run16(char*);
STATUS run(char*);
STATUS cpu_info(char*);
STATUS calc(char*);
STATUS xin_paint(char*);
STATUS reg_dump(void);
STATUS load(char*);
STATUS disk_load(char*, char*, char*);
STATUS disk_write_data(char*, char*, char*);
STATUS execute_addr(char*);
STATUS pong(void);
STATUS loadch(char*);
STATUS cpp_test(void);
STATUS __asm_reboot(void);
STATUS xin_create_folder_app(char*);
STATUS shutdown(void);
STATUS xin_entry_remove_app(char*);
STATUS xin_folder_remove_app(char*);
STATUS xin_folder_change_app(char*);
STATUS pwd(char*);
STATUS xin_move_entry_app(char*, char*);
STATUS xin_move_entry_app(char*, char*);
STATUS xin_copy_app(char*, char*);
STATUS xin_link_create_app(char*, char*);
STATUS xin_link_create_app(char*, char*);
STATUS xin_info(char*);
STATUS screen_clear(void);
STATUS xin_list_files_app(char*[]);

STATUS xin_note(char*);
STATUS logo_color_change(char*, char*);
STATUS nic_info(char*);
STATUS gyn_interpreter(char*);
STATUS screenshot(void);
STATUS xin_xpaint(char*);
STATUS xagame_test(void);
STATUS tetris(void);
STATUS xgl_test(void);
STATUS arp_table_print(char*, char*);
STATUS ping(char*);
STATUS ip_test(char*);
STATUS udp_test(char*);
STATUS edit(char*);
STATUS arp_check(char*);
STATUS dhcp_test(void);
STATUS nic_rename(char*, char*);
STATUS nic_print(void);
STATUS xin_create_file_app(char*);
STATUS disk_func(void);
STATUS c_test(void);
STATUS sprintf_test(void);
STATUS elf_loader_test(char*);

STATUS __cpp_string_test(void);
STATUS __cpp_lexer_test(void);
STATUS __cpp_xin_test(void);
STATUS __cpp_array_test(void);
STATUS __cpp_algo_test(void);
STATUS __cpp_vector_test(void);
STATUS __cpp_global_constructors_test(void);
STATUS __cpp_array_test(void);

STATUS __c_alloc_test(void);

#ifdef __cplusplus
}
#endif
