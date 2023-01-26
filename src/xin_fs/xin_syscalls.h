

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

__STATUS __sys_xin_file_create(char* entry_name);
__STATUS __sys_xin_folder_create(char* entry_name);
__STATUS __sys_xin_entry_move(char* entry_name, char* new_name);
__STATUS __sys_xin_folder_remove(char* folder_name);
__STATUS __sys_xin_entry_remove(char* entry_name);
__STATUS __sys_xin_folder_change(const char* new_folder_name);
__STATUS __sys_xin_copy(char* file_name, char* new_file_name);
__STATUS __sys_xin_link_remove(char* linkname);
__STATUS __sys_xin_link_create(char* file_name, char* link_name);
__STATUS __sys_xin_list_files(char** argv);

#ifdef __cplusplus
}
#endif