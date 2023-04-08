
extern netapi_interrupt_handle
global netapi_interrupt_handle_entry

netapi_interrupt_handle_entry:
    cli
    pushfd
    pushad
    call netapi_interrupt_handle
    popad
    popfd
    sti
    iretd