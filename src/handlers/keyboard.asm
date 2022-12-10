[bits 32]

global keyboard_handler_init
extern keyboard_handler

keyboard_handler_init:
	pushfd
    pushad
	call keyboard_handler
	popad
	popfd
	iretd


