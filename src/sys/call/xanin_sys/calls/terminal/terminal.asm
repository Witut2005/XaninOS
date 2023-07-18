
%define XANIN_VTY_SET 2002
%define XANIN_VTY_GET 2003
%define XANIN_XTB_GET 2004
%define XANIN_XTB_INIT 2005

; %define XANIN_XTF_INIT 2004
; %define XANIN_XTF_BUFFER_NTH_LINE_SIZE_GET 2005
; %define XANIN_XTF_GET_NUMBER_OF_LINES 2006
; %define XANIN_XTF_BUFFER_NTH_LINE_INDEX_GET 2007
; %define XANIN_XTF_REMOVE_LAST_CELL 2008
; %define XANIN_XTF_CURSOR_ON 2009
; %define XANIN_XTF_CURSOR_OFF 2010
; %define XANIN_XTF_SCROLLING_ON 2011
; %define XANIN_XTF_SCROLLING_OFF 2012
; %define XANIN_XTF_BUFFER_CLEAR 2013

global vty_set, vty_get, xtb_get, xtb_init

vty_set:
mov eax, XANIN_VTY_SET
; ecx already set
int 0x81
ret

vty_get:
mov eax, XANIN_VTY_GET
int 0x81
ret

xtb_get:
mov eax, XANIN_XTB_GET
int 0x81
ret 


xtb_init:
mov eax, XANIN_XTB_INIT
mov ebx, [esp - 4]
int 0x81
ret 