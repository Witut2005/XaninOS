


global double_fault_exception_entry 
extern double_fault_exception_handler
extern exit

section .data

section .text
double_fault_exception_entry:
    cli
    pushfd
    pusha
    call double_fault_exception_handler
    popa
    popfd
    sti
    call exit
