
build:
	python3 build.py

run:
	@sudo qemu-system-i386 -m 2048 -drive file=../bin/xanin.img,cache=unsafe -net nic -net tap,ifname=tap0,script=no,downscript=no

curses:
	@sudo qemu-system-i386 -m 256 -drive file=../bin/xanin.img,cache=unsafe -net nic -net tap,ifname=tap0,script=no,downscript=no -curses

debug:
	@sudo qemu-system-i386 -m 256 -drive file=../bin/xanin.img,cache=unsafe -net nic -net tap,ifname=tap0,script=no,downscript=no -s -S

