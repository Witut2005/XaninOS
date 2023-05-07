

global breakpoint_exception_entry 
extern breakpoint_exception_handler
extern exit

section .data

section .text
breakpoint_exception_entry:
    cli
    pushfd
    pusha
    call breakpoint_exception_handler
    popa
    popfd
    sti
    call exit
