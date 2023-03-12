
global invalid_opcode_exception_entry
extern invalid_opcode_exception_handler
extern exit

section .data

section .text
invalid_opcode_exception_entry:
    cli
    pushfd
    pusha
    call invalid_opcode_exception_handler 
    popa
    popfd
    sti
    call exit
