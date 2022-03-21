#pragma once

/*  http://www.brokenthorn.com/Resources/OSDevScanCodes.html
    https://www.youtube.com/watch?v=YtnNX074jMU
    xaninOS uses Original XT Scan Code Set  */


#include <stdint.h>


enum special_keys
{
  BSPC = 0xE,
  BSPC_RELEASE = 0x8E,
  ENTER = 0x1C,
  ESC = 0x1,
  CAPS = 0x3A,
  ARROW_UP = 72,
  ARROW_UP_RELEASE = 0xC8,
  ARROW_LEFT = 75,
  ARROW_LEFT_RELEASE = 0xCB,
  ARROW_RIGHT = 77,
  ARROW_RIGHT_RELEASE = 0xCD,
  ARROW_DOWN = 80,
  ARROW_DOWN_RELEASE = 0xD0,
  LSHIFT = 0x2A,
  LSHIFT_RELEASE = 0xAA,
  F3_KEY = 0x3D,
  F4_KEY = 0x3E,
  L_CTRL = 0x1D,
  L_CTRL_RELEASE = 0x9D,
  DELETE_KEY = 0x53,
  W_KEY = 0x11,
  S_KEY = 0x1F,
  O_KEY = 0x18,
  J_KEY = 0x24,
  TAB_KEY = 0xF
};



uint8_t keyboard_scan_code;

uint8_t keyboard_map[128] = {
  // -------- 0 to 9 --------
  ' ',
  ' ', // escape key
  '1','2','3','4','5','6','7','8',
  // -------- 10 to 19 --------
  '9','0','-','=',
  ' ', // Backspace
  ' ', // Tab
  'q','w','e','r',
  // -------- 20 to 29 --------
  't','y','u','i','o','p','[',']',
  '\0', // Enter
  ' ', // left Ctrl
  // -------- 30 to 39 --------
  'a','s','d','f','g','h','j','k','l',';',
  // -------- 40 to 49 --------
  0x27,'`',
  ' ', // left Shift
  0x5C,'z','x','c','v','b','n',
  // -------- 50 to 59 --------
  'm',',','.',
  '/', // slash, or numpad slash if preceded by keycode 224
  ' ', // right Shift
  '*', // numpad asterisk
  ' ', // left Alt
  ' ', // Spacebar
  ' ',
  ' ', // F1
  // -------- 60 to 69 --------
  ' ', // F2
  ' ', // F3
  ' ', // F4
  ' ', // F5
  ' ', // F6
  ' ', // F7
  ' ', // F8
  ' ', // F9
  ' ', // F10
  ' ',
  // -------- 70 to 79 --------
  ' ', // scroll lock
  '7', // numpad 7, HOME key if preceded by keycode 224
  '\0', // numpad 8, up arrow if preceded by keycode 224
  '9', // numpad 9, PAGE UP key if preceded by keycode 224
  '-', // numpad hyphen
  '\0', // numpad 4, left arrow if preceded by keycode 224
  '5', // numpad 5
  '\0', // numpad 6, right arrow if preceded by keycode 224
  ' ',
  '1', // numpad 1, END key if preceded by keycode 224
  // -------- 80 to 89 --------
  '\0', // numpad 2, down arrow if preceded by keycode 224
  '3', // numpad 3, PAGE DOWN key if preceded by keycode 224
  '0', // numpad 0, INSERT key if preceded by keycode 224
  '\0', // numpad dot, DELETE key if preceded by keycode 224
  ' ',' ',' ',' ',' ',' ',
  // -------- 90 to 99 --------
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  // -------- 100 to 109 --------
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  // -------- 110 to 119 --------
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  // -------- 120-127 --------
  ' ',' ',' ',' ',' ',' ',' ',' '


};

