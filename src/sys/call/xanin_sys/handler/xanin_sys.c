
#include <stdint.h>
#include <sys/call/xanin_sys/ids/xanin_syscalls.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/canvas.h>
#include <lib/libc/memory.h>

#include <sys/devices/hda/disk.h>
#include <sys/input/input.h>
#include <fs/xin.h>
#include <sys/terminal/interpreter/interpreter.h>
#include <lib/libc/hal.h>

// SYSCALLS FUNCTIONS
#include <sys/call/xanin_sys/calls/pmmngr/alloc.h>
#include <sys/call/xanin_sys/calls/vga/vga.h>
#include <sys/devices/vga/vga.h>
#include <sys/terminal/frontend/frontend.h>
#include <sys/terminal/backend/backend.h>

stdio_mode_t stdio_current_mode;

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
        : "ecx", "edx", "ebx");

    interrupt_enable();
    // xprintf("eax: %d ", eax);

    switch (eax)
    {

        // XinFs

    case XANIN_FOPEN:
    {
        // ECX = file name, EDX = options_str
        eax = (uint32_t)__xin_fopen((char *)ecx, (char *)edx);
        break;
    }

    case XANIN_FREAD:
    {
        eax = __xin_fread((XinEntry *)ecx, (void *)edx, ebx);
        break;
    }

    case XANIN_FWRITE:
    {
        eax = __xin_fwrite((XinEntry *)ecx, (void *)edx, ebx);
        break;
    }

    case XANIN_FCLOSE:
    {
        __xin_fclose((XinEntry **)ecx);
        break;
    }

    case XANIN_OPEN:
    {
        eax = __xin_open((char *)ecx, edx);
        break;
    }

    case XANIN_READ:
    {
        eax = __xin_read(ecx, (void *)edx, ebx);
        break;
    }

    case XANIN_WRITE:
    {
        eax = __xin_write(ecx, (void *)edx, ebx);
        break;
    }

    case XANIN_CLOSE:
    {
        __xin_close(ecx);
        break;
    }

        // Memory Allocation

    case XANIN_ALLOCATE:
    {
        // ECX = SIZE
        eax = (uint32_t)__sys_malloc(ecx);
        break;
    }

    case XANIN_CALLOCATE:
    {
        // ECX = SIZE
        eax = (uint32_t)__sys_calloc(ecx);
        break;
    }

    case XANIN_FREE:
    {
        // ECX = PTR
        // xprintf("0x%x\n", ecx);
        interrupt_disable();
        mmngr_block_free(USER_HEAP, (void *)ecx);
        interrupt_enable();
        break;
    }

    case XANIN_REALLOCATE:
    {
        // ECX = ptr, EDX = size
        eax = (uint32_t)__sys_realloc((void *)ecx, edx);
        break;
    }

        // Input

    case XANIN_GETCHAR:
    {
        eax = __inputc();
        break;
    }

    case XANIN_GETSCAN:
    {
        eax = __inputg().scan_code;
        break;
    }

    case XANIN_INPUTG:
    {
        // ECX = PTR
        *(xchar *)ecx = __inputg();
        break;
    }

    case XANIN_KEYINFO_GET:
    {
        *(key_info_t *)ecx = __keyinfo_get();
        break;
    }

    case XANIN_IS_NORMAL_KEY_PRESSED:
    {
        eax = KeyInfo.keys_pressed[ecx];
        break;
    }

    case XANIN_IS_SPECIAL_KEY_PRESSED:
    {
        eax = KeyInfo.special_keys_pressed[ecx];
        break;
    }

    case XANIN_INPUT_CHARACTER_MAPPER_SET:
    {
        __input_character_mapper_set((void (*)(uint8_t))ecx);
        break;
    }

    case XANIN_INPUT_CHARACTER_MAPPER_CALL:
    {
        __input_character_mapper_call(ecx);
        break;
    }

    case XANIN_INPUT_ADD_OBJECT_TO_OBSERVE:
    {
        __input_add_object_to_observe(*(KeyboardModuleObservedObject *)&ecx);
        break;
    }

    case XANIN_INPUT_REMOVE_OBJECT_FROM_OBSERVE:
    {
        __input_remove_object_from_observe((const key_info_t *const)ecx);
        break;
    }

    case XANIN_INPUT_HANDLE_OBSERVED_OBJECTS:
    {
        __input_handle_observed_objects((const key_info_t *const)ecx);
        break;
    }

    case XANIN_INPUT_ADD_HANDLER:
    {
        __input_add_handler((const InputHandler *const)ecx);
        break;
    }

    case XANIN_INPUT_REMOVE_HANDLER:
    {
        __input_remove_handler(*((input_handler_t *)&ecx));
        break;
    }

    case XANIN_INPUT_REMOVE_USER_HANDLERS:
    {
        __input_remove_user_handlers();
        break;
    }

    case XANIN_INPUT_CALL_HANDLERS:
    {
        __input_call_handlers(*((key_info_t *)&ecx));
        break;
    }

    case XANIN_DISK_READ:
    {
        // ECX = sector_id, EDX = how_many, EBX = where to load
        disk_read(ATA_FIRST_BUS, ATA_MASTER, ecx, edx, (uint16_t *)ebx);
        eax = (uint32_t)ebx;
        break;
    }

    case XANIN_DISK_WRITE:
    {
        // ECX = sector_id, EDX = how_many, EBX = from where to load
        disk_write(ATA_FIRST_BUS, ATA_MASTER, ecx, edx, (uint16_t *)ebx);
        break;
    }

    case XANIN_ARGC_GET:
    {
        eax = (uint32_t)argc;
        break;
    }

    case XANIN_ARGV_GET:
    {
        eax = (uint32_t)argv;
        break;
    }

    case XANIN_STDIO_MODE_SET:
    {
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

    case XANIN_STDIO_MODE_GET:
    {
        eax = stdio_current_mode;
        break;
    }

    case XANIN_VTY_SET:
    {
        __vty_set((Xtf *)ecx);
        eax = (uint32_t)__vty_get();
        break;
    }

    case XANIN_VTY_GET:
    {
        eax = (uint32_t)__vty_get();
        break;
    }

    case XANIN_XTB_GET:
    {
        eax = (uint32_t)__xtb_get();
        break;
    }

    case XANIN_XTB_INIT:
    {
        __xtb_init(ecx, edx, (uint16_t *)ebx);
        break;
    }

    case XANIN_XTB_FLUSH:
    {
        __xtb_flush((Xtf *)ecx);
        break;
    }

    case XANIN_XTB_FLUSH_ALL:
    {
        __xtb_flush_all((Xtf *)ecx);
        break;
    }

    case XANIN_XTB_SCROLL_UP:
    {
        __xtb_scroll_up((Xtf *)ecx);
        break;
    }

    case XANIN_XTB_SCROLL_DOWN:
    {
        __xtb_scroll_down((Xtf *)ecx);
        break;
    }

    case XANIN_XTF_INIT:
    {
        eax = (uint32_t)xtf_init(ecx);
        break;
    }

    case XANIN_XTF_DESTROY:
    {
        __xtf_destroy((Xtf *)ecx);
        break;
    }

    case XANIN_XTF_BUFFER_NTH_LINE_INDEX_GET:
    {
        eax = (uint32_t)__xtf_buffer_nth_line_index_get((Xtf *)ecx, edx);
        break;
    }

    case XANIN_XTF_BUFFER_NTH_LINE_SIZE_GET:
    {
        eax = (uint32_t)__xtf_buffer_nth_line_size_get((Xtf *)ecx, edx);
        break;
    }

    case XANIN_XTF_LINE_NUMBER_FROM_POSITION_GET:
    {
        eax = (uint32_t)__xtf_line_number_from_position_get((Xtf *)ecx, edx);
        break;
    }

    case XANIN_XTF_CELL_PUT:
    {
        __xtf_cell_put((Xtf *)ecx, (char)edx, (color_t)ebx);
        break;
    }

    case XANIN_XTF_CHARACTER_PUT:
    {
        __xtf_character_put((Xtf *)ecx, (char)edx);
        break;
    }

    case XANIN_XTF_REMOVE_LAST_CELL:
    {
        __xtf_remove_last_cell((Xtf *)ecx);
        break;
    }

        // ADD LATER
        // case XANIN_XTF_VIRUTAL_CURSOR_ADD:
        // {
        //     xtf_virtual_cursor_add((Xtf*)ecx, (color_t)edx);
        //     break;
        // }

    case XANIN_XTF_BUFFER_CLEAR:
    {
        __xtf_buffer_clear((Xtf *)ecx);
        break;
    }

    case XANIN_XTF_SCROLLING_ON:
    {
        __xtf_scrolling_on((Xtf *)ecx);
        break;
    }

    case XANIN_XTF_SCROLLING_OFF:
    {
        __xtf_scrolling_off((Xtf *)ecx);
        break;
    }

    case XANIN_XTF_CURSOR_ON:
    {
        __xtf_cursor_on((Xtf *)ecx, (color_t)edx);
        break;
    }

    case XANIN_XTF_CURSOR_OFF:
    {
        __xtf_cursor_off((Xtf *)ecx);
        break;
    }

    case XANIN_XTF_CURSOR_INC:
    {
        __xtf_cursor_inc((Xtf *)ecx);
        break;
    }

    case XANIN_XTF_CURSOR_DEC:
    {
        __xtf_cursor_dec((Xtf *)ecx);
        break;
    }

    case XANIN_VGA_BUFFER_SEGMENT_GET:
    {
        eax = (uint32_t)vga_get_buffer_segment();
        break;
    }

    case XANIN_VGA_TEXT_MODE_WIDTH_GET:
    {
        eax = vga_text_mode_width;
        break;
    }

    case XANIN_VGA_TEXT_MODE_HEIGHT_GET:
    {
        eax = vga_text_mode_height;
        break;
    }
    }

    return eax;
}