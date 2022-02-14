
#qemu-system-x86_64 -machine accel=kvm:tcg -nic none -m 512 -drive ../bin/xanin.img
qemu-system-x86_64 -m 512 -hda ../bin/xanin.img
