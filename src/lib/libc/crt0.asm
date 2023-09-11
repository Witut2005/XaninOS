section .text

extern main

%define XANIN_ARGC_GET 400
%define XANIN_ARGV_GET 401

global _start
_start:
	; # Set up end of the stack frame linked list.
	; movq $0, %rbp
	; pushq %rbp # rip=0
	; pushq %rbp # rbp=0
	; movq %rsp, %rbp

	; # We need those in a moment when we call main.
	; pushq %rsi
	; pushq %rdi

	; # Prepare signals, memory allocation, stdio and such.
	; call initialize_standard_library

	; # Run the global constructors.
	; call _init

	; # Restore argc and argv.
	; popq %rdi
	; popq %rsi

	; call screen_init ; init XaninOS screen  

	mov eax, XANIN_ARGV_GET
	int 0x81
	push eax

	mov eax, XANIN_ARGC_GET
	int 0x81
	push eax

	; # Run main
	call main

	; # Terminate the process with the exit code.
	; movl %eax, %edi
	; call exit
	mov eax, 1
	int 0x80
    ; ret
