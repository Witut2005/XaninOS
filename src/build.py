

import os
import argparse
import sys
from colorama import init
from termcolor import colored

OBJECT = 1
BINARY = 2

class OwnOptions:
    def __init__(self, options):
        self.string = options

class CompileObject:

    def __init__(self, path, builder, options, output_name=None):
        self.path = path
        self.builder = builder
        self.options = options
        
        if isinstance(self.options, OwnOptions):
            self.options = self.options.string

        if output_name == OBJECT:
            self.output_name = path.split('.')[0] + path.split('.')[1] + '.o'
            if(path[0] == '.'):
                self.output_name = '.' + self.output_name

        elif output_name == BINARY:
            self.output_name = path.split('.')[0] + path.split('.')[1] + '.bin'
            if(path[0] == '.'):
                self.output_name = '.' + self.output_name

        else:
            self.output_name = output_name
    
    def command(self):
        if self.output_name == None:
            return self.builder + ' ' + self.options + ' ' + self.path
        else:
            return self.builder + ' ' + self.options + ' ' + self.path + ' -o ' + self.output_name


def terminate_if_error(exit_code: int):
    if exit_code != 0:
        sys.exit(1)
    return

def create_c_library(objpath, libpath, libraries, added=[]):
    final_string = ''
    for lib in libraries:
        final_string = final_string + ' ' + lib.output_name
    for lib in added:
        final_string = final_string + ' ' + lib

    commands = [
        args.linker + ' -r' + final_string + ' -o ' + objpath,
        args.archive + ' rsc ' + libpath + ' ' + objpath + ' ./lib/libc/crt0.o', 
        'cp ./lib/libc/libc.a ./external_apps/libc.a',
        'cp ./lib/libc/libc.o ./external_apps/libc.o',
        'cp ./lib/libc/crt0.o ./external_apps/crt0.o',
        'cd ./external_apps/',
        'make ./external_apps/',
        'cd ..'
    ]

    for command in commands:
        terminate_if_error(os.system(command))


def compile_kernel(*kargs):
    final_string = ''
    modules = kargs[0]

    for module in modules:

        if module == 'boot' or module == 'built-in 16bit programs' or module == '16bit mode': 
            continue

        for object in modules[module]:
            if object.output_name.split('.')[-1] == 'bin' or object.path.split('/')[-1] == 'crt0.asm':
                continue

            final_string = final_string + ' ' + object.output_name

    commands = [
        builders['c'] + ' ' + builder_options['c']['kernel'] + ' ./sys/kernel.c' + final_string + ' -o ' + './kernel.bin',
        'cat ./programs/shutdown.bin ./programs/current_app ./programs/syscall_test ./lib/libc/real_mode_fswitch_asm ./lib/libc/fast_return_to_32_mode > ./programs/xanin_external_apps',
        'dd if=./programs/xanin_external_apps of=./programs/xanin_apps_space bs=512 count=16 conv=notrunc',
        'cat ./boot/boot ./lib/libc/enter_real_mode ./programs/xanin_apps_space ./programs/blank_sector ./fs/xin_pointers ./fs/entries_table ./boot/kernelLoader ./boot/disk_freestanding_driver kernel.bin > xanin.bin',
        'dd if=xanin.bin of=xanin.img',
        'python3 ./utils/align_file.py -f ./xanin.img -size 600000',
        'mv xanin.img -f ../bin',
        'mv xanin.bin -f ../bin'
    ]

    if(args.preinstall == 'yes'):
        commands.append('make -C ./external_apps')
        commands.append('python3 ./utils/app_preinstall.py -files external_apps/ etc/ -image ../bin/xanin.img')
    
    for command in commands:
        terminate_if_error(os.system(command))

    
C = 'i386-elf-gcc'
CC = 'i386-elf-g++'

parser = argparse.ArgumentParser()

parser.add_argument('--srcpath', type=str)
parser.add_argument('--binpath', type=str)
parser.add_argument('--preinstall', type=str, default='yes')

parser.add_argument('--assembler', type=str, default='nasm')
parser.add_argument('--cbuilder', type=str, default='i386-elf-gcc')
parser.add_argument('--ccbuilder', type=str, default='i386-elf-g++')
parser.add_argument('--linker', type=str, default='i386-elf-ld')
parser.add_argument('--archive', type=str, default='i386-elf-ar')

parser.add_argument('--long', action='store_true')

args = parser.parse_args()

builders = {
    'asm': args.assembler,
    'c': args.cbuilder,
    'cc': args.ccbuilder,
}

builder_options = {
    'asm': {
        'bin': '-fbin',
        'elf32': '-f elf32'
    },

    'c':{
        'default': '-O0 -Wall -Werror -Wno-unused-but-set-variable -Wno-unused-variable -Wno-discarded-qualifiers -Wno-parentheses -Wno-comment -Wno-address-of-packed-member -Wno-maybe-uninitialized -Wno-pointer-sign -Wno-div-by-zero -Wno-duplicate-decl-specifier -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -Werror=return-type -I ./ -c',
        'kernel': '-O0 -Wall -Wno-discarded-qualifiers -Wno-parentheses -Wno-comment -Wno-address-of-packed-member -Wno-maybe-uninitialized -Wno-pointer-sign -Wno-div-by-zero -Wno-duplicate-decl-specifier -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding  -Wno-unused-function -Wno-div-by-zero -I ./'
    },

    'cc':{
        'default': '-O0 -fno-exceptions -lstdc++ -masm=intel -std=c++17 -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -Wno-write-strings -fno-rtti -fconcepts-ts -I ./ -c'
    }
}


objects_to_compile = {
    
    'boot': [
        CompileObject('./boot/boot.asm', builders['asm'], builder_options['asm']['bin'], BINARY),
        CompileObject('./boot/kernelLoader.asm', builders['asm'], builder_options['asm']['bin'], BINARY),
    ],

    'filesystem': [
        CompileObject('./fs/xin_pointers.asm', builders['asm'], builder_options['asm']['bin'], BINARY),
        CompileObject('./fs/entries_table.asm', builders['asm'], builder_options['asm']['bin'], BINARY),
        CompileObject('./fs/xin.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./fs/xin_syscalls.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./fs/xin_extended_table.asm', builders['asm'], builder_options['asm']['bin'], BINARY),
        # CompileObject('./fs/xanin_apps_space.asm', builders['asm'], builder_options['asm']['bin'], BINARY),
    ],

    'drivers': [
        CompileObject('./sys/devices/keyboard/keyboard.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/devices/pit/pit.asm', builders['asm'], builder_options['asm']['elf32'], './sys/devices/pit/pit_entry.o'),
        CompileObject('./sys/devices/pcspk/pc_speaker.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/devices/mouse/mouse.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/devices/mouse/mouse.asm', builders['asm'], builder_options['asm']['elf32'], './sys/devices/mouse/mouse_init.o'),
        CompileObject('./sys/devices/com/com.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/devices/ioapic/ioapic.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/devices/nic/8254x_int_handler.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/devices/nic/8254x.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),

        CompileObject('./sys/devices/apic/apic.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/devices/vga/vga.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/devices/usb/usb.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/devices/acpi/fadt/fadt.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/devices/acpi/madt/madt.c', builders['c'], builder_options['c']['default'], OBJECT),

        CompileObject('./sys/devices/acpi/rsdp/rsdp.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/devices/acpi/rsdt/rsdt.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/devices/acpi/sdt/sdt.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/devices/hda/disk.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/devices/nic/ne2000.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/devices/pit/pit.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/devices/keyboard/key_map.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],

    'syscalls': [
        CompileObject('./sys/call/posix/syscall_entry.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/call/xanin_sys/handler/xanin_sys_entry.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/call/xanin_sys/calls/devices/disk.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/call/xanin_sys/calls/stdio/stdio.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/call/xanin_sys/calls/terminal/terminal.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/call/xanin_sys/calls/vga/vga.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        # CompileObject('./sys/call/xanin_sys/calls/xanin_calls.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/call/xanin_sys/handler/xanin_sys.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],

    'debug': [
        CompileObject('./sys/debug/debug.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],

    'interrupt': [
        CompileObject('./sys/interrupts/idt/idt.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/divide_by_zero_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/debug_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/nmi_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/breakpoint_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/overflow_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/bound_range_exceeded_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/invalid_opcode_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/device_not_availble_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/double_fault_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/coprocessor_segment_overrun_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/invalid_tss_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/segment_not_present_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/stack_fault_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/general_protection_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/page_fault_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/x86_fpu_floating_point_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/aligment_check_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/machine_check_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/simd_floating_point_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/interrupts/handlers/entries/virtualization_exception.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
    ],

    'network': [
        CompileObject('./sys/net/network_protocols/ethernet_frame/ethernet_frame.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/net/network_protocols/icmp/icmp.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/net/network_protocols/arp/arp.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/net/network_protocols/internet_protocol/ipv4/ip.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/net/network_protocols/udp/udp.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/net/network_protocols/dhcp/dhcp.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
    ],

    'netapi': [
        CompileObject('./sys/net/netapi/netapi_interrupt.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/net/netapi/network_device.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/net/netapi/loopback/loopback.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/net/netapi/objects/ip.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/net/netapi/objects/response.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/net/netapi/objects/mac.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
    ],


    'kmodules': [
        CompileObject('./sys/screen/vty/vty.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/terminal/interface/terminal.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/pmmngr/alloc.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/input/input.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/log/syslog.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],

    'xanin_sys': [
        CompileObject('./sys/call/xanin_sys/calls/pmmngr/alloc.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],

    'libc':[
        # CompileObject('./lib/libc/real_mode_fswitch.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./lib/libc/file.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        # CompileObject('./lib/libc/crt0.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./lib/libc/alloc.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./lib/cpu/code/cpu_state_info.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        # CompileObject('./lib/libc/real_mode_fswitch.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./lib/libc/hal.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./lib/libc/math.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./lib/libc/memory.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./lib/libc/stdiox.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./lib/libc/stdlibx.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./lib/libc/signal.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./lib/libc/string.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./lib/libc/data_structures.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./lib/libc/system.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./lib/libc/algorithm.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./lib/libc/time.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./lib/libc/process.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./lib/libc/stdiox_legacy.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],

    'libcpp': [
        CompileObject('./lib/libcpp/algorithm.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libcpp/command_parser.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libcpp/icxxabi.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libcpp/regex.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        # CompileObject('./lib/libcpp/bytes.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libcpp/endian.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
    ],

    'vty': [
        CompileObject('./sys/terminal/frontend/frontend.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/terminal/backend/backend.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/terminal/handlers/handlers.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],

    'tui': [
        CompileObject('./lib/tui/tui.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],

    'xanin_graphics(legacy)': [
        CompileObject('./lib/xaninGraphics/xaninGraphics.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],
    
    'graphics_libraries': [
        CompileObject('./lib/xgl/xgl.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./lib/xgl/vga_rgb.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./lib/game_engine/xagame.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
    ],

    'built-in programs': [
        CompileObject('./programs/edit.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/xagames/xagame_test.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./programs/xagames/tetris.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./programs/screenshot.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./programs/test/cpp_test.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
    ],

    'built-in 16bit programs': [
        CompileObject('./programs/shutdown.asm', builders['asm'], builder_options['asm']['bin'], BINARY),
    ],
    
    '16bit mode': [
        CompileObject('./sys/real_mode/enter_real_mode.asm', builders['asm'], builder_options['asm']['bin'], BINARY),
        CompileObject('./sys/real_mode/fast_return_to_32_mode.asm', builders['asm'], builder_options['asm']['bin'], BINARY),
    ]

}

for os_module, objects in objects_to_compile.items():
    print(colored('\ncompling {} module'.format(os_module).upper(), 'green'))
    for object in objects:
        status = os.system(object.command())
        if status != 0:
            sys.exit(1)
        
        if args.long == False:
            print(object.path.ljust(90, ' '), end='')
        else:
            print(object.command())

        print(colored('OK', 'green'))
    
print(colored('\nXANIN OS MODULES BUILDED\n', 'green'))
    
create_c_library('./lib/libc/libc.o', './lib/libc/libc.a', objects_to_compile['libc'], [
        './sys/log/syslog.o', './fs/xin_syscalls.o', 
        './sys/screen/vty/vty.o', 
        './sys/devices/hda/disk.o', 
        # './sys/terminal/backend/backend.o', 
        # './sys/terminal/frontend/frontend.o',
        './fs/xin.o', './sys/call/xanin_sys/calls/devices/disk.o', './sys/call/xanin_sys/calls/stdio/stdio.o', 
        './sys/call/xanin_sys/calls/terminal/terminal.o', 
        './sys/call/xanin_sys/calls/vga/vga.o', 
        './sys/terminal/interface/terminal.o'        
                ])

# print(objects_to_compile['kmodules'] + objects_to_compile['interrupt'])
# compile_kernel(objects_to_compile['kmodules'] + objects_to_compile['interrupt'] + objects_to_compile['drivers'] + objects_to_compile['xanin_graphics(legacy)'] + objects_to_compile['graphics_libraries'] + 
#                 objects_to_compile['built-in programs'] + objects_to_compile['libc'] + objects_to_compile['libcpp'] + objects_to_compile['network'] + objects_to_compile['netapi'] + objects_to_compile[''])

compile_kernel(objects_to_compile)

print(colored('\nXANIN OS IMAGE BUILDED\n', 'green'))
