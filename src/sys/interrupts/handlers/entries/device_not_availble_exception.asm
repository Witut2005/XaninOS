

global device_not_available_exception_entry 
extern device_not_available_exception_handler
extern exit

section .data

section .text
device_not_available_exception_entry:
    cli
    pushfd
    pusha
    call device_not_available_exception_handler
    popa
    popfd
    sti
    call exit
