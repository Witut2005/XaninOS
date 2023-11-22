

%ifidn __OUTPUT_FORMAT__, elf32 
  %define PTR_SIZE 4
%elifidn __OUTPUT_FORMAT__, elf64 
 %define PTR_SIZE 8
%endif

%macro ESP_GET_NTH_ARGUMENT 2
    mov %1, [esp + (%2 * PTR_SIZE)]
%endmacro

%macro XANIN_INVOKE_SYSTEM_CALL 0
int 0x81
sti
%endmacro