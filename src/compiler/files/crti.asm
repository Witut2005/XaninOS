;; (C) 2018 Srimanta Barua
;;
;; C runtime
;; This file defines the beginning of the _init and _fini functions which
;; initialize and then shut down the C runtime. These are required for us
;; to call C code

[BITS 32]

;; The .init section contains the _init function
section .init

global _init
_init:
	push	ebp
	mov	ebp, esp
	;; GCC will put the contents of crtbegin.o's .init section here


;; The .fini section contains the _fini function
section .fini

global	_fini
_fini:
	push	ebp
	mov	ebp, esp
        ;; GCC will put the contents of crtbegin.o's .fini section here