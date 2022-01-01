
[org 0x7c00] ;there we start execution
[bits 16]; <-- 16bit real mode


cli
jmp _loadSector


_loadSector:

;https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=02h:_Read_Sectors_From_Drive

mov ax,0x2000
mov es,ax    
 
mov ah,0x2 
mov al,0x20 
mov ch,0x0
mov cl,0x2 ; we dont want to copy first sector
mov dh,0x0  
mov bx,0x0

int 0x13

mov ah,0xe
mov si,str

print_msg:
lodsb
cmp al,0
je jmp_ker_load
int 0x10
jmp print_msg



jmp_ker_load:
jmp word 0x2000:0000


disk_num: dw 0x0
str: db "ERROR your computer doesnt support fully int13 xaninOS cant be loaded",0 

times 510 - ($-$$) db 0x0

dw 0xaa55




