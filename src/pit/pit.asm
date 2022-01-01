[bits 32]

global pitHandlerInit

extern pitHandler

pitHandlerInit:
    pushad
	cld
	call pitHandler
	popad
	mov al,0x20
	out 0x20,al ; SEND EOI TO PIC1
	iretd

