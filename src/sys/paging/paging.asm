extern page_directory

global paging_enable
paging_enable:
    mov eax, page_directory ; set eax to page_directory
    mov cr3, eax

    mov eax, cr0
    or eax, (1 << 31) ; Paging bit set
    mov cr0, eax
    jmp $
    ret 