
build:
	python3 build.py

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

