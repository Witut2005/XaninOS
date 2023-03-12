

global segment_not_present_exception_entry 
extern segment_not_present_exception_handler
extern exit

section .data

section .text
segment_not_present_exception_entry:
    cli
    pushfd
    pusha
    call segment_not_present_exception_handler
    popa
    popfd
    sti
    call exit
