[bits 32]

global keyboard_handler_init
extern keyboard_driver

keyboard_handler_init:
	pushfd
    pushad
	call keyboard_driver
	popad
	popfd
	iretd


