

global bound_range_exceeded_exception_entry 
extern bound_range_exceeded_exception_handler
extern exit

section .data

section .text
bound_range_exceeded_exception_entry:
    cli
    pushfd
    pusha
    call bound_range_exceeded_exception_handler 
    popa
    popfd
    sti
    call exit
