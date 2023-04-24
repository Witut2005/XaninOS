

C	= i386-elf-gcc
CC 	= i386-elf-g++

XANIN_PATH = /home/witut/Desktop/xaninOS/src/
XANIN_BINARIES = /home/witut/Desktop/xaninOS/bin

# C_OPTIONS  = -O0 -masm=intel -Wno-discarded-qualifiers -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-int-conversion -Wno-unused-function -Wno-div-by-zero -Wno-address-of-packed-member -I $(XANIN_PATH) -c
C_COMPILE  = -O0 -masm=intel -Wno-discarded-qualifiers -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-int-conversion -Wno-unused-function -Wno-div-by-zero -Wno-address-of-packed-member -I $(XANIN_PATH) 
CC_OPTIONS =  -O0 -fno-exceptions -lstdc++ -masm=intel -std=c++17 -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -Wno-write-strings -fno-rtti -fconcepts-ts -I $(XANIN_PATH) -c
C_OPTIONS  =  -O0 -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -Werror=return-type -I $(XANIN_PATH) -c

ASM	= nasm
APPS = ./xaninApps

HANDLERS = ./handlers

build:

	@echo [ASM] boot.asm
	@$(ASM) -fbin ./boot/boot.asm

	@echo [ASM] boot.asm
	@$(ASM) -fbin ./boot/kernelLoader.asm
	
	@echo [ASM] xin_pointers.asm
	@$(ASM) -fbin ./xin_fs/xin_pointers.asm

	@echo [ASM] entries_table.asm
	@$(ASM) -fbin ./xin_fs/entries_table.asm
	
	@echo [ASM] keyboard.asm
	@$(ASM) -f elf32 ./keyboard/keyboard.asm -o ./keyboard/keyboard.o

	@ echo [ASM] registers_values_get.asm
	@ $(ASM) -f elf32 ./libc/registers_values_get.asm -o ./libc/registers_values_get.o
	
	@echo [ASM] pit.asm
	@$(ASM) -f elf32 ./pit/pit.asm -o ./pit/pit_entry.o

	@echo [ASM] syscall_entry.asm
	@$(ASM) -f elf32 ./syscall/syscall_entry.asm -o ./syscall/syscall_entry.o

	@echo [ASM] enter_real_mode.asm
	@$(ASM) -fbin ./libc/enter_real_mode.asm -o ./libc/enter_real_mode

	@echo [ASM] shutdown.asm
	@$(ASM) -fbin $(APPS)/shutdown.asm -o $(APPS)/shutdown

	@echo [ASM] reboot.asm
	@$(ASM) -fbin $(APPS)/reboot.asm -o $(APPS)/reboot

	@echo [ASM] syscall_test.asm
	@$(ASM) -fbin $(APPS)/syscall_test.asm -o  $(APPS)/syscall_test -w-other

	@echo [ASM] xanin_apps_space.asm
	@$(ASM) -fbin $(APPS)/xanin_apps_space.asm -o $(APPS)/xanin_apps_space

	@echo [ASM] instruction_pointer.asm
	@$(ASM) -f elf32 ./libc/instruction_pointer.asm -o ./libc/instruction_pointer.o

	@echo [ASM] real_mode_fswitch.asm
	@$(ASM) -fbin ./libc/real_mode_fswitch.asm -o ./libc/real_mode_fswitch_asm

	@echo [ASM] xanin_state.asm
	@$(ASM) -f elf32 ./libc/xanin_state.asm -o ./libc/xanin_state.o

	@echo [ASM] divide_by_zero_exception
	@$(ASM) -f elf32 ./handlers/entries/divide_by_zero_exception.asm -o ./handlers/entries/divide_by_zero_exception.o

	@echo [ASM] debug_exception
	@$(ASM) -f elf32 ./handlers/entries/debug_exception.asm -o ./handlers/entries/debug_exception.o

	@echo [ASM] nmi_exception
	@$(ASM) -f elf32 ./handlers/entries/nmi_exception.asm -o ./handlers/entries/nmi_exception.o

	@echo [ASM] breakpoint_exception 
	@$(ASM) -f elf32 ./handlers/entries/breakpoint_exception.asm -o ./handlers/entries/breakpoint_exception.o

	@echo [ASM] overflow_exception 
	@$(ASM) -f elf32 ./handlers/entries/overflow_exception.asm -o ./handlers/entries/overflow_exception.o

	@echo [ASM] bound_range_exceeded_exception
	@$(ASM) -f elf32 ./handlers/entries/bound_range_exceeded_exception.asm -o ./handlers/entries/bound_range_exceeded_exception.o

	@echo [ASM] invalid_opcode_exception 
	@$(ASM) -f elf32 ./handlers/entries/invalid_opcode_exception.asm -o ./handlers/entries/invalid_opcode_exception.o

	@echo [ASM] device_not_availble_exception
	@$(ASM) -f elf32 ./handlers/entries/device_not_availble_exception.asm -o ./handlers/entries/device_not_availble_exception.o

	@echo [ASM] double_fault_exception 
	@$(ASM) -f elf32 ./handlers/entries/double_fault_exception.asm -o ./handlers/entries/double_fault_exception.o

	@echo [ASM] coprocessor_segment_overrun_exception
	@$(ASM) -f elf32 ./handlers/entries/coprocessor_segment_overrun_exception.asm -o ./handlers/entries/coprocessor_segment_overrun_exception.o

	@echo [ASM] invalid_tss_exception
	@$(ASM) -f elf32 ./handlers/entries/invalid_tss_exception.asm -o ./handlers/entries/invalid_tss_exception.o

	@echo [ASM] segment_not_present_exception
	@$(ASM) -f elf32 ./handlers/entries/segment_not_present_exception.asm -o ./handlers/entries/segment_not_present_exception.o

	@echo [ASM] stack_fault_exception
	@$(ASM) -f elf32 ./handlers/entries/stack_fault_exception.asm -o ./handlers/entries/stack_fault_exception.o

	@echo [ASM] general_protection_exception 
	@$(ASM) -f elf32 ./handlers/entries/general_protection_exception.asm -o ./handlers/entries/general_protection_exception.o

	@echo [ASM] page_fault_exception
	@$(ASM) -f elf32 ./handlers/entries/page_fault_exception.asm -o ./handlers/entries/page_fault_exception.o

	@echo [ASM] x86_fpu_floating_point_exception 
	@$(ASM) -f elf32 ./handlers/entries/x86_fpu_floating_point_exception.asm -o ./handlers/entries/x86_fpu_floating_point_exception.o

	@echo [ASM] aligment_check_exception
	@$(ASM) -f elf32 ./handlers/entries/aligment_check_exception.asm -o ./handlers/entries/aligment_check_exception.o

	@echo [ASM] machine_check_exception
	@$(ASM) -f elf32 ./handlers/entries/machine_check_exception.asm -o ./handlers/entries/machine_check_exception.o

	@echo [ASM] simd_floating_point_exception
	@$(ASM) -f elf32 ./handlers/entries/simd_floating_point_exception.asm -o ./handlers/entries/simd_floating_point_exception.o

	@echo [ASM] virtualization_exception
	@$(ASM) -f elf32 ./handlers/entries/virtualization_exception.asm -o ./handlers/entries/virtualization_exception.o

	@echo [ASM] coprocessor_segment_overrun_exception
	@$(ASM) -f elf32 ./handlers/entries/coprocessor_segment_overrun_exception.asm -o ./handlers/entries/coprocessor_segment_overrun_exception.o

	@echo [ASM] netapi_interrupt
	@$(ASM) -f elf32 ./netapi/netapi_interrupt.asm -o ./netapi/netapi_interrupt.o

	@echo [CPP] ip.cpp
	@$(CC) $(CC_OPTIONS) ./netapi/objects/ip.cpp -o ./netapi/objects/ip.o 

	@echo [CC] apic.cpp
	@$(CC) $(CC_OPTIONS) ./devices/APIC/apic.cpp -o ./devices/APIC/apic.o

	@echo [CC] cpp_test.cpp
	@$(CC) $(CC_OPTIONS) ./test/cpp_test.cpp -o ./test/cpp_test.o

	@echo [C] pc_speaker.c
	@$(C) $(C_OPTIONS) ./devices/PCSPK/pc_speaker.c -o ./devices/PCSPK/pc_speaker.o

	@echo [C] mouse.c
	@$(C) $(C_OPTIONS) ./devices/MOUSE/mouse.c -o ./devices/MOUSE/mouse.o

	# @echo [CC] mouse.cpp
	# @$(CC) $(CC_OPTIONS) ./devices/mouse.cpp -o ./devices/mouse.o

	@echo [ASM] mouse.asm
	@$(ASM) -f elf32 ./devices/MOUSE/mouse.asm -o ./devices/MOUSE/mouse_init.o

	@echo [CC] com.cpp
	@$(CC) $(CC_OPTIONS) ./devices/COM/com.cpp -o ./devices/COM/ComPort.o

	@echo [CC] ioapic.cpp
	@$(CC) $(CC_OPTIONS) ./devices/IOAPIC/ioapic.cpp -o ./devices/IOAPIC/ioapic.o

	@echo [CC] regex.cpp
	@$(CC) $(CC_OPTIONS) ./libcpp/regex.cpp -o ./libcpp/regex.o 
	
	@echo [C] xgl.c
	@$(C) $(C_OPTIONS) ./xgl/xgl.c -o ./xgl/xgl.o

	
	@echo [ASM] 8254x INTERRUPT ENTRY
	@$(ASM) -f elf32 ./devices/NIC/8254x_int_handler.asm -o ./devices/NIC/8254x_int_handler.o

	@echo [ASM] fast_return_to_32_mode.asm 
	@$(ASM) -fbin ./libc/fast_return_to_32_mode.asm -o ./libc/fast_return_to_32_mode

	@echo [CC] 8254x.cpp
	@$(CC) $(CC_OPTIONS) ./devices/NIC/8254x.cpp -o ./devices/NIC/8254x.o

	@echo [CC] EthernetFrameInterface
	@$(CC) $(CC_OPTIONS) ./network_protocols/ethernet_frame/ethernet_frame.cpp -o ./network_protocols/ethernet_frame/ethernet_frame.o

	@echo [CC] arp.cpp
	@$(CC) $(CC_OPTIONS) ./network_protocols/arp/arp.cpp -o ./network_protocols/arp/arp.o

	@echo [CC] ip.cpp
	@$(CC) $(CC_OPTIONS) ./network_protocols/internet_protocol/ipv4/ip.cpp -o ./network_protocols/internet_protocol/ipv4/ip.o

	@echo [CC] udp.cpp
	@$(CC) $(CC_OPTIONS) ./network_protocols/udp/udp.cpp -o ./network_protocols/udp/udp.o

	@echo [C] idt.c
	@$(C) $(C_OPTIONS) ./IDT/idt.c -o ./IDT/idt.o

	@echo [C] icmp.c
	@$(C) $(C_OPTIONS) ./network_protocols/icmp/icmp.c -o ./network_protocols/icmp/icmp.o

	@echo [CC] screenshot.cpp
	@$(CC) $(CC_OPTIONS) ./xaninApps/screenshot.cpp -o ./xaninApps/screenshot.o

	@echo [CC] xagame.cpp
	@$(CC) $(CC_OPTIONS) ./game_engine/xagame.cpp -o ./game_engine/xagame.o

	@echo [CC] xagame_test.cpp
	@$(CC) $(CC_OPTIONS) ./xagames/xagame_test.cpp -o ./xagames/xagame_test.o

	@echo [CC] tetris.cpp
	@$(CC) $(CC_OPTIONS) ./xagames/tetris.cpp -o ./xagames/tetris.o

	@# @echo [CC] bytes.cpp
	@# @$(CC) $(CC_OPTIONS) ./libcpp/bytes.cpp -o ./libcpp/bytes.o

	@# @echo [CC] tuple.hpp
	@# @$(CC) $(CC_OPTIONS) ./libcpp/tuple.hpp -o ./libcpp/tuple.o

	@echo [CC] endian.cpp
	@$(CC) $(CC_OPTIONS) ./libcpp/endian.cpp -o ./libcpp/endian.o

	@echo [CC] network_device.cpp
	@$(CC) $(CC_OPTIONS) ./netapi/network_device.cpp -o ./netapi/network_devicecpp.o

	@echo [CC] loopback.cpp
	@$(CC) $(CC_OPTIONS) ./netapi/loopback/loopback.cpp -o ./netapi/loopback/loopback.o

	@echo [C] response.c
	@$(CC) $(CC_OPTIONS) ./netapi/objects/response.c -o ./netapi/objects/response.o

	@echo [CC] mac.cpp
	@$(CC) $(CC_OPTIONS) ./netapi/objects/mac.cpp -o ./netapi/objects/mac.o 

	@echo [CC] dhcp.cpp
	@$(CC) $(CC_OPTIONS) ./network_protocols/dhcp/dhcp.cpp -o ./network_protocols/dhcp/dhcp.o

	@echo [CC] ne2000.cpp
	@$(CC) $(CC_OPTIONS) ./devices/NIC/ne2000.cpp -o ./devices/NIC/ne2000.o

 
	@#C LIBARIES

	@echo [C] alloc.c
	@$(C) $(C_OPTIONS) ./libc/alloc.c -o ./libc/alloc.o

	@echo [C] hal.c
	@$(C) $(C_OPTIONS) ./libc/hal.c -o ./libc/hal.o

	@echo [C] math.c
	@$(C) $(C_OPTIONS) ./libc/math.c -o ./libc/math.o

	@echo [C] memory.c
	@$(C) $(C_OPTIONS) ./libc/memory.c -o ./libc/memory.o

	@echo [C] stdiox.c
	@$(C) $(C_OPTIONS) ./libc/stdiox.c -o ./libc/stdiox.o

	@echo [C] stdlibx.c
	@$(C) $(C_OPTIONS) ./libc/stdlibx.c -o ./libc/stdlibx.o

	@echo [C] signal.c
	@$(C) $(C_OPTIONS) ./libc/signal.c -o ./libc/signal.o

	@echo [C] tui.c
	@$(C) $(C_OPTIONS) ./tui/tui.c -o ./tui/tui.o

	@echo [C] terminal.c
	@$(C) $(C_OPTIONS) ./terminal/terminal.c -o ./terminal/terminal.o

	@echo [C] data_structures.c
	@$(C) $(C_OPTIONS) ./libc/data_structures.c -o ./libc/data_structures.o

	@echo [C] system.c
	@$(C) $(C_OPTIONS) ./libc/system.c -o ./libc/system.o

	@echo [C] string.c
	@$(C) $(C_OPTIONS) ./libc/string.c -o ./libc/string.o

	@echo [CC] algorithm.cpp
	@$(CC) $(CC_OPTIONS) ./libcpp/algorithm.cpp -o ./libcpp/algorithm.o

	@echo [CC] command_parser.cpp
	@$(CC) $(CC_OPTIONS) ./libcpp/command_parser.cpp -o ./libcpp/command_parser.o

	@echo [CC] icxabi.cpp
	@$(CC) $(CC_OPTIONS) ./libcpp/icxxabi.cpp -o ./libcpp/icxxabi.o

	@echo [C] algorithm.c
	@$(C) $(C_OPTIONS) ./libc/algorithm.c -o ./libc/algorithm.o

	@echo [C] syslog.c
	@$(C) $(C_OPTIONS) ./libc/syslog.c -o ./libc/syslog.o

	@echo [C] time.c
	@$(C) $(C_OPTIONS) ./libc/time.c -o ./libc/time.o

	@echo [C] process.c
	@$(C) $(C_OPTIONS) ./libc/process.c -o ./libc/process.o
	
	@echo [C] register_dump.c
	@$(C) $(C_OPTIONS) ./libc/register_dump.c -o ./libc/register_dump.o

	@echo [C] xaninGraphics 
	@$(C) $(C_OPTIONS) ./xaninGraphics/xaninGraphics.c -o ./xaninGraphics/xaninGraphics.o
	
	@echo [C] pit.c
	@$(C) $(C_OPTIONS) ./pit/pit.c -o ./pit/pit.o

	@echo [C] vty.c
	@$(C) $(C_OPTIONS) ./terminal/vty.c -o ./terminal/vty.o

	@echo [C] key_map.c
	@$(C) $(C_OPTIONS) ./keyboard/key_map.c -o ./keyboard/key_map.o

	@echo [C] xin.c
	@$(C) $(C_OPTIONS) ./xin_fs/xin.c -o ./xin_fs/xin.o

	@echo [C] xin_syscalls.c
	@$(C) $(C_OPTIONS) ./xin_fs/xin_syscalls.c -o ./xin_fs/xin_syscalls.o

	@echo [C] edit.c
	@$(C) $(C_OPTIONS) ./xin_fs/edit.c -o ./xin_fs/edit.o

	@echo [C] vga.c
	@$(C) $(C_OPTIONS) ./devices/VGA/vga.c -o ./devices/VGA/vga.o

	@echo [C] usb.c
	@$(C) $(C_OPTIONS) ./devices/USB/usb.c -o ./devices/USB/usb.o

	@echo [C] ACPI.c
	@$(C) $(C_OPTIONS) ./devices/ACPI/ACPI.c -o ./devices/ACPI/ACPI.o

	@echo [C] disk.c
	@$(C) $(C_OPTIONS) ./devices/HARD_DISK/disk.c -o ./devices/HARD_DISK/disk.o

	@echo [C] real_mode_fswitch.c
	@$(C) $(C_OPTIONS) ./libc/real_mode_fswitch.c -o ./libc/real_mode_fswitch.o

	@echo [ASM] xin_extended_table.asm
	@$(ASM) ./xin_fs/xin_extended_table.asm -o ./xin_fs/xin_extended_table



	@echo [C] kernel.c
	@$(C) $(C_COMPILE) kernel.c \
	./handlers/entries/divide_by_zero_exception.o \
	./handlers/entries/debug_exception.o \
	./handlers/entries/nmi_exception.o \
	./handlers/entries/breakpoint_exception.o \
	./handlers/entries/overflow_exception.o \
	./handlers/entries/bound_range_exceeded_exception.o \
	./handlers/entries/invalid_opcode_exception.o \
	./handlers/entries/device_not_availble_exception.o \
	./handlers/entries/double_fault_exception.o \
	./handlers/entries/coprocessor_segment_overrun_exception.o \
	./handlers/entries/invalid_tss_exception.o \
	./handlers/entries/segment_not_present_exception.o \
	./handlers/entries/stack_fault_exception.o \
	./handlers/entries/general_protection_exception.o \
	./handlers/entries/page_fault_exception.o \
	./handlers/entries/x86_fpu_floating_point_exception.o \
	./handlers/entries/aligment_check_exception.o \
	./handlers/entries/machine_check_exception.o \
	./handlers/entries/simd_floating_point_exception.o \
	./handlers/entries/virtualization_exception.o \
	./libc/alloc.o \
	./libc/hal.o \
	./libc/math.o \
	./libc/memory.o \
	./libc/stdiox.o \
	./libc/stdlibx.o \
	./libc/string.o \
	./libc/syslog.o \
	./libc/system.o \
	./libc/time.o \
	./libc/data_structures.o \
	./libc/xanin_state.o \
	./libc/instruction_pointer.o \
	./libc/real_mode_fswitch.o \
	./libc/algorithm.o \
	./libc/signal.o \
	./libc/process.o \
	./libc/register_dump.o \
	./libcpp/algorithm.o \
	./libcpp/regex.o \
	./tui/tui.o \
	./xgl/xgl.o \
	./libcpp/endian.o \
	./libcpp/icxxabi.o \
	./devices/VGA/vga.o \
	./libc/registers_values_get.o \
	./terminal/vty.o \
	./terminal/terminal.o \
	./keyboard/key_map.o \
	./xaninGraphics/xaninGraphics.o \
	./keyboard/keyboard.o \
	./pit/pit_entry.o \
	./pit/pit.o \
	./syscall/syscall_entry.o \
	./xin_fs/xin.o \
	./xin_fs/xin_syscalls.o \
	./xin_fs/edit.o \
	./test/cpp_test.o \
	./test/cpp_test2.o \
	./devices/MOUSE/mouse_init.o \
	./devices/MOUSE/mouse.o \
	./devices/APIC/apic.o \
	./devices/NIC/8254x_int_handler.o \
	./devices/NIC/8254x.o \
	./devices/PCSPK/pc_speaker.o \
	./devices/USB/usb.o \
	./devices/ACPI/ACPI.o \
	./devices/HARD_DISK/disk.o \
	./netapi/objects/mac.o \
	./netapi/objects/ip.o \
	./network_protocols/ethernet_frame/ethernet_frame.o \
	./network_protocols/arp/arp.o \
	./network_protocols/internet_protocol/ipv4/ip.o \
	./network_protocols/udp/udp.o \
	./network_protocols/icmp/icmp.o \
	./network_protocols/dhcp/dhcp.o \
	./netapi/objects/response.c \
	./netapi/network_devicecpp.o \
	./netapi/netapi_interrupt.o \
	./netapi/loopback/loopback.o \
	./devices/IOAPIC/ioapic.o \
	./test/testc.o \
	./IDT/idt.o \
	./devices/COM/ComPort.o \
	./xaninApps/screenshot.o \
	./game_engine/xagame.o \
	./xagames/xagame_test.o \
	./xagames/tetris.o -Ttext 0xB00000 -o kernel.bin

	@echo [C] disk_freestanding_driver.c
	@$(C) $(C_COMPILE) ./boot/disk_freestanding_driver.c ./boot/linker.ld -Ttext 0xA00000 -o ./boot/disk_freestanding_driver
	python3 ./utils/align_file.py -file ./boot/disk_freestanding_driver -size 512
	

	@echo "\033[0;32mCOMPILATION SUCCESS"
	@echo "\033[0m"

	@cat ./xaninApps/shutdown $(APPS)/current_app $(APPS)/syscall_test ./libc/real_mode_fswitch_asm ./libc/fast_return_to_32_mode > ./xaninApps/xanin_external_apps
	@dd if=./xaninApps/xanin_external_apps of=./xaninApps/xanin_apps_space bs=512 count=16 conv=notrunc
	
## ifeq ($(boot), 3)
	## @cat ./yan-bootloader/yan.bin ./libc/enter_real_mode ./xaninApps/xanin_apps_space ./xin_fs/xin_pointers ./xin_fs/entries_table ./boot/kernelLoader kernel.bin > xanin.bin
	@cat ./boot/boot ./libc/enter_real_mode ./xaninApps/xanin_apps_space ./xaninApps/blank_sector ./xin_fs/xin_pointers ./xin_fs/entries_table ./boot/kernelLoader ./boot/disk_freestanding_driver kernel.bin > xanin.bin
## else
## 	@cat ./boot/boot ./libc/enter_real_mode ./xaninApps/xanin_apps_space ./xaninApps/blank_sector ./xin_fs/xin_pointers ./xin_fs/entries_table ./boot/kernelLoader kernel.bin ./xin_fs/xin_extended_table > xanin.bin
## endif


	@dd if=xanin.bin of=xanin.img
	python3 ./utils/align_file.py -f ./xanin.img -size 800000
	@mv xanin.img -f ../bin
	@mv xanin.bin -f ../bin
	
	@echo "\033[0;32mCOMPILING XANIN EXTERNAL APPS"
	@echo "\033[0m"
	@make -C ./external_apps/

	@python3 ./utils/app_preinstall.py -files yhm/ external_apps/ config/ -image $(XANIN_BINARIES)/xanin.img 




run:
	@sudo qemu-system-i386 -m 256 -drive file=../bin/xanin.img,cache=unsafe -net nic -net tap,ifname=tap0,script=no,downscript=no
	# @sudo qemu-system-i386 -m 256 -drive file=../bin/xanin.img,cache=unsafe -net nic -net user

curses:
	@sudo qemu-system-i386 -m 256 -drive file=../bin/xanin.img,cache=unsafe -net nic -net tap,ifname=tap0,script=no,downscript=no -curses

debug:
	@sudo qemu-system-i386 -m 256 -drive file=../bin/xanin.img,cache=unsafe -net nic -net tap,ifname=tap0,script=no,downscript=no -s -S
	# @sudo qemu-system-i386 -drive file=../bin/xanin.img,cache=unsafe -net nic,model=e1000 -net tap -m 256 -s -S

