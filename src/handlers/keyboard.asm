[bits 32]

global keyboard_handler_init
extern keyboard_handler

keyboard_handler_init:
    pushad
	cld
	call keyboard_handler
	popad
	mov al,0x20
	out 0x20,al ; SEND EOI TO PIC1
	iretd


