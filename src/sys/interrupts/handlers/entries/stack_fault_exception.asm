
global stack_fault_exception_entry 
extern stack_fault_exception_handler
extern exit

section .data

section .text
stack_fault_exception_entry:
    cli
    pushfd
    pusha
    call stack_fault_exception_handler
    popa
    popfd
    sti
    call exit

