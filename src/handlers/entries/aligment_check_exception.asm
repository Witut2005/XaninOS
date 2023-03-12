
global aligment_check_exception_entry 
extern aligment_check_exception_handler
extern exit

section .data

section .text
aligment_check_exception_entry:
    cli
    pushfd
    pusha
    call aligment_check_exception_handler
    popa
    popfd
    sti
    call exit
