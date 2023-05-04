
%define MULTIBOOT2_MAGIC 0xE85250D6
%define XANIN_MULTIBOOT_SETTINGS 0

section .multiboot

header:

dd MULTIBOOT2_MAGIC 
dd XANIN_MULTIBOOT_SETTINGS

dd end - header ; length

dd 0x100000000 - (MULTIBOOT2_MAGIC + XANIN_MULTIBOOT_SETTINGS) ;checksum

dw 0
dw 0
dd 8


end: