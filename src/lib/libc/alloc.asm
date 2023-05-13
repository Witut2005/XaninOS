

global free 

free:

mov eax, 103
; ecx have ptr to free
int 0x81
ret