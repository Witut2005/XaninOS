
#pragma once

// XinFs
#define XANIN_FOPEN 10
#define XANIN_FREAD 11
#define XANIN_FWRITE 12
#define XANIN_FCLOSE 13

#define XANIN_OPEN 14
#define XANIN_READ 15
#define XANIN_WRITE 16
#define XANIN_CLOSE 17

// Memory Allocation

#define XANIN_ALLOCATE 100
#define XANIN_CALLOCATE 101
#define XANIN_REALLOCATE 102
#define XANIN_FREE 103

// Input

#define XANIN_INPUT_GET 200
#define XANIN_GETCHAR 201
#define XANIN_GETSCAN 202
#define XANIN_INPUTG 203
#define XANIN_KEYINFO_GET 204

// Disk
#define XANIN_DISK_READ 300
#define XANIN_DISK_WRITE 301

#define XANIN_ARGC_GET 400
#define XANIN_ARGV_GET 401

#define XANIN_STDIO_MODE_SET 2000
#define XANIN_STDIO_MODE_GET 2001

#define XANIN_VTY_SET 2002
#define XANIN_VTY_GET 2003
#define XANIN_XTB_GET 2004
#define XANIN_XTB_INIT 2005
#define XANIN_XTB_FLUSH 2006
#define XANIN_XTB_FLUSH_ALL 2007
#define XANIN_XTB_SCROLL_UP 2008
#define XANIN_XTB_SCROLL_DOWN 2009

#define XANIN_XTF_INIT 2030
#define XANIN_XTF_DESTROY 2031
#define XANIN_XTF_BUFFER_NTH_LINE_INDEX_GET 2032
#define XANIN_XTF_BUFFER_NTH_LINE_SIZE_GET 2033
#define XANIN_XTF_LINE_NUMBER_FROM_POSITION_GET 2034

#define XANIN_XTF_CELL_PUT 2035
#define XANIN_XTF_REMOVE_LAST_CELL 2036
#define XANIN_XTF_VIRUTAL_CURSOR_ADD 2037
#define XANIN_XTF_BUFFER_CLEAR 2038
#define XANIN_XTF_SCROLLING_ON 2039
#define XANIN_XTF_SCROLLING_OFF 2040
#define XANIN_XTF_CURSOR_ON 2041
#define XANIN_XTF_CURSOR_OFF 2042
#define XANIN_XTF_CURSOR_INC 2043
#define XANIN_XTF_CURSOR_DEC 2044
#define XANIN_XTF_CHARACTER_PUT 2045

#define XANIN_SCREEN_INIT 2200
#define XANIN_SCREEN_LETTERS_REFRESH 2201
#define XANIN_SCREEN_LETTERS_REFRESH_ADD 2202


#define XANIN_VGA_BUFFER_SEGMENT_GET 20000
#define XANIN_VGA_TEXT_MODE_WIDTH_GET 20001
#define XANIN_VGA_TEXT_MODE_HEIGHT_GET 20002