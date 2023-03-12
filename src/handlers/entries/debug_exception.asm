


global debug_exception_entry 
extern debug_exception_handler
extern exit

section .data

section .text
debug_exception_entry:
    cli
    pushfd
    pusha
    call debug_exception_handler
    popa
    popfd
    sti
    call exit
