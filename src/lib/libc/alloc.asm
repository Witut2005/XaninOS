
%define MALLOC_SYSCALL_ID 100
%define CALLOC_SYSCALL_ID 101
%define REALLOC_SYSCALL_ID 102
%define FREE_SYSCALL_ID 103

global free 

free:

mov eax, FREE_SYSCALL_ID
    ; ecx have ptr to free
    int 0x81
    ret

; ---------------------------------

global realloc

realloc:
    mov eax, REALLOC_SYSCALL_ID
    int 0x81
    ; ecx, edx are set properly already
    ret ; eax already holds return value

; ---------------------------------

global malloc
malloc:
    mov eax, MALLOC_SYSCALL_ID
    ; ecx, edx are set properly already
    int 0x81
    ret ; eax already holds return value

; ---------------------------------

global calloc
calloc:
    mov eax, CALLOC_SYSCALL_ID
    ; ecx, edx are set properly already
    int 0x81
    ret ; eax already holds return value