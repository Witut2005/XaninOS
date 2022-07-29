
@echo off
qemu-system-i386.exe -smp 2 -net nic,macaddr=24:22:33:44:55:66 -drive file=../bin/xanin.img,cache=unsafe -m 512