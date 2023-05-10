

C	= i386-elf-gcc
CC 	= i386-elf-g++

XANIN_PATH = /home/witut/Desktop/xaninOS/src/
XANIN_BINARIES = /home/witut/Desktop/xaninOS/bin

# C_OPTIONS  = -O0 -masm=intel -Wno-discarded-qualifiers -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-int-conversion -Wno-unused-function -Wno-div-by-zero -Wno-address-of-packed-member -I $(XANIN_PATH) -c
C_COMPILE  = -O0 -masm=intel -Wno-discarded-qualifiers -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-int-conversion -Wno-unused-function -Wno-div-by-zero -Wno-address-of-packed-member -I $(XANIN_PATH) 
CC_OPTIONS =  -O0 -fno-exceptions -lstdc++ -masm=intel -std=c++17 -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -Wno-write-strings -fno-rtti -fconcepts-ts -I $(XANIN_PATH) -c
C_OPTIONS  =  -O0 -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -Werror=return-type -I $(XANIN_PATH) -c

ASM	= nasm
APPS = ./programs

HANDLERS = ./sys/interrupts/handlers

build:

	@echo "\n///////////////////////////////////////////////"
	@echo "\033[0;32mBOOT MODULE COMPILATION PHARSE"
	@echo "\033[0m"

	@echo [ASM] boot.asm
	@$(ASM) -fbin ./boot/boot.asm

	@echo [ASM] kernelLoader.asm
	@$(ASM) -fbin ./boot/kernelLoader.asm

	@echo "\n///////////////////////////////////////////////"
	

	@echo "\033[0;32mFILE SYSTEM MODULE COMPILATION PHARSE"
	@echo "\033[0m"

	@echo [ASM] xin_pointers.asm
	@$(ASM) -fbin ./fs/xin_pointers.asm

	@echo [ASM] entries_table.asm
	@$(ASM) -fbin ./fs/entries_table.asm

	@echo [C] xin.c
	@$(C) $(C_OPTIONS) ./fs/xin.c -o ./fs/xin.o

	@echo [C] xin_syscalls.c
	@$(C) $(C_OPTIONS) ./fs/xin_syscalls.c -o ./fs/xin_syscalls.o

	@echo [ASM] xin_extended_table.asm
	@$(ASM) ./fs/xin_extended_table.asm -o ./fs/xin_extended_table
	
	@echo [ASM] xanin_apps_space.asm
	@$(ASM) -fbin $(APPS)/xanin_apps_space.asm -o $(APPS)/xanin_apps_space

	@echo "\n///////////////////////////////////////////////"

	@echo "\033[0;32mDEVICE DRIVES MODULE COMPILATION PHARSE"
	@echo "\033[0m"

	@echo [ASM] keyboard.asm
	@$(ASM) -f elf32 ./sys/devices/keyboard/keyboard.asm -o ./sys/devices/keyboard/keyboard.o

	@echo [ASM] pit.asm
	@$(ASM) -f elf32 ./sys/devices/pit/pit.asm -o ./sys/devices/pit/pit_entry.o
	
	@echo [C] pc_speaker.c
	@$(C) $(C_OPTIONS) ./sys/devices/pcspk/pc_speaker.c -o ./sys/devices/pcspk/pc_speaker.o

	@echo [C] mouse.c
	@$(C) $(C_OPTIONS) ./sys/devices/mouse/mouse.c -o ./sys/devices/mouse/mouse.o

	@echo [ASM] mouse.asm
	@$(ASM) -f elf32 ./sys/devices/mouse/mouse.asm -o ./sys/devices/mouse/mouse_init.o

	@echo [CC] com.cpp
	@$(CC) $(CC_OPTIONS) ./sys/devices/com/com.cpp -o ./sys/devices/com/ComPort.o

	@echo [CC] ioapic.cpp
	@$(CC) $(CC_OPTIONS) ./sys/devices/ioapic/ioapic.cpp -o ./sys/devices/ioapic/ioapic.o

	@echo [ASM] 8254x INTERRUPT ENTRY
	@$(ASM) -f elf32 ./sys/devices/nic/8254x_int_handler.asm -o ./sys/devices/nic/8254x_int_handler.o

	@echo [CC] 8254x.cpp
	@$(CC) $(CC_OPTIONS) ./sys/devices/nic/8254x.cpp -o ./sys/devices/nic/8254x.o

	@echo "\n///////////////////////////////////////////////"

	
	@echo "\033[0;32mSYSCALL MODULE COMPILATION PHARSE"
	@echo "\033[0m"

	@echo [ASM] syscall_entry.asm
	@$(ASM) -f elf32 ./sys/call/syscall_entry.asm -o ./sys/call/syscall_entry.o

	@echo [ASM] xanin_sys_entry.asm
	@$(ASM) -f elf32 ./sys/call/xanin_sys_entry.asm -o ./sys/call/xanin_sys_entry.o

	@echo [C] xanin_sys.c
	@$(C) $(C_OPTIONS) ./sys/call/xanin_sys/xanin_sys.c -o ./sys/call/xanin_sys/xanin_sys.o

	@echo "\n///////////////////////////////////////////////"

	@echo "\033[0;32mINTERRUPT MODULE COMPILATION PHARSE"
	@echo "\033[0m"

	@echo [C] idt.c
	@$(C) $(C_OPTIONS) ./sys/interrupts/idt/idt.c -o ./sys/interrupts/idt/idt.o

	@echo [ASM] divide_by_zero_exception
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/divide_by_zero_exception.asm -o ./sys/interrupts/handlers/entries/divide_by_zero_exception.o

	@echo [ASM] debug_exception
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/debug_exception.asm -o ./sys/interrupts/handlers/entries/debug_exception.o

	@echo [ASM] nmi_exception
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/nmi_exception.asm -o ./sys/interrupts/handlers/entries/nmi_exception.o

	@echo [ASM] breakpoint_exception 
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/breakpoint_exception.asm -o ./sys/interrupts/handlers/entries/breakpoint_exception.o

	@echo [ASM] overflow_exception 
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/overflow_exception.asm -o ./sys/interrupts/handlers/entries/overflow_exception.o

	@echo [ASM] bound_range_exceeded_exception
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/bound_range_exceeded_exception.asm -o ./sys/interrupts/handlers/entries/bound_range_exceeded_exception.o

	@echo [ASM] invalid_opcode_exception 
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/invalid_opcode_exception.asm -o ./sys/interrupts/handlers/entries/invalid_opcode_exception.o

	@echo [ASM] device_not_availble_exception
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/device_not_availble_exception.asm -o ./sys/interrupts/handlers/entries/device_not_availble_exception.o

	@echo [ASM] double_fault_exception 
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/double_fault_exception.asm -o ./sys/interrupts/handlers/entries/double_fault_exception.o

	@echo [ASM] coprocessor_segment_overrun_exception
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/coprocessor_segment_overrun_exception.asm -o ./sys/interrupts/handlers/entries/coprocessor_segment_overrun_exception.o

	@echo [ASM] invalid_tss_exception
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/invalid_tss_exception.asm -o ./sys/interrupts/handlers/entries/invalid_tss_exception.o

	@echo [ASM] segment_not_present_exception
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/segment_not_present_exception.asm -o ./sys/interrupts/handlers/entries/segment_not_present_exception.o

	@echo [ASM] stack_fault_exception
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/stack_fault_exception.asm -o ./sys/interrupts/handlers/entries/stack_fault_exception.o

	@echo [ASM] general_protection_exception 
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/general_protection_exception.asm -o ./sys/interrupts/handlers/entries/general_protection_exception.o

	@echo [ASM] page_fault_exception
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/page_fault_exception.asm -o ./sys/interrupts/handlers/entries/page_fault_exception.o

	@echo [ASM] x86_fpu_floating_point_exception 
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/x86_fpu_floating_point_exception.asm -o ./sys/interrupts/handlers/entries/x86_fpu_floating_point_exception.o

	@echo [ASM] aligment_check_exception
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/aligment_check_exception.asm -o ./sys/interrupts/handlers/entries/aligment_check_exception.o

	@echo [ASM] machine_check_exception
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/machine_check_exception.asm -o ./sys/interrupts/handlers/entries/machine_check_exception.o

	@echo [ASM] simd_floating_point_exception
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/simd_floating_point_exception.asm -o ./sys/interrupts/handlers/entries/simd_floating_point_exception.o

	@echo [ASM] virtualization_exception
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/virtualization_exception.asm -o ./sys/interrupts/handlers/entries/virtualization_exception.o

	@echo [ASM] coprocessor_segment_overrun_exception
	@$(ASM) -f elf32 ./sys/interrupts/handlers/entries/coprocessor_segment_overrun_exception.asm -o ./sys/interrupts/handlers/entries/coprocessor_segment_overrun_exception.o

	@echo "\n///////////////////////////////////////////////"

	@echo "\033[0;32mDEVICE DRIVERS COMPILATION PHARSE"
	@echo "\033[0m"
	
	@echo [CC] apic.cpp
	@$(CC) $(CC_OPTIONS) ./sys/devices/apic/apic.cpp -o ./sys/devices/apic/apic.o

	@echo [C] vga.c
	@$(C) $(C_OPTIONS) ./sys/devices/vga/vga.c -o ./sys/devices/vga/vga.o

	@echo [C] usb.c
	@$(C) $(C_OPTIONS) ./sys/devices/usb/usb.c -o ./sys/devices/usb/usb.o

	@echo [C] ACPI.c
	@$(C) $(C_OPTIONS) ./sys/devices/acpi/ACPI.c -o ./sys/devices/acpi/ACPI.o

	@echo [C] disk.c
	@$(C) $(C_OPTIONS) ./sys/devices/hda/disk.c -o ./sys/devices/hda/disk.o
	
	@echo [CC] ne2000.cpp
	@$(CC) $(CC_OPTIONS) ./sys/devices/nic/ne2000.cpp -o ./sys/devices/nic/ne2000.o
	
	@echo [C] pit.c
	@$(C) $(C_OPTIONS) ./sys/devices/pit/pit.c -o ./sys/devices/pit/pit.o

	@echo [C] key_map.c
	@$(C) $(C_OPTIONS) ./sys/devices/keyboard/key_map.c -o ./sys/devices/keyboard/key_map.o

	@echo "\n///////////////////////////////////////////////"

	@echo "\033[0;32mNETWORK PROTOCOLS MODULE COMPILATION PHARSE"
	@echo "\033[0m"

	@echo [CC] EthernetFrameInterface
	@$(CC) $(CC_OPTIONS) ./sys/net/network_protocols/ethernet_frame/ethernet_frame.cpp -o ./sys/net/network_protocols/ethernet_frame/ethernet_frame.o
	
	@echo [CC] icmp.cpp
	@$(CC) $(CC_OPTIONS) ./sys/net/network_protocols/icmp/icmp.cpp -o ./sys/net/network_protocols/icmp/icmp.o
	
	@echo [CC] arp.cpp
	@$(CC) $(CC_OPTIONS) ./sys/net/network_protocols/arp/arp.cpp -o ./sys/net/network_protocols/arp/arp.o

	@echo [CC] ip.cpp
	@$(CC) $(CC_OPTIONS) ./sys/net/network_protocols/internet_protocol/ipv4/ip.cpp -o ./sys/net/network_protocols/internet_protocol/ipv4/ip.o

	@echo [CC] udp.cpp
	@$(CC) $(CC_OPTIONS) ./sys/net/network_protocols/udp/udp.cpp -o ./sys/net/network_protocols/udp/udp.o
	
	@echo [CC] dhcp.cpp
	@$(CC) $(CC_OPTIONS) ./sys/net/network_protocols/dhcp/dhcp.cpp -o ./sys/net/network_protocols/dhcp/dhcp.o


	@echo "\n///////////////////////////////////////////////"

	@echo "\033[0;32mCAMILLA OBJECTS NETWORK INTERFACE COMPILATION PHARSE"
	@echo "\033[0m"

	@echo [ASM] netapi_interrupt
	@$(ASM) -f elf32 ./sys/net/netapi/netapi_interrupt.asm -o ./sys/net/netapi/netapi_interrupt.o
	
	@echo [CC] network_device.cpp
	@$(CC) $(CC_OPTIONS) ./sys/net/netapi/network_device.cpp -o ./sys/net/netapi/network_devicecpp.o

	@echo [CC] loopback.cpp
	@$(CC) $(CC_OPTIONS) ./sys/net/netapi/loopback/loopback.cpp -o ./sys/net/netapi/loopback/loopback.o

	@echo "\n///////////////////////////////////////////////"

	@echo "\033[0;32mCAMILLA OBJECTS NETWORK INTERFACE COMPILATION PHARSE"
	@echo "\033[0m"

	@echo [CPP] ip.cpp
	@$(CC) $(CC_OPTIONS) ./sys/net/netapi/objects/ip.cpp -o ./sys/net/netapi/objects/ip.o 

	@echo [C] response.c
	@$(CC) $(CC_OPTIONS) ./sys/net/netapi/objects/response.c -o ./sys/net/netapi/objects/response.o
	
	@echo [CC] mac.cpp
	@$(CC) $(CC_OPTIONS) ./sys/net/netapi/objects/mac.cpp -o ./sys/net/netapi/objects/mac.o 

	@echo "\n///////////////////////////////////////////////"

	@echo "\033[0;32mKERNEL MODULES COMPILATION PHARSE"
	@echo "\033[0m"
	
	@echo [C] vty.c
	@$(C) $(C_OPTIONS) ./sys/terminal/vty/vty.c -o ./sys/terminal/vty/vty.o
	
	@echo [C] terminal.c
	@$(C) $(C_OPTIONS) ./sys/terminal/interface/terminal.c -o ./sys/terminal/interface/terminal.o

	@echo [C] alloc.c
	@$(C) $(C_OPTIONS) ./sys/pmmngr/alloc.c -o ./sys/pmmngr/alloc.o

	@echo [C] input.c
	@$(C) $(C_OPTIONS) ./sys/input/input.c -o ./sys/input/input.o

	@echo "\n///////////////////////////////////////////////"

	@echo "\033[0;32mXANIN SPECIFIC SYSCALLS MODULE COMPILATION PHARSE"
	@echo "\033[0m"

	@echo [C] alloc.c
	@$(C) $(C_OPTIONS) ./sys/call/xanin_sys/pmmngr/alloc.c -o ./sys/call/xanin_sys/pmmngr/alloc.o

	@echo "\n///////////////////////////////////////////////"

	@echo "\033[0;32mREGULAR C LIBRARY COMPILATION PHARSE"
	@echo "\033[0m"

	@ echo [ASM] registers_values_get.asm
	@ $(ASM) -f elf32 ./lib/libc/registers_values_get.asm -o ./lib/libc/registers_values_get.o
	
	@echo [ASM] enter_real_mode.asm
	@$(ASM) -fbin ./lib/libc/enter_real_mode.asm -o ./lib/libc/enter_real_mode
	
	@echo [ASM] fast_return_to_32_mode.asm 
	@$(ASM) -fbin ./lib/libc/fast_return_to_32_mode.asm -o ./lib/libc/fast_return_to_32_mode

	@echo [C] hal.c
	@$(C) $(C_OPTIONS) ./lib/libc/hal.c -o ./lib/libc/hal.o

	@echo [C] math.c
	@$(C) $(C_OPTIONS) ./lib/libc/math.c -o ./lib/libc/math.o
	
	@echo [ASM] instruction_pointer.asm
	@$(ASM) -f elf32 ./lib/libc/instruction_pointer.asm -o ./lib/libc/instruction_pointer.o

	@echo [ASM] real_mode_fswitch.asm
	@$(ASM) -fbin ./lib/libc/real_mode_fswitch.asm -o ./lib/libc/real_mode_fswitch_asm

	@echo [ASM] xanin_state.asm
	@$(ASM) -f elf32 ./lib/libc/xanin_state.asm -o ./lib/libc/xanin_state.o

	@echo [C] memory.c
	@$(C) $(C_OPTIONS) ./lib/libc/memory.c -o ./lib/libc/memory.o

	@echo [C] stdiox.c
	@$(C) $(C_OPTIONS) ./lib/libc/stdiox.c -o ./lib/libc/stdiox.o

	@echo [C] stdlibx.c
	@$(C) $(C_OPTIONS) ./lib/libc/stdlibx.c -o ./lib/libc/stdlibx.o

	@echo [C] signal.c
	@$(C) $(C_OPTIONS) ./lib/libc/signal.c -o ./lib/libc/signal.o

	@echo [C] tui.c
	@$(C) $(C_OPTIONS) ./lib/tui/tui.c -o ./lib/tui/tui.o

	@echo [C] data_structures.c
	@$(C) $(C_OPTIONS) ./lib/libc/data_structures.c -o ./lib/libc/data_structures.o

	@echo [C] system.c
	@$(C) $(C_OPTIONS) ./lib/libc/system.c -o ./lib/libc/system.o

	@echo [C] string.c
	@$(C) $(C_OPTIONS) ./lib/libc/string.c -o ./lib/libc/string.o

	@echo [CC] algorithm.cpp
	@$(CC) $(CC_OPTIONS) ./lib/libcpp/algorithm.cpp -o ./lib/libcpp/algorithm.o

	@echo [CC] command_parser.cpp
	@$(CC) $(CC_OPTIONS) ./lib/libcpp/command_parser.cpp -o ./lib/libcpp/command_parser.o

	@echo [CC] icxabi.cpp
	@$(CC) $(CC_OPTIONS) ./lib/libcpp/icxxabi.cpp -o ./lib/libcpp/icxxabi.o

	@echo [C] algorithm.c
	@$(C) $(C_OPTIONS) ./lib/libc/algorithm.c -o ./lib/libc/algorithm.o

	@echo [C] syslog.c
	@$(C) $(C_OPTIONS) ./sys/log/syslog.c -o ./sys/log/syslog.o

	@echo [C] time.c
	@$(C) $(C_OPTIONS) ./lib/libc/time.c -o ./lib/libc/time.o

	@echo [C] process.c
	@$(C) $(C_OPTIONS) ./lib/libc/process.c -o ./lib/libc/process.o
	
	@echo [C] xaninGraphics 
	@$(C) $(C_OPTIONS) ./lib/xaninGraphics/xaninGraphics.c -o ./lib/xaninGraphics/xaninGraphics.o
	
	@echo [C] real_mode_fswitch.c
	@$(C) $(C_OPTIONS) ./lib/libc/real_mode_fswitch.c -o ./lib/libc/real_mode_fswitch.o

	@echo [ASM] CRT0
	@nasm -f elf32 ./lib/libc/crt0.asm -o ./lib/libc/crt0.o

	@echo "\n///////////////////////////////////////////////"

	@echo "\033[0;32mREGULAR CPP LIBRARY COMPILATION PHARSE"
	@echo "\033[0m"

	@echo [CC] regex.cpp
	@$(CC) $(CC_OPTIONS) ./lib/libcpp/regex.cpp -o ./lib/libcpp/regex.o 

	@# @echo [CC] bytes.cpp
	@# @$(CC) $(CC_OPTIONS) ./lib/libcpp/bytes.cpp -o ./lib/libcpp/bytes.o

	@# @echo [CC] tuple.hpp
	@# @$(CC) $(CC_OPTIONS) ./lib/libcpp/tuple.hpp -o ./lib/libcpp/tuple.o

	@echo [CC] endian.cpp
	@$(CC) $(CC_OPTIONS) ./lib/libcpp/endian.cpp -o ./lib/libcpp/endian.o

	@echo "\n///////////////////////////////////////////////"

	@echo "\033[0;32mXANIN GRAPHICS LIBRARIES COMPILATION PHARSE"
	@echo "\033[0m"
	
	@echo [C] xgl.c
	@$(C) $(C_OPTIONS) ./lib/xgl/xgl.c -o ./lib/xgl/xgl.o

	@echo [CC] game_engine.cpp
	@$(CC) $(CC_OPTIONS) ./lib/game_engine/xagame.cpp -o ./lib/game_engine/xagame.o

	@echo "\n///////////////////////////////////////////////"
	

	@echo "\033[0;32mXANIN BUILT-IN PROGRAMS COMPILATION PHARSE"
	@echo "\033[0m"

	@echo [C] edit.c
	@$(C) $(C_OPTIONS) ./programs/edit.c -o ./programs/edit.o

	@echo [C] register_dump.c
	@$(C) $(C_OPTIONS) ./programs/register_dump.c -o ./programs/register_dump.o

	@echo [CC] xagame_test.cpp
	@$(CC) $(CC_OPTIONS) ./programs/xagames/xagame_test.cpp -o ./programs/xagames/xagame_test.o

	@echo [CC] tetris.cpp
	@$(CC) $(CC_OPTIONS) ./programs/xagames/tetris.cpp -o ./programs/xagames/tetris.o

	@echo [CC] screenshot.cpp
	@$(CC) $(CC_OPTIONS) ./programs/screenshot.cpp -o ./programs/screenshot.o

	@echo [CC] cpp_test.cpp
	@$(CC) $(CC_OPTIONS) ./programs/test/cpp_test.cpp -o ./programs/test/cpp_test.o

	@echo "\n///////////////////////////////////////////////"

	@echo "\033[0;32mXANIN BUILT-IN 16-BIT PROGRAMS COMPILATION PHARSE"
	@echo "\033[0m"
	
	@echo [ASM] shutdown.asm
	@$(ASM) -fbin $(APPS)/shutdown.asm -o $(APPS)/shutdown

	@echo "\n///////////////////////////////////////////////"

	@echo [C] C_LIBARY
	@i386-elf-ld -r ./lib/libc/hal.o \
	./lib/libc/math.o \
	./lib/libc/memory.o \
	./lib/libc/stdiox.o \
	./lib/libc/stdlibx.o \
	./lib/libc/string.o \
	./sys/log/syslog.o \
	./lib/libc/system.o \
	./lib/libc/time.o \
	./lib/libc/data_structures.o \
	./lib/libc/xanin_state.o \
	./lib/libc/instruction_pointer.o \
	./lib/libc/real_mode_fswitch.o \
	./lib/libc/algorithm.o \
	./lib/libc/signal.o \
	./lib/libc/process.o \
	./lib/libc/registers_values_get.o \
	./fs/xin.o \
	./fs/xin_syscalls.o \
	./sys/terminal/vty/vty.o \
	./sys/devices/hda/disk.o \
	./sys/terminal/interface/terminal.o -o ./lib/libc/libc.o

	@i386-elf-gcc -shared -Ttext 0x900000 -o ./lib/libc/libc.so ./lib/libc/libc.o
	@i386-elf-ar rsc ./lib/libc/libc.a ./lib/libc/libc.o ./lib/libc/crt0.o

	@echo [C] kernel.c
	@$(C) $(C_COMPILE) ./sys/kernel.c \
	./sys/pmmngr/alloc.o \
	./sys/input/input.o \
	./programs/register_dump.o \
	./sys/call/xanin_sys/pmmngr/alloc.o \
	./sys/interrupts/handlers/entries/divide_by_zero_exception.o \
	./sys/interrupts/handlers/entries/debug_exception.o \
	./sys/interrupts/handlers/entries/nmi_exception.o \
	./sys/interrupts/handlers/entries/breakpoint_exception.o \
	./sys/interrupts/handlers/entries/overflow_exception.o \
	./sys/interrupts/handlers/entries/bound_range_exceeded_exception.o \
	./sys/interrupts/handlers/entries/invalid_opcode_exception.o \
	./sys/interrupts/handlers/entries/device_not_availble_exception.o \
	./sys/interrupts/handlers/entries/double_fault_exception.o \
	./sys/interrupts/handlers/entries/coprocessor_segment_overrun_exception.o \
	./sys/interrupts/handlers/entries/invalid_tss_exception.o \
	./sys/interrupts/handlers/entries/segment_not_present_exception.o \
	./sys/interrupts/handlers/entries/stack_fault_exception.o \
	./sys/interrupts/handlers/entries/general_protection_exception.o \
	./sys/interrupts/handlers/entries/page_fault_exception.o \
	./sys/interrupts/handlers/entries/x86_fpu_floating_point_exception.o \
	./sys/interrupts/handlers/entries/aligment_check_exception.o \
	./sys/interrupts/handlers/entries/machine_check_exception.o \
	./sys/interrupts/handlers/entries/simd_floating_point_exception.o \
	./sys/interrupts/handlers/entries/virtualization_exception.o \
	./lib/libc/libc.o \
	./sys/devices/pit/pit.o \
	./sys/devices/pit/pit_entry.o \
	./lib/libcpp/algorithm.o \
	./lib/libcpp/regex.o \
	./lib/tui/tui.o \
	./lib/xgl/xgl.o \
	./lib/xaninGraphics/xaninGraphics.o \
	./lib/libcpp/endian.o \
	./lib/libcpp/icxxabi.o \
	./sys/devices/vga/vga.o \
	./sys/devices/keyboard/key_map.o \
	./sys/devices/keyboard/keyboard.o \
	./sys/call/syscall_entry.o \
	./sys/call/xanin_sys_entry.o \
	./sys/call/xanin_sys/xanin_sys.o \
	./programs/edit.o \
	./programs/test/cpp_test.o \
	./sys/devices/mouse/mouse_init.o \
	./sys/devices/mouse/mouse.o \
	./sys/devices/apic/apic.o \
	./sys/devices/nic/8254x_int_handler.o \
	./sys/devices/nic/8254x.o \
	./sys/devices/pcspk/pc_speaker.o \
	./sys/devices/usb/usb.o \
	./sys/devices/acpi/ACPI.o \
	./sys/net/netapi/objects/mac.o \
	./sys/net/netapi/objects/ip.o \
	./sys/net/network_protocols/ethernet_frame/ethernet_frame.o \
	./sys/net/network_protocols/arp/arp.o \
	./sys/net/network_protocols/internet_protocol/ipv4/ip.o \
	./sys/net/network_protocols/udp/udp.o \
	./sys/net/network_protocols/icmp/icmp.o \
	./sys/net/network_protocols/dhcp/dhcp.o \
	./sys/net/netapi/objects/response.c \
	./sys/net/netapi/network_devicecpp.o \
	./sys/net/netapi/netapi_interrupt.o \
	./sys/net/netapi/loopback/loopback.o \
	./sys/devices/ioapic/ioapic.o \
	./sys/interrupts/idt/idt.o \
	./sys/devices/com/ComPort.o \
	./programs/screenshot.o \
	./lib/game_engine/xagame.o \
	./programs/xagames/xagame_test.o \
	./programs/xagames/tetris.o -Ttext 0xB00000 -o kernel.bin

	@echo [C] disk_freestanding_driver.c
	@$(C) $(C_COMPILE) ./boot/disk_freestanding_driver.c ./boot/linker.ld -Ttext 0xA00000 -o ./boot/disk_freestanding_driver
	python3 ./utils/align_file.py -file ./boot/disk_freestanding_driver -size 512
	
	cp ./lib/libc/libc.o ./external_apps/libc.o
	cp ./lib/libc/libc.a ./external_apps/libc.a
	cp ./lib/libc/crt0.o ./external_apps/crt0.o
	cp ./lib/libc/libc.so ./external_apps/libsharedc.so

	python3 ./utils/XaninSupervisor.py
	
	@echo "\033[0;32mCOMPILING XANIN EXTERNAL APPS"
	@echo "\033[0m"
	@make -C ./external_apps/

	@echo "\033[0;32mCOMPILATION SUCCESS"
	@echo "\033[0m"

	@cat ./programs/shutdown $(APPS)/current_app $(APPS)/syscall_test ./lib/libc/real_mode_fswitch_asm ./lib/libc/fast_return_to_32_mode > ./programs/xanin_external_apps
	@dd if=./programs/xanin_external_apps of=./programs/xanin_apps_space bs=512 count=16 conv=notrunc
	
	@cat ./boot/boot ./lib/libc/enter_real_mode ./programs/xanin_apps_space ./programs/blank_sector ./fs/xin_pointers ./fs/entries_table ./boot/kernelLoader ./boot/disk_freestanding_driver kernel.bin > xanin.bin

	@dd if=xanin.bin of=xanin.img
	python3 ./utils/align_file.py -f ./xanin.img -size 800000
	@mv xanin.img -f ../bin
	@mv xanin.bin -f ../bin
	
	@python3 ./utils/app_preinstall.py -files external_apps/ ./etc/net/ ./etc/var/ -image $(XANIN_BINARIES)/xanin.img 

	@find ./ -name "*.o" -type f -delete

run:
	@sudo qemu-system-i386 -m 4096 -drive file=../bin/xanin.img,cache=unsafe -net nic -net tap,ifname=tap0,script=no,downscript=no
	# @sudo qemu-system-i386 -m 256 -drive file=../bin/xanin.img,cache=unsafe -net nic -net user

curses:
	@sudo qemu-system-i386 -m 256 -drive file=../bin/xanin.img,cache=unsafe -net nic -net tap,ifname=tap0,script=no,downscript=no -curses

debug:
	@sudo qemu-system-i386 -m 256 -drive file=../bin/xanin.img,cache=unsafe -net nic -net tap,ifname=tap0,script=no,downscript=no -s -S
	# @sudo qemu-system-i386 -drive file=../bin/xanin.img,cache=unsafe -net nic,model=e1000 -net tap -m 256 -s -S

