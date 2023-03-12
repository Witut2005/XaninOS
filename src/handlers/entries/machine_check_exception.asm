
global machine_check_exception_entry 
extern machine_check_exception_handler
extern exit

section .data

section .text
machine_check_exception_entry:
    cli
    pushfd
    pusha
    call machine_check_exception_handler
    popa
    popfd
    sti
    call exit
