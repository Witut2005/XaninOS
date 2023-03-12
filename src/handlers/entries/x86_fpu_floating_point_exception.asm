global x86_fpu_floating_point_exception_entry 
extern x86_fpu_floating_point_exception_handler
extern exit

section .data

section .text
x86_fpu_floating_point_exception_entry:
    cli
    pushfd
    pusha
    call x86_fpu_floating_point_exception_handler
    popa
    popfd
    sti
    call exit

