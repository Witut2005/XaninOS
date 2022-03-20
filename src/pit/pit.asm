[bits 32]

global pit_handler_init

extern pit_handler

pit_handler_init:
    pushad
	cld
	call pit_handler
	popad
	mov al,0x20
	out 0x20,al ; SEND EOI TO PIC1
	iretd

