
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <xin_fs/xin_entry.h>

#define XIN_SYSTEM_FOLDER '/'
#define XIN_BASE_FILE_ADDRESS_TO_LOAD 0x100000
#define XIN_FILE_BEGIN 0

typedef int __STATUS;



// extern uint8_t xin_base_state[100];
extern char xin_current_path[38];
extern char xin_current_directory[38];

extern uint8_t enter_real_mode_buffer[512];
extern uint8_t shutdown_program_buffer[512];

extern uint8_t* bootloader_program_buffer;
extern uint8_t* kernel_load_backup;


#ifdef __cplusplus
extern "C" {
#endif

char* xin_set_current_directory(char* directory);
char* xin_get_current_directory(void);
char* xin_get_current_path(char* file_name);

__STATUS sys_xin_remove_entry(char* entry_name);
char* xin_get_current_path(char* file_name);

xin_entry* xin_init_fs(void);
xin_entry* xin_find_free_entry(void);

int xin_create_file(char* entry_name);
int xin_folder_create(char* entry_name);
xin_entry* xin_folder_change(char* new_directory);



int xin_entry_resize(uint16_t last_sector);
xin_entry* fopen(char* file_path, const char* mode);
int open(char* file_path, uint32_t options);
size_t write(xin_entry *entry, void *buf, size_t count);
size_t read(xin_entry *entry, void *buf, size_t count);
xin_entry* create(char* file_name);
void fseek(xin_entry *file, uint32_t new_position);
xin_entry* xin_get_file_pf(char* entry_path); // pf = parent folder
xin_entry *xin_find_entry(char *entry_name);
__STATUS remove_directory(char* folder_name);
void create_file_kernel(char* entry_name);
void fclose(xin_entry** file);
char* getline(xin_entry* file, int line_id);
char* getline_from_ptr(char* data, int line_id);

#ifdef __cplusplus
}
#endif