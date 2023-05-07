

global divide_by_zero_exception_entry
extern divide_by_zero_exception_handler
extern exit

section .data

section .text
divide_by_zero_exception_entry:
    cli
    pushfd
    pusha
    call divide_by_zero_exception_handler
    popa
    popfd
    sti
    call exit


    


