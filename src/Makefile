
bld_run:
	bash .build.sh
	bash .run_linux.sh

build:
	bash .build.sh

run:
	#qemu-system-i386 -device usb-ehci,id=ehci,cache=unsafe -m 512 ../bin/xanin.bin
	qemu-system-i386 -drive file=../bin/xanin.img,cache=unsafe -m 512


curses:
	qemu-system-i386 -curses -k en-us -m 512 -hda ../bin/xanin.img 

debug:
	qemu-system-i386 -m 512 -s -S ../bin/xanin.img 
windows:
	qemu-system-i386.exe -m 512 -hda ..\bin\xanin.img
