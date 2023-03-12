
global virtualization_exception_entry 
extern virtualization_exception_handler
extern exit

section .data

section .text
virtualization_exception_entry:
    cli
    pushfd
    pusha
    call virtualization_exception_handler
    popa
    popfd
    sti
    call exit
