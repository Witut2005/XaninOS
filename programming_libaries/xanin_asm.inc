

%macro RET_DEFAULT_16 0
pushf
push 0x2000
push 0x0
iret
%endmacro
