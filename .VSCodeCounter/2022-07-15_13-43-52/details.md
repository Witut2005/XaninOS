# Details

Date : 2022-07-15 13:43:52

Directory /home/witut/Desktop/xaninOS

Total : 176 files,  9820 codes, 808 comments, 4204 blanks, all 14832 lines

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)

## Files
| filename | language | code | comment | blank | total |
| :--- | :--- | ---: | ---: | ---: | ---: |
| [README.md](/README.md) | Markdown | 39 | 0 | 16 | 55 |
| [documentation/TODO.md](/documentation/TODO.md) | Markdown | 7 | 0 | 5 | 12 |
| [documentation/disk_image.md](/documentation/disk_image.md) | Markdown | 10 | 0 | 4 | 14 |
| [documentation/xanin_coding_style.md](/documentation/xanin_coding_style.md) | Markdown | 39 | 0 | 17 | 56 |
| [programming_libaries/xanin_asm.inc](/programming_libaries/xanin_asm.inc) | x86 and x86_64 Assembly | 6 | 0 | 3 | 9 |
| [src/.run_linux.sh](/src/.run_linux.sh) | Shell Script | 1 | 3 | 2 | 6 |
| [src/IDT/idt.c](/src/IDT/idt.c) | C | 74 | 10 | 28 | 112 |
| [src/Makefile](/src/Makefile) | Makefile | 73 | 0 | 30 | 103 |
| [src/app_config.h](/src/app_config.h) | C++ | 33 | 1 | 3 | 37 |
| [src/boot/boot.asm](/src/boot/boot.asm) | x86 and x86_64 Assembly | 91 | 55 | 48 | 194 |
| [src/boot/disk_data_loader.asm](/src/boot/disk_data_loader.asm) | x86 and x86_64 Assembly | 22 | 3 | 14 | 39 |
| [src/boot/kernelLoader.asm](/src/boot/kernelLoader.asm) | x86 and x86_64 Assembly | 172 | 29 | 84 | 285 |
| [src/boot/locateFS.asm](/src/boot/locateFS.asm) | x86 and x86_64 Assembly | 2 | 0 | 1 | 3 |
| [src/chip8/chip8.cpp](/src/chip8/chip8.cpp) | C++ | 58 | 0 | 45 | 103 |
| [src/chip8/chip8.hpp](/src/chip8/chip8.hpp) | C++ | 39 | 1 | 27 | 67 |
| [src/chip8/chip8_opcodes.hpp](/src/chip8/chip8_opcodes.hpp) | C++ | 22 | 3 | 23 | 48 |
| [src/devices/ACPI/ACPI.c](/src/devices/ACPI/ACPI.c) | C | 134 | 0 | 48 | 182 |
| [src/devices/ACPI/ACPI.h](/src/devices/ACPI/ACPI.h) | C++ | 135 | 2 | 48 | 185 |
| [src/devices/APIC/apic.cpp](/src/devices/APIC/apic.cpp) | C++ | 33 | 2 | 15 | 50 |
| [src/devices/APIC/apic.h](/src/devices/APIC/apic.h) | C++ | 9 | 0 | 3 | 12 |
| [src/devices/APIC/apic.hpp](/src/devices/APIC/apic.hpp) | C++ | 42 | 0 | 17 | 59 |
| [src/devices/APIC/apic_registers.h](/src/devices/APIC/apic_registers.h) | C++ | 58 | 0 | 11 | 69 |
| [src/devices/COM/com.cpp](/src/devices/COM/com.cpp) | C++ | 61 | 0 | 24 | 85 |
| [src/devices/COM/com.hpp](/src/devices/COM/com.hpp) | C++ | 21 | 0 | 14 | 35 |
| [src/devices/DMA/dma.c](/src/devices/DMA/dma.c) | C | 85 | 10 | 32 | 127 |
| [src/devices/DMA/dma.h](/src/devices/DMA/dma.h) | C++ | 76 | 0 | 32 | 108 |
| [src/devices/HARD_DISK/disk.asm](/src/devices/HARD_DISK/disk.asm) | x86 and x86_64 Assembly | 81 | 3 | 27 | 111 |
| [src/devices/HARD_DISK/disk.c](/src/devices/HARD_DISK/disk.c) | C | 93 | 5 | 62 | 160 |
| [src/devices/HARD_DISK/disk.h](/src/devices/HARD_DISK/disk.h) | C++ | 31 | 1 | 20 | 52 |
| [src/devices/HARD_DISK/load_kernel.asm](/src/devices/HARD_DISK/load_kernel.asm) | x86 and x86_64 Assembly | 10 | 0 | 9 | 19 |
| [src/devices/IOAPIC/ioapic.cpp](/src/devices/IOAPIC/ioapic.cpp) | C++ | 58 | 0 | 20 | 78 |
| [src/devices/IOAPIC/ioapic.h](/src/devices/IOAPIC/ioapic.h) | C++ | 21 | 0 | 11 | 32 |
| [src/devices/IOAPIC/ioapic.hpp](/src/devices/IOAPIC/ioapic.hpp) | C++ | 81 | 0 | 27 | 108 |
| [src/devices/MOUSE/mouse.asm](/src/devices/MOUSE/mouse.asm) | x86 and x86_64 Assembly | 12 | 0 | 4 | 16 |
| [src/devices/MOUSE/mouse.c](/src/devices/MOUSE/mouse.c) | C | 76 | 31 | 23 | 130 |
| [src/devices/NIC/8254x.cpp](/src/devices/NIC/8254x.cpp) | C++ | 88 | 17 | 53 | 158 |
| [src/devices/NIC/8254x.hpp](/src/devices/NIC/8254x.hpp) | C++ | 126 | 1 | 38 | 165 |
| [src/devices/PCI/pci.c](/src/devices/PCI/pci.c) | C | 221 | 3 | 160 | 384 |
| [src/devices/PCI/pci.h](/src/devices/PCI/pci.h) | C++ | 125 | 0 | 42 | 167 |
| [src/devices/USB/ehci.c](/src/devices/USB/ehci.c) | C | 0 | 0 | 1 | 1 |
| [src/devices/USB/ehci.h](/src/devices/USB/ehci.h) | C++ | 0 | 0 | 1 | 1 |
| [src/devices/USB/usb.c](/src/devices/USB/usb.c) | C | 38 | 49 | 23 | 110 |
| [src/devices/USB/usb.h](/src/devices/USB/usb.h) | C++ | 9 | 0 | 13 | 22 |
| [src/devices/VGA/vga.c](/src/devices/VGA/vga.c) | C | 188 | 14 | 74 | 276 |
| [src/devices/VGA/vga.h](/src/devices/VGA/vga.h) | C++ | 580 | 18 | 22 | 620 |
| [src/devices/mouse.cpp](/src/devices/mouse.cpp) | C++ | 0 | 201 | 0 | 201 |
| [src/elf/elf_loader.c](/src/elf/elf_loader.c) | C | 73 | 0 | 37 | 110 |
| [src/elf/elf_loader.h](/src/elf/elf_loader.h) | C++ | 7 | 0 | 3 | 10 |
| [src/game_engine/xagame.cpp](/src/game_engine/xagame.cpp) | C++ | 61 | 0 | 25 | 86 |
| [src/game_engine/xagame.hpp](/src/game_engine/xagame.hpp) | C++ | 43 | 14 | 16 | 73 |
| [src/gyn_cl/gyn.c](/src/gyn_cl/gyn.c) | C | 41 | 4 | 20 | 65 |
| [src/gyn_cl/gyn.h](/src/gyn_cl/gyn.h) | C++ | 8 | 0 | 6 | 14 |
| [src/handlers/handlers.c](/src/handlers/handlers.c) | C | 215 | 4 | 39 | 258 |
| [src/handlers/handlers.h](/src/handlers/handlers.h) | C++ | 16 | 2 | 13 | 31 |
| [src/handlers/keyboard.asm](/src/handlers/keyboard.asm) | x86 and x86_64 Assembly | 11 | 0 | 5 | 16 |
| [src/headers/macros.h](/src/headers/macros.h) | C++ | 5 | 0 | 5 | 10 |
| [src/kernel.c](/src/kernel.c) | C | 182 | 25 | 108 | 315 |
| [src/keyboard/key_map.h](/src/keyboard/key_map.h) | C++ | 56 | 18 | 12 | 86 |
| [src/keyboard/keyboard_driver.c](/src/keyboard/keyboard_driver.c) | C | 108 | 0 | 35 | 143 |
| [src/keyboard/keyboard_init.c](/src/keyboard/keyboard_init.c) | C | 22 | 0 | 16 | 38 |
| [src/keyboard/scan_codes.h](/src/keyboard/scan_codes.h) | C++ | 45 | 0 | 0 | 45 |
| [src/libc/algorithm.h](/src/libc/algorithm.h) | C++ | 18 | 0 | 7 | 25 |
| [src/libc/alloc.h](/src/libc/alloc.h) | C++ | 95 | 10 | 40 | 145 |
| [src/libc/assert.h](/src/libc/assert.h) | C++ | 11 | 0 | 4 | 15 |
| [src/libc/clock.h](/src/libc/clock.h) | C++ | 1 | 0 | 2 | 3 |
| [src/libc/colors.h](/src/libc/colors.h) | C++ | 21 | 1 | 8 | 30 |
| [src/libc/enable_sse.asm](/src/libc/enable_sse.asm) | x86 and x86_64 Assembly | 8 | 0 | 3 | 11 |
| [src/libc/enter_real_mode.asm](/src/libc/enter_real_mode.asm) | x86 and x86_64 Assembly | 43 | 0 | 19 | 62 |
| [src/libc/hal.h](/src/libc/hal.h) | C++ | 180 | 4 | 61 | 245 |
| [src/libc/math.h](/src/libc/math.h) | C++ | 13 | 0 | 12 | 25 |
| [src/libc/memory.h](/src/libc/memory.h) | C++ | 21 | 0 | 9 | 30 |
| [src/libc/parameter.h](/src/libc/parameter.h) | C++ | 9 | 0 | 5 | 14 |
| [src/libc/stdiox.h](/src/libc/stdiox.h) | C | 527 | 7 | 263 | 797 |
| [src/libc/stdlibx.h](/src/libc/stdlibx.h) | C++ | 272 | 9 | 100 | 381 |
| [src/libc/string.h](/src/libc/string.h) | C++ | 270 | 18 | 138 | 426 |
| [src/libc/syslog.h](/src/libc/syslog.h) | C++ | 29 | 0 | 22 | 51 |
| [src/libc/time.h](/src/libc/time.h) | C++ | 28 | 0 | 16 | 44 |
| [src/libcpp/algorithm.h](/src/libcpp/algorithm.h) | C++ | 80 | 0 | 34 | 114 |
| [src/libcpp/array.h](/src/libcpp/array.h) | C++ | 91 | 0 | 46 | 137 |
| [src/libcpp/box.h](/src/libcpp/box.h) | C++ | 22 | 0 | 11 | 33 |
| [src/libcpp/chal.h](/src/libcpp/chal.h) | C++ | 17 | 0 | 4 | 21 |
| [src/libcpp/cmath.h](/src/libcpp/cmath.h) | C++ | 6 | 0 | 2 | 8 |
| [src/libcpp/cmemory.h](/src/libcpp/cmemory.h) | C++ | 9 | 0 | 4 | 13 |
| [src/libcpp/cstdio.h](/src/libcpp/cstdio.h) | C++ | 12 | 0 | 8 | 20 |
| [src/libcpp/cstdlib.h](/src/libcpp/cstdlib.h) | C++ | 43 | 0 | 11 | 54 |
| [src/libcpp/cstring.h](/src/libcpp/cstring.h) | C++ | 24 | 0 | 6 | 30 |
| [src/libcpp/ctime.h](/src/libcpp/ctime.h) | C++ | 23 | 0 | 12 | 35 |
| [src/libcpp/cxin.h](/src/libcpp/cxin.h) | C++ | 14 | 0 | 7 | 21 |
| [src/libcpp/fstream.h](/src/libcpp/fstream.h) | C++ | 39 | 0 | 22 | 61 |
| [src/libcpp/hal.h](/src/libcpp/hal.h) | C++ | 47 | 5 | 22 | 74 |
| [src/libcpp/if.h](/src/libcpp/if.h) | C++ | 8 | 0 | 4 | 12 |
| [src/libcpp/initializer_list.hpp](/src/libcpp/initializer_list.hpp) | C++ | 38 | 42 | 17 | 97 |
| [src/libcpp/iostream.h](/src/libcpp/iostream.h) | C++ | 3 | 0 | 2 | 5 |
| [src/libcpp/istream.h](/src/libcpp/istream.h) | C++ | 123 | 0 | 30 | 153 |
| [src/libcpp/iterator.h](/src/libcpp/iterator.h) | C++ | 59 | 0 | 24 | 83 |
| [src/libcpp/new.hpp](/src/libcpp/new.hpp) | C++ | 16 | 11 | 6 | 33 |
| [src/libcpp/ostream.h](/src/libcpp/ostream.h) | C++ | 203 | 0 | 62 | 265 |
| [src/libcpp/stack.h](/src/libcpp/stack.h) | C++ | 36 | 0 | 18 | 54 |
| [src/libcpp/string.h](/src/libcpp/string.h) | C++ | 108 | 22 | 47 | 177 |
| [src/libcpp/terminal_color.hpp](/src/libcpp/terminal_color.hpp) | C++ | 38 | 0 | 8 | 46 |
| [src/libcpp/time.hpp](/src/libcpp/time.hpp) | C++ | 39 | 0 | 31 | 70 |
| [src/libcpp/tuple.hpp](/src/libcpp/tuple.hpp) | C++ | 0 | 0 | 1 | 1 |
| [src/libcpp/type_traits.h](/src/libcpp/type_traits.h) | C++ | 9 | 1 | 4 | 14 |
| [src/libcpp/utility.h](/src/libcpp/utility.h) | C++ | 33 | 1 | 20 | 54 |
| [src/pit/pit.asm](/src/pit/pit.asm) | x86 and x86_64 Assembly | 11 | 0 | 5 | 16 |
| [src/pit/pit.c](/src/pit/pit.c) | C | 37 | 16 | 28 | 81 |
| [src/programming_libaries/c/algorithm.h](/src/programming_libaries/c/algorithm.h) | C++ | 3 | 0 | 3 | 6 |
| [src/programming_libaries/c/assert.h](/src/programming_libaries/c/assert.h) | C++ | 5 | 0 | 5 | 10 |
| [src/programming_libaries/c/c_libary.h](/src/programming_libaries/c/c_libary.h) | C++ | 0 | 14 | 1 | 15 |
| [src/programming_libaries/c/colors.h](/src/programming_libaries/c/colors.h) | C++ | 21 | 1 | 9 | 31 |
| [src/programming_libaries/c/hal.h](/src/programming_libaries/c/hal.h) | C++ | 22 | 0 | 7 | 29 |
| [src/programming_libaries/c/stdiox.h](/src/programming_libaries/c/stdiox.h) | C | 15 | 0 | 5 | 20 |
| [src/programming_libaries/c/stdlibx.h](/src/programming_libaries/c/stdlibx.h) | C++ | 37 | 0 | 8 | 45 |
| [src/programming_libaries/c/string.h](/src/programming_libaries/c/string.h) | C++ | 22 | 0 | 3 | 25 |
| [src/programming_libaries/c/time.h](/src/programming_libaries/c/time.h) | C++ | 5 | 0 | 3 | 8 |
| [src/syscall/syscall.asm](/src/syscall/syscall.asm) | x86 and x86_64 Assembly | 78 | 8 | 37 | 123 |
| [src/syscall/syscall.c](/src/syscall/syscall.c) | C | 89 | 27 | 35 | 151 |
| [src/syscall/syscall_test.asm](/src/syscall/syscall_test.asm) | x86 and x86_64 Assembly | 5 | 0 | 6 | 11 |
| [src/terminal/interpreter.c](/src/terminal/interpreter.c) | C | 141 | 0 | 52 | 193 |
| [src/terminal/interpreter.h](/src/terminal/interpreter.h) | C++ | 109 | 12 | 45 | 166 |
| [src/terminal/vty.c](/src/terminal/vty.c) | C++ | 83 | 3 | 43 | 129 |
| [src/terminal/vty.h](/src/terminal/vty.h) | C++ | 78 | 0 | 38 | 116 |
| [src/test.c](/src/test.c) | C | 6 | 0 | 5 | 11 |
| [src/test/cpp_test.cpp](/src/test/cpp_test.cpp) | C++ | 88 | 15 | 55 | 158 |
| [src/test/cpp_test2.cpp](/src/test/cpp_test2.cpp) | C++ | 12 | 0 | 7 | 19 |
| [src/test/testc.c](/src/test/testc.c) | C | 6 | 0 | 5 | 11 |
| [src/xagames/xagame_test.cpp](/src/xagames/xagame_test.cpp) | C++ | 16 | 0 | 8 | 24 |
| [src/xaninApps/calc.c](/src/xaninApps/calc.c) | C | 48 | 0 | 22 | 70 |
| [src/xaninApps/cpu_info.c](/src/xaninApps/cpu_info.c) | C | 121 | 0 | 42 | 163 |
| [src/xaninApps/device_info.c](/src/xaninApps/device_info.c) | C | 9 | 5 | 14 | 28 |
| [src/xaninApps/disk_load.c](/src/xaninApps/disk_load.c) | C | 9 | 0 | 3 | 12 |
| [src/xaninApps/disk_write.c](/src/xaninApps/disk_write.c) | C | 10 | 0 | 4 | 14 |
| [src/xaninApps/epilepsy.c](/src/xaninApps/epilepsy.c) | C | 19 | 0 | 9 | 28 |
| [src/xaninApps/execute_addr.c](/src/xaninApps/execute_addr.c) | C | 8 | 0 | 4 | 12 |
| [src/xaninApps/file_system/xin_info.c](/src/xaninApps/file_system/xin_info.c) | C | 24 | 0 | 7 | 31 |
| [src/xaninApps/help.c](/src/xaninApps/help.c) | C | 128 | 0 | 33 | 161 |
| [src/xaninApps/hexeditor/hexeditor.c](/src/xaninApps/hexeditor/hexeditor.c) | C | 167 | 1 | 80 | 248 |
| [src/xaninApps/hexeditor/hexeditor.h](/src/xaninApps/hexeditor/hexeditor.h) | C++ | 4 | 0 | 1 | 5 |
| [src/xaninApps/install.c](/src/xaninApps/install.c) | C | 11 | 0 | 6 | 17 |
| [src/xaninApps/keyboard_test.c](/src/xaninApps/keyboard_test.c) | C | 18 | 0 | 9 | 27 |
| [src/xaninApps/load.c](/src/xaninApps/load.c) | C | 21 | 1 | 21 | 43 |
| [src/xaninApps/loadch.c](/src/xaninApps/loadch.c) | C | 18 | 0 | 12 | 30 |
| [src/xaninApps/logo.c](/src/xaninApps/logo.c) | C | 48 | 0 | 23 | 71 |
| [src/xaninApps/mouse.c](/src/xaninApps/mouse.c) | C | 5 | 13 | 7 | 25 |
| [src/xaninApps/nic_info.c](/src/xaninApps/nic_info.c) | C | 30 | 0 | 17 | 47 |
| [src/xaninApps/pong/pong.c](/src/xaninApps/pong/pong.c) | C | 168 | 0 | 76 | 244 |
| [src/xaninApps/pong/pong.h](/src/xaninApps/pong/pong.h) | C++ | 18 | 0 | 9 | 27 |
| [src/xaninApps/reboot.asm](/src/xaninApps/reboot.asm) | x86 and x86_64 Assembly | 28 | 5 | 17 | 50 |
| [src/xaninApps/reboot.c](/src/xaninApps/reboot.c) | C | 18 | 0 | 15 | 33 |
| [src/xaninApps/register_dump.c](/src/xaninApps/register_dump.c) | C | 48 | 1 | 19 | 68 |
| [src/xaninApps/run.c](/src/xaninApps/run.c) | C | 42 | 1 | 22 | 65 |
| [src/xaninApps/run16.c](/src/xaninApps/run16.c) | C | 35 | 1 | 19 | 55 |
| [src/xaninApps/shutdown.asm](/src/xaninApps/shutdown.asm) | x86 and x86_64 Assembly | 12 | 1 | 9 | 22 |
| [src/xaninApps/shutdown.c](/src/xaninApps/shutdown.c) | C | 35 | 2 | 22 | 59 |
| [src/xaninApps/stack_frame_test.c](/src/xaninApps/stack_frame_test.c) | C | 18 | 0 | 13 | 31 |
| [src/xaninApps/syscall_test.asm](/src/xaninApps/syscall_test.asm) | x86 and x86_64 Assembly | 25 | 3 | 17 | 45 |
| [src/xaninApps/timer_test.c](/src/xaninApps/timer_test.c) | C | 25 | 0 | 13 | 38 |
| [src/xaninApps/xanin_apps_space.asm](/src/xaninApps/xanin_apps_space.asm) | x86 and x86_64 Assembly | 1 | 0 | 2 | 3 |
| [src/xaninApps/zsk.c](/src/xaninApps/zsk.c) | C | 49 | 1 | 21 | 71 |
| [src/xaninGraphics/xaninGraphics.c](/src/xaninGraphics/xaninGraphics.c) | C | 19 | 0 | 10 | 29 |
| [src/xaninGraphics/xaninGraphics.h](/src/xaninGraphics/xaninGraphics.h) | C++ | 5 | 0 | 3 | 8 |
| [src/xanin_info/info_block.c](/src/xanin_info/info_block.c) | C | 8 | 0 | 6 | 14 |
| [src/xanin_info/info_block.h](/src/xanin_info/info_block.h) | C++ | 12 | 0 | 6 | 18 |
| [src/xin_fs/cat.c](/src/xin_fs/cat.c) | C | 35 | 1 | 24 | 60 |
| [src/xin_fs/copy.c](/src/xin_fs/copy.c) | C | 14 | 0 | 10 | 24 |
| [src/xin_fs/entries_table.asm](/src/xin_fs/entries_table.asm) | x86 and x86_64 Assembly | 2 | 1 | 4 | 7 |
| [src/xin_fs/link.c](/src/xin_fs/link.c) | C | 18 | 0 | 11 | 29 |
| [src/xin_fs/list_files.c](/src/xin_fs/list_files.c) | C | 38 | 0 | 18 | 56 |
| [src/xin_fs/move.c](/src/xin_fs/move.c) | C | 21 | 0 | 11 | 32 |
| [src/xin_fs/xanin_fs_saver.c](/src/xin_fs/xanin_fs_saver.c) | C | 7 | 0 | 6 | 13 |
| [src/xin_fs/xin.c](/src/xin_fs/xin.c) | C | 463 | 9 | 196 | 668 |
| [src/xin_fs/xin.h](/src/xin_fs/xin.h) | C++ | 21 | 0 | 12 | 33 |
| [src/xin_fs/xin_entry.h](/src/xin_fs/xin_entry.h) | C++ | 38 | 0 | 17 | 55 |
| [src/xin_fs/xin_note.c](/src/xin_fs/xin_note.c) | C | 120 | 1 | 68 | 189 |
| [src/xin_fs/xin_paint.c](/src/xin_fs/xin_paint.c) | C | 129 | 4 | 59 | 192 |
| [src/xin_fs/xin_pointers.asm](/src/xin_fs/xin_pointers.asm) | x86 and x86_64 Assembly | 2 | 1 | 6 | 9 |

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)