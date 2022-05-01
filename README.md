# xaninOS

32-bit protected mode operating system made for educational purposes

Features:

- own bootloader
- interrupt descriptor table (IDT)
- all exceptions are handled
- debugging commands(reg_dump, load, loadch)
- configurated PIC
- configurated keyboard controller
- note (notepad)
- paint
- recreated some parts of C libaries
- file system manipulation commands (create, mkdir)
- usb detector
- own file system (xin file system)
- pong game


---

## Directories

```
bin/ - binary files
src/ - source files
```

## XaninOS commands

```
"help" or "h"               //prints all available commands
"help" or "h" command_name  //prints short manual about the command
```

## Running XaninOS

```
make run (you can find this file inside /src directory)
```

## Running XaninOS on Windows

```
.run_windows.bat (you can find this file inside /src directory)
```

## Building XaninOS from source

```
build.sh (you can find this file inside /src directory)
```
