[bits 32]

global keyboard_handler_init
extern keyboard_handler
extern eoi_send

keyboard_handler_init:
	pushfd
    pushad
	call keyboard_handler
	popad
	popfd
	call eoi_send
	sti
	iretd


