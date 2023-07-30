
%define XANIN_VGA_BUFFER_SEGMENT_GET 20000
%define XANIN_VGA_TEXT_MODE_WIDTH_GET 20001
%define XANIN_VGA_TEXT_MODE_HEIGHT_GET 20002


global __vga_buffer_segment_get, __vga_text_mode_height_get, __vga_text_mode_width_get

__vga_buffer_segment_get:
mov eax, XANIN_VGA_BUFFER_SEGMENT_GET
int 0x81
ret

__vga_text_mode_width_get:
mov eax, XANIN_VGA_TEXT_MODE_WIDTH_GET
int 0x81
ret

__vga_text_mode_height_get:
mov eax, XANIN_VGA_TEXT_MODE_HEIGHT_GET
int 0x81
ret