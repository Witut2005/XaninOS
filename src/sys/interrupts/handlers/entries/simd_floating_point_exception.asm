
global simd_floating_point_exception_entry 
extern simd_floating_point_exception_handler
extern exit

section .data

section .text
simd_floating_point_exception_entry:
    cli
    pushfd
    pusha
    call simd_floating_point_exception_handler
    popa
    popfd
    sti
    call exit
