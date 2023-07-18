
%define XANIN_VGA_BUFFER_SEGMENT_GET 20000


global __vga_buffer_segment_get

__vga_buffer_segment_get:
mov eax, XANIN_VGA_BUFFER_SEGMENT_GET
int 0x81
ret