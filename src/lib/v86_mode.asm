
global v86_mode_enter

v86_mode_enter:
   mov ebp, esp               ; save stack pointer

   push word  0x0        ; ss
   push word  0x1000        ; esp
   pushf                     ; eflags
   or word [esp], (1 << 17)  ; set VM flags
   push word 0x0        ; cs
   push word  0x7c02       ; eip
   iret
