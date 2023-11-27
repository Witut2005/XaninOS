

/*
  http://www.brokenthorn.com/Resources/OSDevScanCodes.html
  https://www.youtube.com/watch?v=YtnNX074jMU
  xaninOS uses Original XT Scan Code Set
*/

#include <sys/devices/keyboard/scan_codes.h>
#include <stdint.h>

uint8_t keyboard_map[128] = {
    // -------- 0 to 9 --------
    ' ',
    ' ', // escape key
    '1', '2', '3', '4', '5', '6', '7', '8',
    // -------- 10 to 19 --------
    '9', '0', '-', '=',
    ' ', // Backspace
    ' ', // Tab
    'q', 'w', 'e', 'r',
    // -------- 20 to 29 --------
    't', 'y', 'u', 'i', 'o', 'p', '[', ']',
    '\0', // Enter
    ' ',  // left Ctrl
    // -------- 30 to 39 --------
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    // -------- 40 to 49 --------
    0x27, '`',
    ' ', // left Shift
    0x5C, 'z', 'x', 'c', 'v', 'b', 'n',
    // -------- 50 to 59 --------
    'm', ',', '.',
    '/',  // slash, or numpad slash if preceded by keycode 224
    '\0', // right Shift
    '\0', // numpad asterisk
    '\0', // left Alt
    ' ',  // Spacebar
    '\0', // caps
    '\0', // F1
    // -------- 60 to 69 --------
    '\0', // F2
    '\0', // F3
    '\0', // F4
    '\0', // F5
    '\0', // F6
    '\0', // F7
    '\0', // F8
    '\0', // F9
    '\0', // F10
    '\0', // num lock
    // -------- 70 to 79 --------
    '\0', // scroll lock
    '7',  // numpad 7, HOME key if preceded by keycode 224
    '\0', // numpad 8, up arrow if preceded by keycode 224
    '9',  // numpad 9, PAGE UP key if preceded by keycode 224
    '-',  // numpad hyphen
    '\0', // numpad 4, left arrow if preceded by keycode 224
    '5',  // numpad 5
    '\0', // numpad 6, right arrow if preceded by keycode 224
    ' ',
    '1', // numpad 1, END key if preceded by keycode 224
    // -------- 80 to 89 --------
    '\0', // numpad 2, down arrow if preceded by keycode 224
    '3',  // numpad 3, PAGE DOWN key if preceded by keycode 224
    '0',  // numpad 0, INSERT key if preceded by keycode 224
    '\0', // numpad dot, DELETE key if preceded by keycode 224
    '\0', '\0', '\0', '\0', '\0', '\0',
    // -------- 90 to 99 --------
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    // -------- 100 to 109 --------
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    // -------- 110 to 119 --------
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    // -------- 120-127 --------
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'

};
