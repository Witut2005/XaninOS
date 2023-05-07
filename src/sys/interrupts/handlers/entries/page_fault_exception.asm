
global page_fault_exception_entry 
extern page_fault_exception_handler
extern exit

section .data

section .text
page_fault_exception_entry:
    cli
    pushfd
    pusha
    call page_fault_exception_handler
    popa
    popfd
    sti
    call exit
