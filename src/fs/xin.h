
#pragma once

#include "./xin_structures.h"
#include <lib/libc/stdlibx.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/flow/exit_codes.h>
#include <sys/macros.h>

#define VAL(x) (*x)

#ifdef __cplusplus
// #include <lib/libcpp/string.h>

// class XinPath
// {
// public:
//     XinPath(const std::string_view& path);

// };

extern "C" {
#endif
    /* ------------------------------------------------ */

    void __xin_detect_file_system(void);
    XinFileSystemData __xin_fs_data_get(void);

    uint8_t* __xin_fs_tables_get(void);

    uint8_t* __xin_fs_ptrs_get(void);
    xin_ptr_t* __xin_fs_ptrs_end_get(void);
    uint32_t __xin_fs_ptrs_size_get(void);

    uint8_t* __xin_fs_entries_get(void);
    XinEntry* __xin_fs_entries_end_get(void);
    uint32_t __xin_fs_entries_size_get(void);

    void __xin_fs_tables_set(uint8_t* tables);
    void __xin_fs_load_tables_from_disk(void); // oj calkiem risky
    void __xin_tables_update(void);
    void __xin_init(void);

    /* ------------------------------------------------ */

    [[nodiscard]] bool __xin_entry_alignment_check(XinEntry const* Entry);
    [[nodiscard]] bool __xin_entry_address_check(XinEntry const* Entry);
    [[nodiscard]] bool __xin_entry_validation_check(XinEntry const* Entry);
    [[nodiscard]] bool __xin_is_relative_path_used(char const* path);
    char* __xin_absolute_path_get(char const* rpath, char* buf, XIN_FS_ENTRY_TYPES type);

    [[nodiscard]] char* __xin_entry_name_extern(char* path, char* buf);

    [[nodiscard]] int __xin_entry_descriptor_get(XinEntry const* Entry);
    void __xin_entry_modification_fields_update(XinEntry* Entry);
    [[nodiscard]] bool __xin_is_entry_rwable_check(XinEntry const* Entry); // read and write

    /* ------------------------------------------------ */

    [[nodiscard]] XinEntry* __xin_find_entry(char const* entry_name);
    [[nodiscard]] XinEntry* __xin_find_free_entry(void);
    [[nodiscard]] uint8_t* __xin_find_free_pointer(void);
    [[nodiscard]] uint8_t* __xin_find_free_pointer_with_given_size(uint32_t size);
    char* __xin_path_pf_extern(char* absname, char* buf);      // pf = parent folder
    [[nodiscard]] XinEntry* __xin_entry_pf_extern(char* name); // pf = parent folder
    [[nodiscard]] XinEntry* __xin_entry_pf_get(const char* path);    // pf = parent folder

    [[nodiscard]] XinChildrenEntries* xin_children_entries_get(char* folder, bool show_hidden);
    [[nodiscard]] XinChildrenEntries* xin_children_entries_type_get(char* folder, uint8_t type);
    [[nodiscard]] XinEntriesPack* __xin_hard_links_get(XinEntry const* const File);

    char* __xin_current_directory_get(char* buf);

    /* ------------------------------------------------ */

    bool __xin_entry_sectors_unlock(XinEntry* Entry);
    bool __xin_entry_data_remove(XinEntry* Entry);

    XIN_FS_RETURN_STATUSES __xin_entry_create(XinEntryCreateArgs* Args, XIN_FS_ENTRY_TYPES type);
    XIN_FS_RETURN_STATUSES __xin_file_create(char* filename);
    XIN_FS_RETURN_STATUSES __xin_folder_create(char* foldername);
    XIN_FS_RETURN_STATUSES __xin_link_create(char* filename, char* link_name);

    XIN_FS_RETURN_STATUSES __xin_folder_change(char const* foldername);

    XIN_FS_RETURN_STATUSES __xin_file_remove(char* filename);
    XIN_FS_RETURN_STATUSES __xin_folder_remove(char* foldername);
    XIN_FS_RETURN_STATUSES __xin_link_remove(char const* linkname);

    XIN_FS_RETURN_STATUSES __xin_entry_resize(XinEntry* entry, uint32_t size); // XIN_FS_RETURN_STATUSES
    XIN_FS_RETURN_STATUSES __xin_entry_move(char* entry_name, char* new_name);
    XIN_FS_RETURN_STATUSES __xin_copy(char* file_name, char* new_file_name);

    /* ------------------------------------------------ */

    bool __xin_file_to_xfo_add(XinEntry* File);
    [[nodiscard]] XinEntry* __xin_fopen(char* file_path, char* mode);
    size_t __xin_fread(XinEntry* entry, void* buf, size_t count);
    size_t __xin_fwrite(XinEntry* entry, void* buf, size_t count);

    [[nodiscard]] int __xin_open(char* file_path, uint32_t options);
    size_t __xin_read(int fd, void* buf, size_t count);
    size_t __xin_write(int fd, void* buf, size_t count);

    void __xin_fseek(XinEntry* file, uint32_t new_position);
    void __xin_lseek(int fd, uint32_t new_position);

    [[nodiscard]] const uint32_t __xin_ftell(XinEntry* file);
    [[nodiscard]] const uint32_t __xin_ltell(int fd);

    void __xin_free_temporary_data(XinEntry* File);
    bool __xin_fclose_with_given_size(XinEntry** file, uint32_t new_size);
    void __xin_fclose(XinEntry** file);
    void __xin_close(int fd);
    void __xin_all_files_close(void);

    /* ------------------------------------------------ */

    char* getline_from_ptr(char* data, int line_id);

    /* ------------------------------------------------ */

#ifdef __cplusplus
}
#endif
