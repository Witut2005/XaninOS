

global _start
extern XaninInitArrayInfo
extern kernel_start

_start:
mov dword [XaninInitArrayInfo], ecx     ;address
mov dword [XaninInitArrayInfo + 4], edx ;sizeof section
mov dword [XaninInitArrayInfo + 8], ebx ;sizeof_section entry
jmp kernel_start
