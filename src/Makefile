C = i386-elf-gcc

build:
	python3 build.py

gcc_obj:
	CRTBEGIN_PATH := $(shell i386-elf-gcc -print-file-name=crtbegin.o)
	CRTEND_PATH := $(shell $C -print-file-name=crtend.o)

	echo CRTBEGIN_PATH CRTEND_PATH

burun:
	python3 build.py
	@sudo qemu-system-i386 -m 2048 -drive file=../bin/xanin.img,cache=unsafe -net nic -net tap,ifname=tap0,script=no,downscript=no

run:
	@sudo qemu-system-i386 -m 2048 -drive file=../bin/xanin.img,cache=unsafe -net nic -net tap,ifname=tap0,script=no,downscript=no

curses:
	@sudo qemu-system-i386 -m 256 -drive file=../bin/xanin.img,cache=unsafe -net nic -net tap,ifname=tap0,script=no,downscript=no -curses

debug:
	@sudo qemu-system-i386 -m 256 -drive file=../bin/xanin.img,cache=unsafe -net nic -net tap,ifname=tap0,script=no,downscript=no -s -S

clean:
	find ./ -type f \( -name "*.bin" -o -name "*.o" \) -delete
	find ./ -type f ! -name "*.*"

