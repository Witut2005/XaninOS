
%include "./sys/call/xanin_sys/calls/terminal/ids.inc"

%macro XANIN_INVOKE_SYSTEM_CALL 0
int 0x81
%endmacro

global vty_set, vty_get, xtb_get, xtb_init
global __sys_xtf_init, __sys_xtf_destroy
global __sys_xtf_buffer_nth_line_index_get, __sys_xtf_buffer_nth_line_size_get, __sys_xtf_line_number_from_position_get
global __sys_xtf_cell_put, __sys_xtf_remove_last_cell, __sys_xtf_buffer_clear, ;__sys_xtf_virtual_cursor_add
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
ESP_GET_NTH_ARGUMENT ecx, 1
ESP_GET_NTH_ARGUMENT edx, 2
ESP_GET_NTH_ARGUMENT ebx, 3
XANIN_INVOKE_SYSTEM_CALL
ret 


__sys_xtb_flush                                 ;(Xtf* XtFrontend);
__sys_xtb_scroll_up                             ;(Xtf* XtFrontend);
__sys_xtb_scroll_down                           ;(Xtf* XtFrontend);
__sys_xtb_flush_all                             ;(Xtf* XtFrontend);


__sys_xtf_init:                                 ;(uint32_t buffer_size);
mov eax, XANIN_XTF_INIT
mov ecx, [esp + 4]                              ; buffer size
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_xtf_destroy:                              ;(Xtf* XtFrontend);
mov eax, XANIN_XTF_DESTROY
mov ecx, [esp + 4]                              ;XtFrontend
XANIN_INVOKE_SYSTEM_CALL

ret

__sys_xtf_buffer_nth_line_index_get:            ;(Xtf* XtFrontend, uint32_t line_number)
mov eax, XANIN_XTF_BUFFER_NTH_LINE_INDEX_GET
ESP_GET_NTH_ARGUMENT ecx, 1                     ;XtFrontend
ESP_GET_NTH_ARGUMENT edx, 2                     ;line number
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_xtf_buffer_nth_line_size_get:             ;(Xtf* XtFrontend, uint32_t line_number)
mov eax, XANIN_XTF_BUFFER_NTH_LINE_SIZE_GET
ESP_GET_NTH_ARGUMENT ecx, 1                     ;XtFrontend
ESP_GET_NTH_ARGUMENT edx, 2                     ;line number
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_xtf_line_number_from_position_get:        ;(Xtf* XtFrontend, uint32_t position);
mov eax, XANIN_XTF_LINE_NUMBER_FROM_POSITION_GET
ESP_GET_NTH_ARGUMENT ecx, 1                     ;XtFrontend
ESP_GET_NTH_ARGUMENT edx, 2                     ;position
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_xtf_cell_put:                             ;(Xtf *XtFrontend, char c, uint8_t color);
mov eax, XANIN_XTF_CELL_PUT
ESP_GET_NTH_ARGUMENT ecx, 1                     ;XtFrontend
ESP_GET_NTH_ARGUMENT edx, 2                     ;c
ESP_GET_NTH_ARGUMENT ebx, 3                     ;color
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_xtf_remove_last_cell:                     ;(Xtf* XtFrontend);
mov eax, XANIN_XTF_REMOVE_LAST_CELL
ESP_GET_NTH_ARGUMENT ecx, 1                     ;XtFrontend
XANIN_INVOKE_SYSTEM_CALL
ret

; __sys_xtf_virtual_cursor_add: ;(Xtf* XtFrontend, color_t color);
; ESP_GET_NTH_ARGUMENT ecx, 1; XtFrontend
; ESP_GET_NTH_ARGUMENT edx, 2; color
; XANIN_INVOKE_SYSTEM_CALL
; ret

__sys_xtf_buffer_clear:                         ;(Xtf* XtFrontend);
mov eax, XANIN_XTF_BUFFER_CLEAR
ESP_GET_NTH_ARGUMENT ecx, 1                     ;XtFrontend
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_xtf_scrolling_on:                         ;(Xtf* XtFrontend);
mov eax, XANIN_XTF_SCROLLING_ON
ESP_GET_NTH_ARGUMENT ecx, 1                     ;XtFrontend
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_xtf_scrolling_off:                        ;(Xtf* XtFrontend);
mov eax, XANIN_XTF_SCROLLING_OFF
ESP_GET_NTH_ARGUMENT ecx, 1                     ;XtFrontend
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_xtf_cursor_on:                            ;(Xtf* XtFrontend, color_t color);
mov eax, XANIN_XTF_CURSOR_ON
ESP_GET_NTH_ARGUMENT ecx, 1                     ;XtFrontend  
ESP_GET_NTH_ARGUMENT edx, 2                     ;color
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_xtf_cursor_off:                           ;(Xtf* XtFrontend);
mov eax, XANIN_XTF_CURSOR_OFF
ESP_GET_NTH_ARGUMENT ecx, 1                     ;XtFrontend  
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_xtf_cursor_inc:                           ;(Xtf *XtFrontend);
mov eax, XANIN_XTF_CURSOR_INC
ESP_GET_NTH_ARGUMENT ecx, 1                     ;XtFrontend  
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_xtf_cursor_dec:                           ;(Xtf *XtFrontend);
mov eax, XANIN_XTF_CURSOR_DEC
ESP_GET_NTH_ARGUMENT ecx, 1                     ;XtFrontend  
XANIN_INVOKE_SYSTEM_CALL
ret