
global __asm_reboot

__asm_reboot:

lidt 0x0
mov edx
div edx