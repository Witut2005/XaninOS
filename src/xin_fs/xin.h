
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <xin_fs/xin_entry.h>

#define XIN_SYSTEM_FOLDER '/'
#define XIN_BASE_FILE_ADDRESS_TO_LOAD 0x100000
#define XIN_FILE_BEGIN 0
#define XIN_ENTRY_SIZE 64

typedef int __STATUS;

struct XinFileDescriptor
{
    uint8_t is_used;
    xin_entry* entry;
};

#ifndef __cplusplus
typedef struct XinFileDescriptor XinFileDescriptor;
#endif


// extern uint8_t xin_base_state[100];
extern char xin_current_path[38];
extern char xin_current_directory[38];

extern uint8_t enter_real_mode_buffer[512];
extern uint8_t shutdown_program_buffer[512];

extern uint8_t* bootloader_program_buffer;
extern uint8_t* kernel_load_backup;

extern XinFileDescriptor* FileDescriptorTable;


#ifdef __cplusplus
extern "C" {
#endif

char* xin_set_current_directory(char* directory);
char* xin_get_current_directory(void);
char* xin_get_current_path(char* file_name);

__STATUS sys_xin_remove_entry(char* entry_name);
char* xin_get_current_path(char* file_name);

void xin_init_fs(void);
xin_entry* xin_find_free_entry(void);

int xin_create_file(char* entry_name);
int xin_folder_create(char* entry_name);
xin_entry* xin_folder_change(char* new_directory);



int xin_entry_resize(uint16_t last_sector);
xin_entry* fopen(char* file_path, char* mode);
int open(char* file_path, uint32_t options);
size_t fwrite(xin_entry *entry, void *buf, size_t count);
size_t fread(xin_entry *entry, void *buf, size_t count);
size_t write(int fd, void *buf, size_t count);
size_t read(int fd, void *buf, size_t count);
xin_entry* create(char* file_name);
void fseek(xin_entry *file, uint32_t new_position);
void lseek(int fd, uint32_t new_position);
xin_entry* xin_get_file_pf(char* entry_path); // pf = parent folder
xin_entry *xin_find_entry(char *entry_name);
__STATUS remove_directory(char* folder_name);
void create_file_kernel(char* entry_name);
void fclose(xin_entry** file);
void close(int fd);
char* getline(xin_entry* file, int line_id);
char* getline_from_ptr(char* data, int line_id);
__STATUS __sys_xin_file_create(char* entry_name);
__STATUS __sys_xin_folder_create(char* entry_name);
__STATUS __sys_xin_entry_move(char* entry_name, char* new_name);
__STATUS __sys_xin_folder_remove(char* folder_name);
__STATUS __sys_xin_entry_remove(char* entry_name);
__STATUS __sys_xin_folder_change(const char* new_folder_name);
__STATUS __sys_xin_copy(char* file_name, char* new_file_name);
__STATUS __sys_xin_link_create(char* file_name, char* link_name);
__STATUS __sys_xin_list_files(char** argv);
__STATUS __sys_xin_link_remove(char* linkname);

#ifdef __cplusplus
}
#endif
