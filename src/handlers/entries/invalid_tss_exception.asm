

global invalid_tss_exception_entry 
extern invalid_tss_exception_handler
extern exit

section .data

section .text
invalid_tss_exception_entry:
    cli
    pushfd
    pusha
    call invalid_tss_exception_handler
    popa
    popfd
    sti
    call exit
