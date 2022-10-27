[bits 32]

global pit_handler_init

extern pit_handler

pit_handler_init:
    pushad
	cld
	call pit_handler
	popad
	iretd

