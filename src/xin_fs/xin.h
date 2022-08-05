
#pragma once

#include <stdint.h>
#include <xin_fs/xin_entry.h>

typedef int __STATUS;

enum XANIN_RETURN_STATUS
{

    XANIN_OK = 0x0,
    XANIN_ERROR = 0x3,
    XIN_FILE_EXISTS = 0x4,
    XIN_BAD_FOLDER_NAME = 0x5,
    XIN_ENTRY_NOT_FOUND = 0x6

};

char xin_current_path[38] = {'\0'};
char xin_current_directory[38] = {'\0'};


char* xin_set_current_directory(char* directory);
char* xin_get_current_directory(void);
char* xin_get_current_path(char* file_name);

__STATUS sys_xin_remove_entry(char* entry_name);
char* xin_get_current_path(char* file_name);

xin_entry* xin_init_fs(void);
xin_entry* xin_find_free_entry(void);

int create_file(char* entry_name);
int xin_create_directory(char* entry_name);
xin_entry* xin_change_directory(char* new_directory);



int xin_entry_resize(uint16_t last_sector);
xin_entry* fopen(char* file_path, const char* mode);
size_t write(xin_entry *entry, void *buf, size_t count);
size_t read(xin_entry *entry, void *buf, size_t count);
xin_entry* create(char* file_name);
void fseek(xin_entry *file, uint32_t new_position);