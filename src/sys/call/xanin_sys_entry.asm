
extern xanin_sys_handle
global xanin_sys_entry

section .text

xanin_sys_entry:

pushfd

push ecx
push edx
push ebx
push esp
push ebp
push esi
push edi

call xanin_sys_handle

pop edi ; DONT RESTORE EAX (HOLDS RETURN VALUE)
pop esi 
pop ebp
pop esp
pop ebx
pop edx
pop ecx

popfd

iretd
