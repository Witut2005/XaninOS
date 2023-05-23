
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <fs/xin_entry.h>
#include <stdbool.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/file.h>

#define XIN_SYSTEM_FOLDER '/'
#define XIN_BASE_FILE_ADDRESS_TO_LOAD 0x100000
#define XIN_FILE_BEGIN 0
#define XIN_ENTRY_SIZE 64

enum XIN_RETURN_STATUS
{

    XIN_FILE_EXISTS = 0x4,
    XIN_BAD_FOLDER_NAME = 0x5,
    XIN_ENTRY_NOT_FOUND = 0x6

};

struct XinFileDescriptor
{
    uint8_t is_used;
    XinEntry* entry;
};

struct XinChildrenEntries
{
    XinEntry** children;
    uint32_t how_many;
};

struct XinEntriesPack
{
    XinEntry** entries;
    uint32_t length;
};

#ifndef __cplusplus
typedef struct XinFileDescriptor XinFileDescriptor;
typedef struct XinChildrenEntries XinChildrenEntries;
typedef struct XinEntriesPack XinEntriesPack;
#endif


// extern uint8_t xin_base_state[100];
extern char xin_current_path[38];
extern char xin_current_directory[38];

extern XinFileDescriptor* FileDescriptorTable;

#ifdef __cplusplus
extern "C" {
namespace xin{
#endif

char* xin_set_current_directory(char* directory);
void xin_get_current_directory(char* buf);
char* xin_get_current_path(char* file_name);

__STATUS __sys_xin_entry_remove(char *entry_name);
char* xin_get_current_path(char* file_name);

void xin_load_tables(void);
void xin_init_fs(void);
XinEntry* xin_find_free_entry(void);

int xin_file_create(char* entry_name);
int xin_folder_create(char* entry_name);
XinEntry* xin_folder_change(char* new_directory);

XinEntry* __xin_fopen(char* file_path, char* mode);
size_t __xin_fwrite(XinEntry *entry, void *buf, size_t count);
size_t __xin_fread(XinEntry *entry, void *buf, size_t count);
void __xin_fclose(XinEntry** file);

int __xin_open(char* file_path, uint32_t options);
size_t __xin_write(int fd, void *buf, size_t count);
size_t __xin_read(int fd, void *buf, size_t count);
void __xin_close(int fd);

void fseek(XinEntry *file, uint32_t new_position);
void lseek(int fd, uint32_t new_position);
XinEntry* xin_get_file_pf(char* path); // pf = parent folder
XinEntry* xin_find_entry(char *entry_name);
__STATUS remove_directory(char* folder_name);
void create_file_kernel(char* entry_name);
void fclose_with_given_size(XinEntry** file, uint32_t new_size);
char* getline(XinEntry* file, int line_id);
char* getline_from_ptr(char* data, int line_id);
XinChildrenEntries* xin_get_children_entries(char* folder, bool show_hidden);
XinChildrenEntries* xin_get_children_entries_type(char* folder, uint8_t type);
char* xin_get_entry_name(char* path);
const uint32_t ftell(XinEntry* file);
const uint32_t lteel(int fd);
uint8_t *xin_find_free_pointer(void);
uint8_t *xin_find_free_pointer_with_given_size(uint32_t size);
int xin_get_file_size_in_sectors(XinEntry* File);
void xin_free_temporary_data(XinEntry* File);
XinEntriesPack* xin_get_hard_links(const XinEntry* const File);
bool xin_add_files_to_xfo(XinEntry* File);
void xin_close_all_files(void);

#ifdef __cplusplus
}
}
#endif
