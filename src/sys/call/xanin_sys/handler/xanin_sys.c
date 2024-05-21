
#define KERNEL_MODULE

#include <lib/libc/canvas.h>
#include <lib/libc/memory.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/string.h>
#include <stdint.h>
#include <sys/call/xanin_sys/ids/xanin_syscalls.h>

#include <fs/xin.h>
#include <lib/libc/hal.h>
#include <sys/devices/hda/disk.h>
#include <sys/input/input.h>
#include <sys/terminal/interpreter/interpreter.h>

#include <sys/devices/com/com.h>
#include <sys/pmmngr/alloc.h>

// SYSCALLS FUNCTIONS
#include <sys/call/xanin_sys/calls/vga/vga.h>
#include <sys/devices/vga/vga.h>
#include <sys/terminal/backend/backend.h>
#include <sys/terminal/frontend/frontend.h>

#undef KERNEL_MODULE

stdio_mode_t stdio_current_mode;
uint32_t stdeio_counter = 0;

uint32_t xanin_sys_handle(void)
{
    uint32_t eax, ecx, edx, ebx; //, esi, edi;

    asm(
        "mov %0, eax;"
        "mov %1, ecx;"
        "mov %2, edx;"
        "mov %3, ebx;"
        : "=g"(eax), "=g"(ecx), "=g"(edx), "=g"(ebx)
        :
        : "eax", "ecx", "edx", "ebx");

    interrupt_enable();

    switch (eax)
    {

        // XinFs
    case XANIN_FOPEN: {
        // ECX = file name, EDX = options_str
        eax = (uint32_t)__xin_fopen((char*)ecx, (char*)edx);
        break;
    }

    case XANIN_FREAD: {
        eax = __xin_fread((XinEntry*)ecx, (void*)edx, ebx);
        break;
    }

    case XANIN_FWRITE: {
        eax = __xin_fwrite((XinEntry*)ecx, (void*)edx, ebx);
        break;
    }

    case XANIN_FCLOSE: {
        __xin_fclose((XinEntry**)ecx);
        break;
    }

    case XANIN_OPEN: {
        eax = __xin_open((char*)ecx, edx);
        break;
    }

    case XANIN_READ: {
        eax = __xin_read(ecx, (void*)edx, ebx);
        break;
    }

    case XANIN_WRITE: {
        eax = __xin_write(ecx, (void*)edx, ebx);
        break;
    }

    case XANIN_CLOSE: {
        __xin_close(ecx);
        break;
    }

    case XANIN_XIN_ENTRY_CREATE: {
        eax = __xin_entry_create((XinEntryCreateArgs*)ecx, edx);
        break;
    }

    case XANIN_FSEEK: {
        __xin_fseek((XinEntry*)ecx, edx);
        break;
    }

    case XANIN_FTELL: {
        eax = __xin_ftell((XinEntry*)ecx);
        break;
    }

    case XANIN_SEEK: {
        __xin_fseek((XinEntry*)ecx, edx);
        break;
    }

    case XANIN_TELL: {
        eax = __xin_ltell(ecx);
        break;
    }

    // Memory Allocation
    case XANIN_ALLOCATE: {
        // ECX = SIZE
        eax = (uint32_t)umalloc((uint32_t)ecx);
        break;
    }

    case XANIN_CALLOCATE: {
        // ECX = SIZE
        eax = (uint32_t)ucalloc((uint32_t)ecx);
        break;
    }

    case XANIN_FREE: {
        // ECX = PTR
        ufree((void*)ecx);
        break;
    }

    case XANIN_REALLOCATE: {
        // ECX = ptr, EDX = size
        eax = (uint32_t)urealloc((void*)ecx, edx);
        break;
    }

        // Input

    case XANIN_GETCHAR: {
        eax = __inputc();
        break;
    }

    case XANIN_GETSCAN: {
        eax = __inputg().scan_code;
        break;
    }

    case XANIN_INPUTG: {
        // ECX = PTR
        *(xchar*)ecx = __inputg();
        break;
    }

    case XANIN_KEYINFO_GET: {
        *(KeyInfo*)ecx = input_key_info_get();
        break;
    }

    case XANIN_IS_NORMAL_KEY_PRESSED: {
        eax = input_key_info_get().keys_pressed[ecx];
        break;
    }

    case XANIN_IS_SPECIAL_KEY_PRESSED: {
        eax = input_key_info_get().special_keys_pressed[ecx];
        break;
    }

    case XANIN_INPUT_CHARACTER_MAPPER_SET: {
        input_mapper_set((void (*)(uint8_t))ecx);
        break;
    }

    case XANIN_INPUT_CHARACTER_MAPPER_CALL: {
        input_mapper_call(ecx);
        break;
    }

    case XANIN_INPUT_ADD_HANDLER: {
        input_handler_add(*(InputHandler*)ecx, INPUT_USER);
        break;
    }

    case XANIN_INPUT_REMOVE_HANDLER: {
#warning TODO use id here
        // input_handler_remove(*((input_handler_t*)&ecx));
        break;
    }

    case XANIN_INPUT_REMOVE_USER_HANDLERS: {
        input_user_handlers_remove();
        break;
    }

    case XANIN_INPUT_CALL_HANDLERS: {
        input_handlers_call();
        break;
    }

    case XANIN_INPUT_PRTSC_HANDLER_SET: {
        __input_prtsc_handler_set((input_scan_code_mapper_handler_t)ecx);
        break;
    }

    case XANIN_DISK_READ: {
        // ECX = sector_id, EDX = how_many, EBX = where to load
        disk_sectors_read(ATA_FIRST_BUS, ATA_MASTER, ecx, edx, (uint16_t*)ebx);
        eax = (uint32_t)ebx;
        break;
    }

    case XANIN_DISK_WRITE: {
        // ECX = sector_id, EDX = how_many, EBX = from where to load
        disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, ecx, edx, (uint16_t*)ebx);
        break;
    }

    case XANIN_ARGC_GET: {
        eax = (uint32_t)argc;
        break;
    }

    case XANIN_ARGV_GET: {
        eax = (uint32_t)argv;
        break;
    }

    case XANIN_STDIO_MODE_SET: {
        switch (ecx)
        {
        case STDIO_MODE_CANVAS:
            stdio_current_mode = ecx;
            break;
        default:
            stdio_current_mode = STDIO_MODE_TERMINAL;
            memset(__vty_get()->rows_changed, XTF_ROW_CHANGED, __vty_get()->current_height * SIZE_OF_POINTED_TYPE(__vty_get()->rows_changed));
            __xtb_flush(__vty_get());
            break;
        }
        break;
    }

    case XANIN_STDIO_MODE_GET: {
        eax = stdio_current_mode;
        break;
    }

    case XANIN_VTY_SET: {
        __vty_set((Xtf*)ecx);
        eax = (uint32_t)__vty_get();
        break;
    }

    case XANIN_VTY_GET: {
        eax = (uint32_t)__vty_get();
        break;
    }

    case XANIN_XTB_GET: {
        eax = (uint32_t)__xtb_get();
        break;
    }

    case XANIN_XTB_INIT: {
        __xtb_init(ecx, edx, (uint16_t*)ebx);
        break;
    }

    case XANIN_XTB_FLUSH: {
        __xtb_flush((Xtf*)ecx);
        break;
    }

    case XANIN_XTB_FLUSH_ALL: {
        __xtb_flush_all((Xtf*)ecx);
        break;
    }

    case XANIN_XTB_SCROLL_UP: {
        __xtb_scroll_up((Xtf*)ecx);
        break;
    }

    case XANIN_XTB_SCROLL_DOWN: {
        __xtb_scroll_down((Xtf*)ecx);
        break;
    }

    case XANIN_XTF_INIT: {
        eax = (uint32_t)xtf_init(ecx);
        break;
    }

    case XANIN_XTF_DESTROY: {
        __xtf_destroy((Xtf*)ecx);
        break;
    }

    case XANIN_XTF_BUFFER_NTH_LINE_INDEX_GET: {
        eax = (uint32_t)__xtf_buffer_nth_line_index_get((Xtf*)ecx, edx);
        break;
    }

    case XANIN_XTF_BUFFER_NTH_LINE_SIZE_GET: {
        eax = (uint32_t)__xtf_buffer_nth_line_size_get((Xtf*)ecx, edx);
        break;
    }

    case XANIN_XTF_LINE_NUMBER_FROM_POSITION_GET: {
        eax = (uint32_t)__xtf_line_number_from_position_get((Xtf*)ecx, edx);
        break;
    }

    case XANIN_XTF_CELL_PUT: {
        __xtf_cell_put((Xtf*)ecx, (char)edx, (color_t)ebx);
        break;
    }

    case XANIN_XTF_CHARACTER_PUT: {
        __xtf_character_put((Xtf*)ecx, (char)edx);
        break;
    }

    case XANIN_XTF_REMOVE_LAST_CELL: {
        __xtf_remove_last_cell((Xtf*)ecx);
        break;
    }

        // ADD LATER
        // case XANIN_XTF_VIRUTAL_CURSOR_ADD:
        // {
        //     xtf_virtual_cursor_add((Xtf*)ecx, (color_t)edx);
        //     break;
        // }

    case XANIN_XTF_BUFFER_CLEAR: {
        __xtf_buffer_clear((Xtf*)ecx);
        break;
    }

    case XANIN_XTF_SCROLLING_ON: {
        __xtf_scrolling_on((Xtf*)ecx);
        break;
    }

    case XANIN_XTF_SCROLLING_OFF: {
        __xtf_scrolling_off((Xtf*)ecx);
        break;
    }

    case XANIN_XTF_CURSOR_ON: {
        __xtf_cursor_on((Xtf*)ecx, (color_t)edx);
        break;
    }

    case XANIN_XTF_CURSOR_OFF: {
        __xtf_cursor_off((Xtf*)ecx);
        break;
    }

    case XANIN_XTF_CURSOR_INC: {
        __xtf_cursor_inc((Xtf*)ecx);
        break;
    }

    case XANIN_XTF_CURSOR_DEC: {
        __xtf_cursor_dec((Xtf*)ecx);
        break;
    }

    case XANIN_VGA_BUFFER_SEGMENT_GET: {
        eax = (uint32_t)vga_get_buffer_segment();
        break;
    }

    case XANIN_VGA_TEXT_MODE_WIDTH_GET: {
        eax = vga_text_mode_width;
        break;
    }

    case XANIN_VGA_TEXT_MODE_HEIGHT_GET: {
        eax = vga_text_mode_height;
        break;
    }

#warning "Create new folder"

    case XANIN_SHELL_STDEIO_CREATE: {
        char buf[32] = { 0 };
        char path[32] = "/proc/";
        __xin_folder_create(path);

        int_to_string(stdeio_counter, buf, HEXADECIMAL);
        memcpy(&buf[strlen(buf)], "/", 2);

        strcat(STRCAT_DEST_FIRST, path, buf);
        __xin_folder_create(path);

        strcat(STRCAT_DEST_FIRST, path, "stderr");
        __xin_file_create(path);

        dbg_info(DEBUG_LABEL_SYSCALL, path);

        stdeio_counter++;
        break;
    }

    default: {
        dbg_error(DEBUG_LABEL_SYSCALL, "Unknown syscall");
    }
    }

    return eax;
}
