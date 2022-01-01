[bits 32]

global keyboardHandlerInit


extern keyboardHandler

keyboardHandlerInit:
    pushad
	cld
	call keyboardHandler
	popad
	mov al,0x20
	out 0x20,al ; SEND EOI TO PIC1
	iretd


