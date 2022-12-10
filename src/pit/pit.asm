[bits 32]

global pit_handler_init

extern pit_handler

pit_handler_init:
    pushfd
    pushad
	call pit_handler
	popad
	popfd
	iretd

