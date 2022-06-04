# Details

Date : 2022-06-04 09:27:36

Directory /home/witut/Desktop/xaninOS/src

Total : 144 files,  8449 codes, 749 comments, 3539 blanks, all 12737 lines

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)

## Files
| filename | language | code | comment | blank | total |
| :--- | :--- | ---: | ---: | ---: | ---: |
| [src/.build.sh](/src/.build.sh) | Shell Script | 46 | 7 | 23 | 76 |
| [src/.run_linux.sh](/src/.run_linux.sh) | Shell Script | 1 | 3 | 2 | 6 |
| [src/IDT/idt.c](/src/IDT/idt.c) | C | 67 | 9 | 26 | 102 |
| [src/Makefile](/src/Makefile) | Makefile | 13 | 1 | 7 | 21 |
| [src/boot/boot.asm](/src/boot/boot.asm) | x86 and x86_64 Assembly | 89 | 55 | 48 | 192 |
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
| [src/devices/DMA/dma.c](/src/devices/DMA/dma.c) | C | 85 | 10 | 32 | 127 |
| [src/devices/DMA/dma.h](/src/devices/DMA/dma.h) | C++ | 76 | 0 | 32 | 108 |
| [src/devices/HARD_DISK/disk.asm](/src/devices/HARD_DISK/disk.asm) | x86 and x86_64 Assembly | 81 | 3 | 27 | 111 |
| [src/devices/HARD_DISK/disk.c](/src/devices/HARD_DISK/disk.c) | C | 93 | 5 | 62 | 160 |
| [src/devices/HARD_DISK/disk.h](/src/devices/HARD_DISK/disk.h) | C++ | 31 | 1 | 20 | 52 |
| [src/devices/HARD_DISK/load_kernel.asm](/src/devices/HARD_DISK/load_kernel.asm) | x86 and x86_64 Assembly | 10 | 0 | 9 | 19 |
| [src/devices/IOAPIC/ioapic.cpp](/src/devices/IOAPIC/ioapic.cpp) | C++ | 58 | 5 | 22 | 85 |
| [src/devices/IOAPIC/ioapic.h](/src/devices/IOAPIC/ioapic.h) | C++ | 20 | 0 | 11 | 31 |
| [src/devices/IOAPIC/ioapic.hpp](/src/devices/IOAPIC/ioapic.hpp) | C++ | 81 | 0 | 27 | 108 |
| [src/devices/MOUSE/mouse.asm](/src/devices/MOUSE/mouse.asm) | x86 and x86_64 Assembly | 12 | 0 | 4 | 16 |
| [src/devices/MOUSE/mouse.c](/src/devices/MOUSE/mouse.c) | C | 76 | 31 | 23 | 130 |
| [src/devices/PCI/pci.c](/src/devices/PCI/pci.c) | C | 139 | 3 | 103 | 245 |
| [src/devices/PCI/pci.h](/src/devices/PCI/pci.h) | C++ | 115 | 0 | 29 | 144 |
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
| [src/graphics/graphics.cpp](/src/graphics/graphics.cpp) | C++ | 0 | 0 | 1 | 1 |
| [src/graphics/graphics.hpp](/src/graphics/graphics.hpp) | C++ | 0 | 0 | 1 | 1 |
| [src/handlers/handlers.c](/src/handlers/handlers.c) | C | 214 | 2 | 36 | 252 |
| [src/handlers/handlers.h](/src/handlers/handlers.h) | C++ | 15 | 2 | 12 | 29 |
| [src/handlers/keyboard.asm](/src/handlers/keyboard.asm) | x86 and x86_64 Assembly | 11 | 0 | 5 | 16 |
| [src/headers/macros.h](/src/headers/macros.h) | C++ | 5 | 0 | 5 | 10 |
| [src/kernel.c](/src/kernel.c) | C | 138 | 9 | 75 | 222 |
| [src/keyboard/key_map.h](/src/keyboard/key_map.h) | C++ | 56 | 18 | 12 | 86 |
| [src/keyboard/keyboard_driver.c](/src/keyboard/keyboard_driver.c) | C | 108 | 0 | 35 | 143 |
| [src/keyboard/keyboard_init.c](/src/keyboard/keyboard_init.c) | C | 22 | 0 | 16 | 38 |
| [src/keyboard/scan_codes.h](/src/keyboard/scan_codes.h) | C++ | 45 | 0 | 0 | 45 |
| [src/lib/algorithm.h](/src/lib/algorithm.h) | C++ | 18 | 0 | 7 | 25 |
| [src/lib/alloc.h](/src/lib/alloc.h) | C++ | 95 | 10 | 40 | 145 |
| [src/lib/assert.h](/src/lib/assert.h) | C++ | 11 | 0 | 4 | 15 |
| [src/lib/clock.h](/src/lib/clock.h) | C++ | 3 | 0 | 5 | 8 |
| [src/lib/colors.h](/src/lib/colors.h) | C++ | 21 | 1 | 8 | 30 |
| [src/lib/enable_sse.asm](/src/lib/enable_sse.asm) | x86 and x86_64 Assembly | 8 | 0 | 3 | 11 |
| [src/lib/enter_real_mode.asm](/src/lib/enter_real_mode.asm) | x86 and x86_64 Assembly | 42 | 0 | 19 | 61 |
| [src/lib/exit_process.h](/src/lib/exit_process.h) | C++ | 17 | 0 | 5 | 22 |
| [src/lib/hal.h](/src/lib/hal.h) | C++ | 135 | 2 | 52 | 189 |
| [src/lib/math.h](/src/lib/math.h) | C++ | 13 | 0 | 12 | 25 |
| [src/lib/memory.h](/src/lib/memory.h) | C++ | 21 | 0 | 9 | 30 |
| [src/lib/parameter.h](/src/lib/parameter.h) | C++ | 9 | 0 | 5 | 14 |
| [src/lib/signal.h](/src/lib/signal.h) | C++ | 21 | 1 | 14 | 36 |
| [src/lib/stdiox.h](/src/lib/stdiox.h) | C | 524 | 7 | 265 | 796 |
| [src/lib/stdlibx.h](/src/lib/stdlibx.h) | C++ | 278 | 9 | 103 | 390 |
| [src/lib/string.h](/src/lib/string.h) | C++ | 257 | 30 | 134 | 421 |
| [src/lib/time.h](/src/lib/time.h) | C++ | 28 | 0 | 16 | 44 |
| [src/libcpp/algorithm.h](/src/libcpp/algorithm.h) | C++ | 51 | 0 | 25 | 76 |
| [src/libcpp/array.cpp](/src/libcpp/array.cpp) | C++ | 90 | 0 | 45 | 135 |
| [src/libcpp/chal.h](/src/libcpp/chal.h) | C++ | 17 | 0 | 4 | 21 |
| [src/libcpp/cmath.h](/src/libcpp/cmath.h) | C++ | 6 | 0 | 2 | 8 |
| [src/libcpp/cmemory.h](/src/libcpp/cmemory.h) | C++ | 9 | 0 | 4 | 13 |
| [src/libcpp/cstdio.h](/src/libcpp/cstdio.h) | C++ | 12 | 0 | 8 | 20 |
| [src/libcpp/cstdlib.h](/src/libcpp/cstdlib.h) | C++ | 32 | 0 | 7 | 39 |
| [src/libcpp/cstring.h](/src/libcpp/cstring.h) | C++ | 24 | 0 | 6 | 30 |
| [src/libcpp/ctime.h](/src/libcpp/ctime.h) | C++ | 30 | 0 | 13 | 43 |
| [src/libcpp/cxin.h](/src/libcpp/cxin.h) | C++ | 14 | 0 | 7 | 21 |
| [src/libcpp/fstream.h](/src/libcpp/fstream.h) | C++ | 39 | 0 | 22 | 61 |
| [src/libcpp/hal.h](/src/libcpp/hal.h) | C++ | 47 | 5 | 22 | 74 |
| [src/libcpp/if.h](/src/libcpp/if.h) | C++ | 8 | 0 | 4 | 12 |
| [src/libcpp/initializer_list.hpp](/src/libcpp/initializer_list.hpp) | C++ | 37 | 42 | 17 | 96 |
| [src/libcpp/iostream.h](/src/libcpp/iostream.h) | C++ | 3 | 0 | 2 | 5 |
| [src/libcpp/istream.h](/src/libcpp/istream.h) | C++ | 138 | 0 | 29 | 167 |
| [src/libcpp/new.hpp](/src/libcpp/new.hpp) | C++ | 16 | 11 | 6 | 33 |
| [src/libcpp/ostream.h](/src/libcpp/ostream.h) | C++ | 192 | 0 | 57 | 249 |
| [src/libcpp/string.h](/src/libcpp/string.h) | C++ | 75 | 0 | 30 | 105 |
| [src/libcpp/terminal_color.hpp](/src/libcpp/terminal_color.hpp) | C++ | 38 | 0 | 8 | 46 |
| [src/libcpp/time.hpp](/src/libcpp/time.hpp) | C++ | 29 | 0 | 28 | 57 |
| [src/libcpp/utility.h](/src/libcpp/utility.h) | C++ | 13 | 1 | 13 | 27 |
| [src/pit/pit.asm](/src/pit/pit.asm) | x86 and x86_64 Assembly | 11 | 0 | 5 | 16 |
| [src/pit/pit.c](/src/pit/pit.c) | C | 37 | 16 | 28 | 81 |
| [src/syscall/syscall.asm](/src/syscall/syscall.asm) | x86 and x86_64 Assembly | 82 | 4 | 37 | 123 |
| [src/syscall/syscall.c](/src/syscall/syscall.c) | C | 78 | 27 | 33 | 138 |
| [src/terminal/interpreter.c](/src/terminal/interpreter.c) | C | 190 | 0 | 63 | 253 |
| [src/terminal/interpreter.h](/src/terminal/interpreter.h) | C++ | 35 | 23 | 9 | 67 |
| [src/terminal/vty.c](/src/terminal/vty.c) | C++ | 83 | 3 | 42 | 128 |
| [src/terminal/vty.h](/src/terminal/vty.h) | C++ | 75 | 0 | 36 | 111 |
| [src/test.c](/src/test.c) | C | 6 | 0 | 5 | 11 |
| [src/test/cpp_test.cpp](/src/test/cpp_test.cpp) | C++ | 55 | 11 | 39 | 105 |
| [src/xagames/xagame_test.cpp](/src/xagames/xagame_test.cpp) | C++ | 16 | 0 | 8 | 24 |
| [src/xaninApps/calc.c](/src/xaninApps/calc.c) | C | 49 | 0 | 21 | 70 |
| [src/xaninApps/cpu_info.c](/src/xaninApps/cpu_info.c) | C | 121 | 0 | 42 | 163 |
| [src/xaninApps/device_info.c](/src/xaninApps/device_info.c) | C | 9 | 5 | 14 | 28 |
| [src/xaninApps/disk_load.c](/src/xaninApps/disk_load.c) | C | 6 | 0 | 2 | 8 |
| [src/xaninApps/disk_write.c](/src/xaninApps/disk_write.c) | C | 7 | 0 | 2 | 9 |
| [src/xaninApps/epilepsy.c](/src/xaninApps/epilepsy.c) | C | 20 | 0 | 9 | 29 |
| [src/xaninApps/execute_addr.c](/src/xaninApps/execute_addr.c) | C | 7 | 0 | 3 | 10 |
| [src/xaninApps/file_system/xin_info.c](/src/xaninApps/file_system/xin_info.c) | C | 24 | 0 | 7 | 31 |
| [src/xaninApps/help.c](/src/xaninApps/help.c) | C | 128 | 0 | 33 | 161 |
| [src/xaninApps/hexeditor/hexeditor.c](/src/xaninApps/hexeditor/hexeditor.c) | C | 167 | 1 | 80 | 248 |
| [src/xaninApps/hexeditor/hexeditor.h](/src/xaninApps/hexeditor/hexeditor.h) | C++ | 4 | 0 | 1 | 5 |
| [src/xaninApps/install.c](/src/xaninApps/install.c) | C | 11 | 0 | 6 | 17 |
| [src/xaninApps/keyboard_test.c](/src/xaninApps/keyboard_test.c) | C | 18 | 0 | 9 | 27 |
| [src/xaninApps/load.c](/src/xaninApps/load.c) | C | 26 | 0 | 21 | 47 |
| [src/xaninApps/loadch.c](/src/xaninApps/loadch.c) | C | 18 | 0 | 12 | 30 |
| [src/xaninApps/logo.c](/src/xaninApps/logo.c) | C | 48 | 0 | 23 | 71 |
| [src/xaninApps/mouse.c](/src/xaninApps/mouse.c) | C | 5 | 13 | 7 | 25 |
| [src/xaninApps/pong/pong.c](/src/xaninApps/pong/pong.c) | C | 168 | 0 | 76 | 244 |
| [src/xaninApps/pong/pong.h](/src/xaninApps/pong/pong.h) | C++ | 18 | 0 | 9 | 27 |
| [src/xaninApps/reboot.asm](/src/xaninApps/reboot.asm) | x86 and x86_64 Assembly | 28 | 5 | 17 | 50 |
| [src/xaninApps/reboot.c](/src/xaninApps/reboot.c) | C | 19 | 0 | 16 | 35 |
| [src/xaninApps/register_dump.c](/src/xaninApps/register_dump.c) | C | 47 | 0 | 17 | 64 |
| [src/xaninApps/run.c](/src/xaninApps/run.c) | C | 43 | 1 | 22 | 66 |
| [src/xaninApps/run16.c](/src/xaninApps/run16.c) | C | 36 | 1 | 19 | 56 |
| [src/xaninApps/shutdown.asm](/src/xaninApps/shutdown.asm) | x86 and x86_64 Assembly | 21 | 1 | 13 | 35 |
| [src/xaninApps/shutdown.c](/src/xaninApps/shutdown.c) | C | 34 | 1 | 22 | 57 |
| [src/xaninApps/stack_frame_test.c](/src/xaninApps/stack_frame_test.c) | C | 18 | 0 | 13 | 31 |
| [src/xaninApps/syscall_test.asm](/src/xaninApps/syscall_test.asm) | x86 and x86_64 Assembly | 23 | 3 | 17 | 43 |
| [src/xaninApps/timer_test.c](/src/xaninApps/timer_test.c) | C | 25 | 0 | 13 | 38 |
| [src/xaninApps/xanin_apps_space.asm](/src/xaninApps/xanin_apps_space.asm) | x86 and x86_64 Assembly | 1 | 0 | 2 | 3 |
| [src/xaninApps/zsk.c](/src/xaninApps/zsk.c) | C | 49 | 1 | 21 | 71 |
| [src/xaninGraphics/xaninGraphics.c](/src/xaninGraphics/xaninGraphics.c) | C | 19 | 0 | 10 | 29 |
| [src/xaninGraphics/xaninGraphics.h](/src/xaninGraphics/xaninGraphics.h) | C++ | 5 | 0 | 3 | 8 |
| [src/xin_fs/entries_table.asm](/src/xin_fs/entries_table.asm) | x86 and x86_64 Assembly | 2 | 1 | 4 | 7 |
| [src/xin_fs/list_files.c](/src/xin_fs/list_files.c) | C | 13 | 0 | 7 | 20 |
| [src/xin_fs/xanin_fs_saver.c](/src/xin_fs/xanin_fs_saver.c) | C | 7 | 0 | 6 | 13 |
| [src/xin_fs/xin.c](/src/xin_fs/xin.c) | C | 372 | 11 | 158 | 541 |
| [src/xin_fs/xin.h](/src/xin_fs/xin.h) | C++ | 21 | 0 | 12 | 33 |
| [src/xin_fs/xin_entry.h](/src/xin_fs/xin_entry.h) | C++ | 38 | 0 | 17 | 55 |
| [src/xin_fs/xin_note.c](/src/xin_fs/xin_note.c) | C | 121 | 1 | 68 | 190 |
| [src/xin_fs/xin_paint.c](/src/xin_fs/xin_paint.c) | C | 130 | 0 | 58 | 188 |
| [src/xin_fs/xin_pointers.asm](/src/xin_fs/xin_pointers.asm) | x86 and x86_64 Assembly | 2 | 1 | 6 | 9 |

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)