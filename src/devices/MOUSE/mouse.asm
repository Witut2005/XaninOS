
[bits 32]

global mouse_handler_init
extern my_mouse_handler


mouse_handler_init:
    pushad
	cld
	call my_mouse_handler
	popad
	mov al,0x20
	out 0x20,al ; SEND EOI TO PIC1
    out 0xA0, al
	iretd