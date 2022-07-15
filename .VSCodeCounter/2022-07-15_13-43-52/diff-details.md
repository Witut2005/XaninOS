# Diff Details

Date : 2022-07-15 13:43:52

Directory /home/witut/Desktop/xaninOS

Total : 117 files,  1371 codes, 59 comments, 665 blanks, all 2095 lines

[Summary](results.md) / [Details](details.md) / [Diff Summary](diff.md) / Diff Details

## Files
| filename | language | code | comment | blank | total |
| :--- | :--- | ---: | ---: | ---: | ---: |
| [README.md](/README.md) | Markdown | 39 | 0 | 16 | 55 |
| [documentation/TODO.md](/documentation/TODO.md) | Markdown | 7 | 0 | 5 | 12 |
| [documentation/disk_image.md](/documentation/disk_image.md) | Markdown | 10 | 0 | 4 | 14 |
| [documentation/xanin_coding_style.md](/documentation/xanin_coding_style.md) | Markdown | 39 | 0 | 17 | 56 |
| [programming_libaries/xanin_asm.inc](/programming_libaries/xanin_asm.inc) | x86 and x86_64 Assembly | 6 | 0 | 3 | 9 |
| [src/.build.sh](/src/.build.sh) | Shell Script | -46 | -7 | -23 | -76 |
| [src/IDT/idt.c](/src/IDT/idt.c) | C | 7 | 1 | 2 | 10 |
| [src/Makefile](/src/Makefile) | Makefile | 60 | -1 | 23 | 82 |
| [src/app_config.h](/src/app_config.h) | C++ | 33 | 1 | 3 | 37 |
| [src/boot/boot.asm](/src/boot/boot.asm) | x86 and x86_64 Assembly | 2 | 0 | 0 | 2 |
| [src/devices/COM/com.cpp](/src/devices/COM/com.cpp) | C++ | 61 | 0 | 24 | 85 |
| [src/devices/COM/com.hpp](/src/devices/COM/com.hpp) | C++ | 21 | 0 | 14 | 35 |
| [src/devices/IOAPIC/ioapic.cpp](/src/devices/IOAPIC/ioapic.cpp) | C++ | 0 | -5 | -2 | -7 |
| [src/devices/IOAPIC/ioapic.h](/src/devices/IOAPIC/ioapic.h) | C++ | 1 | 0 | 0 | 1 |
| [src/devices/NIC/8254x.cpp](/src/devices/NIC/8254x.cpp) | C++ | 88 | 17 | 53 | 158 |
| [src/devices/NIC/8254x.hpp](/src/devices/NIC/8254x.hpp) | C++ | 126 | 1 | 38 | 165 |
| [src/devices/PCI/pci.c](/src/devices/PCI/pci.c) | C | 82 | 0 | 57 | 139 |
| [src/devices/PCI/pci.h](/src/devices/PCI/pci.h) | C++ | 10 | 0 | 13 | 23 |
| [src/graphics/graphics.cpp](/src/graphics/graphics.cpp) | C++ | 0 | 0 | -1 | -1 |
| [src/graphics/graphics.hpp](/src/graphics/graphics.hpp) | C++ | 0 | 0 | -1 | -1 |
| [src/gyn_cl/gyn.c](/src/gyn_cl/gyn.c) | C | 41 | 4 | 20 | 65 |
| [src/gyn_cl/gyn.h](/src/gyn_cl/gyn.h) | C++ | 8 | 0 | 6 | 14 |
| [src/handlers/handlers.c](/src/handlers/handlers.c) | C | 1 | 2 | 3 | 6 |
| [src/handlers/handlers.h](/src/handlers/handlers.h) | C++ | 1 | 0 | 1 | 2 |
| [src/kernel.c](/src/kernel.c) | C | 44 | 16 | 33 | 93 |
| [src/lib/algorithm.h](/src/lib/algorithm.h) | C++ | -18 | 0 | -7 | -25 |
| [src/lib/alloc.h](/src/lib/alloc.h) | C++ | -95 | -10 | -40 | -145 |
| [src/lib/assert.h](/src/lib/assert.h) | C++ | -11 | 0 | -4 | -15 |
| [src/lib/clock.h](/src/lib/clock.h) | C++ | -3 | 0 | -5 | -8 |
| [src/lib/colors.h](/src/lib/colors.h) | C++ | -21 | -1 | -8 | -30 |
| [src/lib/enable_sse.asm](/src/lib/enable_sse.asm) | x86 and x86_64 Assembly | -8 | 0 | -3 | -11 |
| [src/lib/enter_real_mode.asm](/src/lib/enter_real_mode.asm) | x86 and x86_64 Assembly | -42 | 0 | -19 | -61 |
| [src/lib/exit_process.h](/src/lib/exit_process.h) | C++ | -17 | 0 | -5 | -22 |
| [src/lib/hal.h](/src/lib/hal.h) | C++ | -135 | -2 | -52 | -189 |
| [src/lib/math.h](/src/lib/math.h) | C++ | -13 | 0 | -12 | -25 |
| [src/lib/memory.h](/src/lib/memory.h) | C++ | -21 | 0 | -9 | -30 |
| [src/lib/parameter.h](/src/lib/parameter.h) | C++ | -9 | 0 | -5 | -14 |
| [src/lib/signal.h](/src/lib/signal.h) | C++ | -21 | -1 | -14 | -36 |
| [src/lib/stdiox.h](/src/lib/stdiox.h) | C | -524 | -7 | -265 | -796 |
| [src/lib/stdlibx.h](/src/lib/stdlibx.h) | C++ | -278 | -9 | -103 | -390 |
| [src/lib/string.h](/src/lib/string.h) | C++ | -257 | -30 | -134 | -421 |
| [src/lib/time.h](/src/lib/time.h) | C++ | -28 | 0 | -16 | -44 |
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
| [src/libcpp/algorithm.h](/src/libcpp/algorithm.h) | C++ | 29 | 0 | 9 | 38 |
| [src/libcpp/array.cpp](/src/libcpp/array.cpp) | C++ | -90 | 0 | -45 | -135 |
| [src/libcpp/array.h](/src/libcpp/array.h) | C++ | 91 | 0 | 46 | 137 |
| [src/libcpp/box.h](/src/libcpp/box.h) | C++ | 22 | 0 | 11 | 33 |
| [src/libcpp/cstdlib.h](/src/libcpp/cstdlib.h) | C++ | 11 | 0 | 4 | 15 |
| [src/libcpp/ctime.h](/src/libcpp/ctime.h) | C++ | -7 | 0 | -1 | -8 |
| [src/libcpp/initializer_list.hpp](/src/libcpp/initializer_list.hpp) | C++ | 1 | 0 | 0 | 1 |
| [src/libcpp/istream.h](/src/libcpp/istream.h) | C++ | -15 | 0 | 1 | -14 |
| [src/libcpp/iterator.h](/src/libcpp/iterator.h) | C++ | 59 | 0 | 24 | 83 |
| [src/libcpp/ostream.h](/src/libcpp/ostream.h) | C++ | 11 | 0 | 5 | 16 |
| [src/libcpp/stack.h](/src/libcpp/stack.h) | C++ | 36 | 0 | 18 | 54 |
| [src/libcpp/string.h](/src/libcpp/string.h) | C++ | 33 | 22 | 17 | 72 |
| [src/libcpp/time.hpp](/src/libcpp/time.hpp) | C++ | 10 | 0 | 3 | 13 |
| [src/libcpp/tuple.hpp](/src/libcpp/tuple.hpp) | C++ | 0 | 0 | 1 | 1 |
| [src/libcpp/type_traits.h](/src/libcpp/type_traits.h) | C++ | 9 | 1 | 4 | 14 |
| [src/libcpp/utility.h](/src/libcpp/utility.h) | C++ | 20 | 0 | 7 | 27 |
| [src/programming_libaries/c/algorithm.h](/src/programming_libaries/c/algorithm.h) | C++ | 3 | 0 | 3 | 6 |
| [src/programming_libaries/c/assert.h](/src/programming_libaries/c/assert.h) | C++ | 5 | 0 | 5 | 10 |
| [src/programming_libaries/c/c_libary.h](/src/programming_libaries/c/c_libary.h) | C++ | 0 | 14 | 1 | 15 |
| [src/programming_libaries/c/colors.h](/src/programming_libaries/c/colors.h) | C++ | 21 | 1 | 9 | 31 |
| [src/programming_libaries/c/hal.h](/src/programming_libaries/c/hal.h) | C++ | 22 | 0 | 7 | 29 |
| [src/programming_libaries/c/stdiox.h](/src/programming_libaries/c/stdiox.h) | C | 15 | 0 | 5 | 20 |
| [src/programming_libaries/c/stdlibx.h](/src/programming_libaries/c/stdlibx.h) | C++ | 37 | 0 | 8 | 45 |
| [src/programming_libaries/c/string.h](/src/programming_libaries/c/string.h) | C++ | 22 | 0 | 3 | 25 |
| [src/programming_libaries/c/time.h](/src/programming_libaries/c/time.h) | C++ | 5 | 0 | 3 | 8 |
| [src/syscall/syscall.asm](/src/syscall/syscall.asm) | x86 and x86_64 Assembly | -4 | 4 | 0 | 0 |
| [src/syscall/syscall.c](/src/syscall/syscall.c) | C | 11 | 0 | 2 | 13 |
| [src/syscall/syscall_test.asm](/src/syscall/syscall_test.asm) | x86 and x86_64 Assembly | 5 | 0 | 6 | 11 |
| [src/terminal/interpreter.c](/src/terminal/interpreter.c) | C | -49 | 0 | -11 | -60 |
| [src/terminal/interpreter.h](/src/terminal/interpreter.h) | C++ | 74 | -11 | 36 | 99 |
| [src/terminal/vty.c](/src/terminal/vty.c) | C++ | 0 | 0 | 1 | 1 |
| [src/terminal/vty.h](/src/terminal/vty.h) | C++ | 3 | 0 | 2 | 5 |
| [src/test/cpp_test.cpp](/src/test/cpp_test.cpp) | C++ | 33 | 4 | 16 | 53 |
| [src/test/cpp_test2.cpp](/src/test/cpp_test2.cpp) | C++ | 12 | 0 | 7 | 19 |
| [src/test/testc.c](/src/test/testc.c) | C | 6 | 0 | 5 | 11 |
| [src/xaninApps/calc.c](/src/xaninApps/calc.c) | C | -1 | 0 | 1 | 0 |
| [src/xaninApps/disk_load.c](/src/xaninApps/disk_load.c) | C | 3 | 0 | 1 | 4 |
| [src/xaninApps/disk_write.c](/src/xaninApps/disk_write.c) | C | 3 | 0 | 2 | 5 |
| [src/xaninApps/epilepsy.c](/src/xaninApps/epilepsy.c) | C | -1 | 0 | 0 | -1 |
| [src/xaninApps/execute_addr.c](/src/xaninApps/execute_addr.c) | C | 1 | 0 | 1 | 2 |
| [src/xaninApps/load.c](/src/xaninApps/load.c) | C | -5 | 1 | 0 | -4 |
| [src/xaninApps/nic_info.c](/src/xaninApps/nic_info.c) | C | 30 | 0 | 17 | 47 |
| [src/xaninApps/reboot.c](/src/xaninApps/reboot.c) | C | -1 | 0 | -1 | -2 |
| [src/xaninApps/register_dump.c](/src/xaninApps/register_dump.c) | C | 1 | 1 | 2 | 4 |
| [src/xaninApps/run.c](/src/xaninApps/run.c) | C | -1 | 0 | 0 | -1 |
| [src/xaninApps/run16.c](/src/xaninApps/run16.c) | C | -1 | 0 | 0 | -1 |
| [src/xaninApps/shutdown.asm](/src/xaninApps/shutdown.asm) | x86 and x86_64 Assembly | -9 | 0 | -4 | -13 |
| [src/xaninApps/shutdown.c](/src/xaninApps/shutdown.c) | C | 1 | 1 | 0 | 2 |
| [src/xaninApps/syscall_test.asm](/src/xaninApps/syscall_test.asm) | x86 and x86_64 Assembly | 2 | 0 | 0 | 2 |
| [src/xanin_info/info_block.c](/src/xanin_info/info_block.c) | C | 8 | 0 | 6 | 14 |
| [src/xanin_info/info_block.h](/src/xanin_info/info_block.h) | C++ | 12 | 0 | 6 | 18 |
| [src/xin_fs/cat.c](/src/xin_fs/cat.c) | C | 35 | 1 | 24 | 60 |
| [src/xin_fs/copy.c](/src/xin_fs/copy.c) | C | 14 | 0 | 10 | 24 |
| [src/xin_fs/link.c](/src/xin_fs/link.c) | C | 18 | 0 | 11 | 29 |
| [src/xin_fs/list_files.c](/src/xin_fs/list_files.c) | C | 25 | 0 | 11 | 36 |
| [src/xin_fs/move.c](/src/xin_fs/move.c) | C | 21 | 0 | 11 | 32 |
| [src/xin_fs/xin.c](/src/xin_fs/xin.c) | C | 91 | -2 | 38 | 127 |
| [src/xin_fs/xin_note.c](/src/xin_fs/xin_note.c) | C | -1 | 0 | 0 | -1 |
| [src/xin_fs/xin_paint.c](/src/xin_fs/xin_paint.c) | C | -1 | 4 | 1 | 4 |

[Summary](results.md) / [Details](details.md) / [Diff Summary](diff.md) / Diff Details