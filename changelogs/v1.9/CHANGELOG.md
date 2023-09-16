
# XaninOS version 1.9 changelog

## Changes in coding style
- From now every new kernel functions will be prefixed using double underscore. Syscalls will be using **__sys** prefix.

## Terminal huge speed boost
- From now XaninOS terminal flushing is about 25 times faster
## Terminal flushing interval
- XaninOS will automatically flush its terminal buffer every n-seconds. You can modify n value by changing etc/stdio/refresh_rate.conf file. then you also need to enter the **stdio_apply** command

## Canvas/Legacy stdio mode
- Old stdio calls is now called legacy or canvas calls. This functions can be used in old apps, but also in **TUI** apps. Canvas mode disables automatic terminal flushing.

## Built-in programs
- each program is now a separate compilation unit