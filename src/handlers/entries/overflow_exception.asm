


global overflow_exception_entry 
extern overflow_exception_handler
extern exit

section .data

section .text
overflow_exception_entry:
    cli
    pushfd
    pusha
    call overflow_exception_handler
    popa
    popfd
    sti
    call exit
