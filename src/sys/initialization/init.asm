
;znajdz _init_array
;tam masz po kolei zdefiniowane wskazniki na funckje inicjalizujace

;PRZYKLAD

; .init_array:0806F008 ; ===========================================================================
; .init_array:0806F008
; .init_array:0806F008 ; Segment type: Pure data
; .init_array:0806F008 ; Segment permissions: Read/Write
; .init_array:0806F008 _init_array     segment dword public 'DATA' use32
; .init_array:0806F008                 assume cs:_init_array
; .init_array:0806F008                 ;org 806F008h
; .init_array:0806F008 off_806F008     dd offset _GLOBAL__sub_I_IcmpResponse
; .init_array:0806F008                                         ; DATA XREF: LOAD:0804805C↑o
; .init_array:0806F00C                 dd offset _GLOBAL__sub_I_ArpTable
; .init_array:0806F010                 dd offset _GLOBAL__sub_I_KeyInfo
; .init_array:0806F014                 dd offset _GLOBAL__sub_I__Z22first_nonempty_row_getv
; .init_array:0806F018                 dd offset _GLOBAL__sub_I_screenshot
; .init_array:0806F01C                 dd offset _GLOBAL__sub_I__Z8cpp_progv
; .init_array:0806F01C _init_array     ends
; .init_array:0806F01C
; .data:0806F020 ; ===========================================================================

