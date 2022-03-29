
#qemu-system-x86_64 -machine accel=kvm:tcg -nic none -m 512 -drive ../bin/xanin.img
#qemu-system-x86_64 -m 512 -hda ../bin/xanin.img
qemu-system-i386 -curses -k en-us -m 512 -hda ../bin/xanin.img 
#qemu-system-x86_64 -drive format=raw,file=../bin/xanin.img, if=hda 
