
%include "./sys/call/xanin_sys/calls/terminal/ids.inc"

%macro XANIN_INVOKE_SYSTEM_CALL 0
int 0x81
%endmacro

global vty_set, vty_get, xtb_get, xtb_init
global __sys_xtf_init, __sys_xtf_destroy
global __sys_xtf_buffer_nth_line_index_get, __sys_xtf_buffer_nth_line_size_get, __sys_xtf_get_line_number_from_position
global __sys_xtf_cell_put, __sys_xtf_remove_last_cell, __sys_xtf_virutal_cursor_add, __sys_xtf_buffer_clear
global __sys_xtf_scrolling_on, __sys_xtf_scrolling_off
global __sys_xtf_cursor_on, __sys_xtf_cursor_off, __sys_xtf_cursor_inc, __sys_xtf_cursor_dec


vty_set:
mov eax, XANIN_VTY_SET
; ecx already set
int 0x81
ret

vty_get:
mov eax, XANIN_VTY_GET
int 0x81
ret

xtb_get:
mov eax, XANIN_XTB_GET
int 0x81
ret 


xtb_init:
mov eax, XANIN_XTB_INIT
mov ebx, [esp - 4]
int 0x81
ret 

__sys_xtf_init:
mov eax, XANIN_XTF_INIT
mov ecx, [esp + 4] ; buffer size
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_xtf_destroy:

ret