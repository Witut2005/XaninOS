extern page_directory

global paging_enable
paging_enable:
    mov dword eax, page_directory;0x20000 << 12 ;page_directory ; eax = address of page_directory
    mov cr3, eax

    mov eax, cr4
    or eax, ((1 << 7) | (1 << 4)) ; PSE (Page Size Extensions) enable 
    mov cr4, eax
    
    mov eax, cr0
    or eax, (1 << 31) ; PG (Paging) bit set

    jmp $
    mov cr0, eax

    ret 