

C	= i386-elf-gcc
CC 	= i386-elf-g++

XANIN_PATH = /home/witut/Desktop/xaninOS/src/

# C_OPTIONS  = -O0 -masm=intel -Wno-discarded-qualifiers -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-int-conversion -Wno-unused-function -Wno-div-by-zero -Wno-address-of-packed-member -I $(XANIN_PATH) -c
C_COMPILE  = -O0 -masm=intel -Wno-discarded-qualifiers -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-int-conversion -Wno-unused-function -Wno-div-by-zero -Wno-address-of-packed-member -I $(XANIN_PATH) 
CC_OPTIONS =  -Os -fno-exceptions -lstdc++ -masm=intel -std=c++17 -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -Wno-write-strings -Wno-return-type -fno-rtti -fconcepts-ts -I $(XANIN_PATH) -c
C_OPTIONS  =  -O0 -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -Wno-discarded-qualifiers -ffreestanding -Wno-unused-function -Wno-return-type -I $(XANIN_PATH) -c

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
	@$(ASM) -f elf32 ./handlers/keyboard.asm -o ./handlers/keyboard.o

	@# echo [ASM] syscall.asm
	@# $(ASM) -f elf32 ./syscall/syscall.asm -o ./syscall/syscall.o
	
	@echo [ASM] pit.asm
	@$(ASM) -f elf32 ./pit/pit.asm -o ./pit/pit.o

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

	@echo [CC] apic.cpp
	@$(CC) $(CC_OPTIONS) ./devices/APIC/apic.cpp -o ./devices/APIC/apic.o

	@echo [CC] cpp_test.cpp
	@$(CC) $(CC_OPTIONS) ./test/cpp_test.cpp -o ./test/cpp_test.o

	@echo [C] pc_speaker.c
	@$(C) $(C_OPTIONS) ./devices/PCSPK/pc_speaker.c -o ./devices/PCSPK/pc_speaker.o

	@echo [C] mouse.c
	@$(C) $(C_OPTIONS) ./devices/MOUSE/mouse.c -o ./devices/MOUSE/mouse.o

	@echo [ASM] mouse.asm
	@$(ASM) -f elf32 ./devices/MOUSE/mouse.asm -o ./devices/MOUSE/mouse_init.o

	@echo [CC] com.cpp
	@$(CC) $(CC_OPTIONS) ./devices/COM/com.cpp -o ./devices/COM/ComPort.o

	@echo [CC] ioapic.cpp
	@$(CC) $(CC_OPTIONS) ./devices/IOAPIC/ioapic.cpp -o ./devices/IOAPIC/ioapic.o
	
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

	@echo [C] icmp.c
	@$(C) $(C_OPTIONS) ./network_protocols/icmp/icmp.c -o ./network_protocols/icmp/icmp.o

	@echo [C] sll.c
	@$(C) $(C_OPTIONS) ./network_protocols/sll/sll.c -o ./network_protocols/sll/sll.o

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

	@echo [C] network_device.cpp
	@$(C) $(C_OPTIONS) ./netapi/network_device.c -o ./netapi/network_device.o

	@echo [CC] dhcp.cpp
	@$(CC) $(CC_OPTIONS) ./network_protocols/dhcp/dhcp.cpp -o ./network_protocols/dhcp/dhcp.o

 
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

	@echo [C] string.c
	@$(C) $(C_OPTIONS) ./libc/string.c -o ./libc/string.o

	@echo [C] syslog.c
	@$(C) $(C_OPTIONS) ./libc/syslog.c -o ./libc/syslog.o

	@echo [C] time.c
	@$(C) $(C_OPTIONS) ./libc/time.c -o ./libc/time.o

	@echo [C] xaninGraphics 
	@$(C) $(C_OPTIONS) ./xaninGraphics/xaninGraphics.c -o ./xaninGraphics/xaninGraphics.o
	
	@# @echo [C] pit.c
	@# @$(C) $(C_OPTIONS) ./pit/pit.c -o ./pit/pit.o

	@echo [C] vty.c
	@$(C) $(C_OPTIONS) ./terminal/vty.c -o ./terminal/vty.o

	@echo [C] key_map.c
	@$(C) $(C_OPTIONS) ./keyboard/key_map.c -o ./keyboard/key_map.o

	@echo [C] xin.c
	@$(C) $(C_OPTIONS) ./xin_fs/xin.c -o ./xin_fs/xin.o

	@echo [C] edit.c
	@$(C) $(C_OPTIONS) ./xin_fs/edit.c -o ./xin_fs/edit.o

	@echo [C] real_mode_fswitch.c
	@$(C) $(C_OPTIONS) ./libc/real_mode_fswitch.c -o ./libc/real_mode_fswitch.o


	@echo [C] kernel.c
	@$(C) $(C_COMPILE) kernel.c \
	./libc/alloc.o \
	./libc/hal.o \
	./libc/math.o \
	./libc/memory.o \
	./libc/stdiox.o \
	./libc/stdlibx.o \
	./libc/string.o \
	./libc/syslog.o \
	./libc/time.o \
	./libc/xanin_state.o \
	./libc/instruction_pointer.o \
	./libc/real_mode_fswitch.o \
	./libcpp/endian.o \
	./terminal/vty.o \
	./keyboard/key_map.o \
	./xaninGraphics/xaninGraphics.o \
	./handlers/keyboard.o \
	./pit/pit.o \
	./xin_fs/xin.o \
	./xin_fs/edit.o \
	./test/cpp_test.o \
	./test/cpp_test2.o \
	./devices/MOUSE/mouse_init.o \
	./devices/MOUSE/mouse.o \
	./devices/APIC/apic.o \
	./devices/NIC/8254x_int_handler.o \
	./devices/NIC/8254x.o \
	./devices/PCSPK/pc_speaker.o \
	./network_protocols/ethernet_frame/ethernet_frame.o \
	./network_protocols/arp/arp.o \
	./network_protocols/internet_protocol/ipv4/ip.o \
	./network_protocols/udp/udp.o \
	./network_protocols/icmp/icmp.o \
	./network_protocols/dhcp/dhcp.o \
	./network_protocols/sll/sll.o \
	./netapi/network_device.o \
	./devices/IOAPIC/ioapic.o \
	./test/testc.o \
	./devices/COM/ComPort.o \
	./xaninApps/screenshot.o \
	./game_engine/xagame.o \
	./xagames/xagame_test.o \
	./xagames/tetris.o -o kernel.bin

	@echo [C] disk_freestanding_driver.c
	@$(C) $(C_COMPILE) ./boot/disk_freestanding_driver.c ./boot/linker.ld -Ttext 0x700000 -o ./boot/disk_freestanding_driver.o
	python3 ./utils/align_file.py -file ./boot/disk_freestanding_driver.o -size 512
	

	@echo "\033[0;32mCOMPILATION SUCCESS"
	@echo "\033[0m"

	@cat ./xaninApps/shutdown $(APPS)/current_app $(APPS)/syscall_test ./libc/real_mode_fswitch_asm ./libc/fast_return_to_32_mode > ./xaninApps/xanin_external_apps
	@dd if=./xaninApps/xanin_external_apps of=./xaninApps/xanin_apps_space bs=512 count=16 conv=notrunc
	
ifeq ($(boot), 3)
	## @cat ./yan-bootloader/yan.bin ./libc/enter_real_mode ./xaninApps/xanin_apps_space ./xin_fs/xin_pointers ./xin_fs/entries_table ./boot/kernelLoader kernel.bin > xanin.bin
	@cat ./boot/boot ./libc/enter_real_mode ./xaninApps/xanin_apps_space ./xaninApps/blank_sector ./xin_fs/xin_pointers ./xin_fs/entries_table ./boot/kernelLoader ./boot/disk_freestanding_driver.o kernel.bin > xanin.bin
else
	@cat ./boot/boot ./libc/enter_real_mode ./xaninApps/xanin_apps_space ./xaninApps/blank_sector ./xin_fs/xin_pointers ./xin_fs/entries_table ./boot/kernelLoader kernel.bin > xanin.bin
endif


	@dd if=xanin.bin of=xanin.img
	python3 ./utils/align_file.py -f ./xanin.img -size 800000
	@mv xanin.img -f ../bin
	@mv xanin.bin -f ../bin



run:
	@# qemu-system-i386 -smp 2 -net nic,model=e1000 -drive file=../bin/xanin.img,cache=unsafe -m 512
	sudo qemu-system-i386 -drive file=../bin/xanin.img,cache=unsafe -m 4096 -net nic -net tap
	# sudo qemu-system-i386 -drive file=../bin/xanin.img,cache=unsafe -m 512
	@#sudo qemu-system-i386 -drive file=../bin/xanin.img,cache=unsafe -m 8192 -netdev user,id=user.0 -device e1000,netdev=user.0


curses:
	qemu-system-i386 -curses -k en-us -m 512 -hda ../bin/xanin.img 

debug:
	sudo qemu-system-i386 -drive file=../bin/xanin.img,cache=unsafe -m 512 -net nic -net tap -s -S

