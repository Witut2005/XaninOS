

global coprocessor_segment_overrun_exception_entry 
extern coprocessor_segment_overrun_exception_handler
extern exit

section .data

section .text
coprocessor_segment_overrun_exception_entry:
    cli
    pushfd
    pusha
    call coprocessor_segment_overrun_exception_handler
    popa
    popfd
    sti
    call exit
