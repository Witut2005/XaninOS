# Xanin operating system coding style

This document decribes coding style for xanin operating system development.
Every naming mistake will be corrected in the near future.

---

## Naming convention

When you use a function or variable use underscore to separate words from each other:

```c
uint16_t my_variable;
uint32_t set_color(uint8_t background_color, uint8_t font_color);
```

For structs use PascalCase.
PascalCase format looks like this:

```c
struct FileSystem;
```

First use the name of the object and then its properties.

Good:

```c
uint8_t screen_color;
uint8_t keyboard_input;
```

Bad:

```c
uint8_t color_screen;
uint8_t input_keyboard;
```

---

## Constants and macros

Names of constants and macros should be written using UPPERCASE letters with underscores separating words from each other:

Good:

```c
#define KEYBOARD_SIG_OFF()
const uint32_t FILE_SYSTEM_ENTRY_OFFSET;
```
Bad:
```c
#define keyboard_sig_off()
const uint32_t fileSystemEntryOffset
```