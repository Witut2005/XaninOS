# xaninOS

32-bit protected mode operating system made for educational purposes

Features:

- own bootloader
- interrupt descriptor table
- configurated PIC
- configurated keyboard controller
- pong game
- petris (paint and tetris in one :b)
- recreated some parts of C libaries
- basic implementation of USTAR filesystem
- file system manipulation commands (touch, md, ls etc.)
- xanin can execute external programs (in 32-bit .bin format)
- usb detector
- you can very easily attach other archive file to kernel

---

## DIRECTORIES

```
bin/ - binary files
src/ - source files
```

---

Command to list all available commands within xaninOS:

##XANIN COMMANDS

```
"help" or "h"
```

##RUNNING XANIN

```
run_linux.sh (you can find this file inside /src directory)
OR
bochs.exe -f ./osdev.bochsrc
```
