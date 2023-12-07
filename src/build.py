

import os
import argparse
import sys
import subprocess
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

def create_kernel_c_library(objpath, libpath, objs=[]):
    commands = [
        f"{args.linker} -r {' '.join(objs)} -o {objpath},"
        f"{args.archive} rsc {libpath} {objpath}"
    ]

    for command in commands:
        terminate_if_error(os.system(command))


def create_c_library(objpath, libpath, objs=[]):
    commands = [
        f"{args.linker} -r {' '.join(objs)} -o {objpath}",
        f"{args.archive} rsc {libpath} {objpath} ./lib/libc/crt0.o", 
        'cp ./lib/libc/libc.a ./external_apps/libc.a',
        'cp ./lib/libc/libc.o ./external_apps/libc.o',
        'cp ./lib/libc/crt0.o ./external_apps/crt0.o',
        'cd ./external_apps/',
        'make ./external_apps/',
        'cd ..'
    ]

    for command in commands:
        terminate_if_error(os.system(command))

def boot_library_create():

    folder_path = './boot/boot_libs'
    o_files = []

    for filename in os.listdir(folder_path):
        if filename.endswith(".o"):
            o_files.append(os.path.join(folder_path, filename))

    if './boot/boot_libs/boot_lib.o' in o_files:
        o_files.remove('./boot/boot_libs/boot_lib.o')

    linker_command = f"{args.linker} {' '.join(o_files)} -r -o ./boot/boot_libs/boot_lib.o"
    print(linker_command)

    terminate_if_error(os.system(linker_command))


def compile_boot2():

    boot_library_create()

    commands = [
        f"{builders['c']} -O0 -masm=intel -Werror -Wimplicit-function-declaration -nostdlib -Ttext 0xA00000 -I ./ ./boot/boot2.c ./boot/boot_libs/boot_lib.o -o ./boot/boot2.elf",
        'python3 ./utils/align_file.py -f ./boot/boot2.elf -size 7168',
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
        f"{builders['c']} {builder_options['c']['kernel']} {(' -g ' if args.dwarf else '')} ./sys/kernel.c {final_string} -o ./kernel.bin",
        'cat ./programs/power/shutdown.bin ./lib/libc/real_mode_fswitch_asm ./lib/libc/fast_return_to_32_mode > ./programs/xanin_external_apps',
        'dd if=./programs/xanin_external_apps of=./programs/xanin_apps_space bs=512 count=16 conv=notrunc',
        'cat ./boot/boot ./lib/libc/enter_real_mode ./programs/xanin_apps_space ./programs/blank_sector ./fs/xin_pointers ./fs/entries_table ./boot/kernel_loader.bin ./boot/boot2.elf kernel.bin > xanin.bin',
        'dd if=xanin.bin of=xanin.img',
        'python3 ./utils/align_file.py -f ./xanin.img -size 600000',
        'mv xanin.img -f ../bin',
        'mv xanin.bin -f ../bin'
    ]

    if(args.preinstall == 'yes'):
        # commands.append('make -C ./external_apps')
        commands.append('python3 ./utils/app_preinstall.py -files external_apps/ etc/ -image ../bin/xanin.img')
        # commands.append('python3 ./utils/app_preinstall.py -files etc/ -image ../bin/xanin.img')
    
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
parser.add_argument('--dwarf', action='store_true', default=False)

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
        'default': '-O0 -Werror -Wall -Wno-deprecated-declarations -Wno-unused-but-set-variable -Wno-unused-variable -Wno-discarded-qualifiers -Wno-parentheses -Wno-comment -Wno-address-of-packed-member -Wno-maybe-uninitialized -Wno-pointer-sign -Wno-div-by-zero -Wno-duplicate-decl-specifier -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -Werror=return-type -I ./ -c',
        'kernel': '-O0 -Wall -Werror -Wno-discarded-qualifiers -Wno-parentheses -Wno-comment -Wno-address-of-packed-member -Wno-maybe-uninitialized -Wno-pointer-sign -Wno-div-by-zero -Wno-duplicate-decl-specifier -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding  -Wno-unused-function -Wno-div-by-zero -I ./'
    },

    'cc':{
        'default': '-O0 -fno-exceptions -masm=intel -std=c++17 -Wno-return-type -Wno-builtin-declaration-mismatch -nostdlib -Wno-unused-function -Wno-write-strings -fno-rtti -fconcepts-ts -I ./ -c'
    }
}

objects_to_compile = {
    
    'boot': [
        CompileObject('./boot/boot.asm', builders['asm'], builder_options['asm']['bin'], BINARY),
        CompileObject('./boot/kernel_loader.asm', builders['asm'], builder_options['asm']['bin'], BINARY),

        CompileObject('./boot/boot_libs/elf.asm', builders['asm'], builder_options['asm']['elf32'], './boot/boot_libs/elf_asm.o'),
        CompileObject('./boot/boot_libs/bootio.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./boot/boot_libs/disk.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./boot/boot_libs/elf.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./boot/boot_libs/string.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],

    'XaninOS initialization': [
        CompileObject('./sys/kernel_start.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/init/kernel_init.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],

    'filesystem': [
        CompileObject('./fs/xin_pointers.asm', builders['asm'], builder_options['asm']['bin'], BINARY),
        CompileObject('./fs/entries_table.asm', builders['asm'], builder_options['asm']['bin'], BINARY),
        CompileObject('./fs/xin.c', builders['c'], builder_options['c']['default'], OBJECT),
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
        CompileObject('./sys/devices/pci/pci.c', builders['c'], builder_options['c']['default'], OBJECT),
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
        CompileObject('./sys/call/xanin_sys/calls/input/input.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
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
        CompileObject('./sys/pmmngr/alloc.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/input/input.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/log/syslog.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/lock/lock.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/storage/storage.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
    ],

    'xanin_sys': [
        CompileObject('./sys/call/xanin_sys/calls/pmmngr/alloc.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],

    'compiler': [
        CompileObject('./compiler/files/crt0.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./compiler/files/crti.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./compiler/files/crtn.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
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
        CompileObject('./lib/libc/canvas.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./lib/libc/hash.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],

    'libcpp': [
        CompileObject('./lib/libcpp/command_parser.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        # CompileObject('./lib/libcpp/icxxabi.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libcpp/regex.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libcpp/istream.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libcpp/ostream.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libcpp/string.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libcpp/time.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        # CompileObject('./lib/libcpp/bytes.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libcpp/endian.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libcpp/hash.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
    ],

    'vty': [
        CompileObject('./lib/screen/screen.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/terminal/frontend/frontend.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/terminal/backend/backend.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/terminal/handlers/handlers.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],

    'tui': [
        CompileObject('./lib/tui/tui.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],

    'Xanin elf library': [
        CompileObject('./lib/elf/elf.c', builders['c'], builder_options['c']['default'], OBJECT),
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
        CompileObject('./programs/fs/edit.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/xagames/xagame_test.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./programs/xagames/tetris.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./programs/misc/screenshot.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./programs/tests/cpp_test.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),

        CompileObject('./programs/stdio/stdio_apply.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/file_format_tools/bmp_info.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/developer/dev_tools.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/network/netplan_apply.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/network/netapi_check.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/usb/usb_controller_info.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/tests/memory_allocator_test.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/misc/grapher/grapher.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/fs/explorer.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/tests/interrupt_test.c', builders['c'], builder_options['c']['default'], OBJECT),

        CompileObject('./programs/internals/idt_examine.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/internals/load_file.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/misc/help.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/internals/print_to_syslog.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/network/nic_info.c', builders['c'], builder_options['c']['default'], OBJECT),

        CompileObject('./programs/fs/xin_xpaint.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/fs/pwd.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/gyn_cl/gyn.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/tests/xgl_test.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/network/arp_table_print.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/network/ping.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/network/ip_test.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/network/udp_test.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/network/arp_check.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/network/dhcp_test.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/network/nic_rename.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/network/nic_print.c', builders['c'], builder_options['c']['default'], OBJECT),

        CompileObject('./programs/xgl/xgl_mode_set.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/internals/buffers.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/misc/tetris/tetris.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/misc/start_screen.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/tests/timer_test.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/fs/cat.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/misc/zsk.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/misc/epilepsy.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/fs/hexeditor/hexeditor.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/internals/cpu_info.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/misc/calc.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/fs/xin_paint.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/internals/register_dump.c', builders['c'], builder_options['c']['default'], OBJECT),

        CompileObject('./programs/internals/load.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/internals/loadch.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/developer/disk_load.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/developer/disk_write.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/misc/pong/pong.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/power/reboot.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/power/shutdown.c', builders['c'], builder_options['c']['default'], OBJECT),

        CompileObject('./programs/fs/copy_file.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/fs/create_file.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/fs/create_folder.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/fs/entry_remove.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/fs/folder_change.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/fs/folder_remove.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/fs/move.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/fs/link_create.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/fs/xin_info.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/fs/xin_note.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/fs/list_files.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/misc/logo.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/misc/ssaver.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],

    'built-in 16bit programs': [
        CompileObject('./programs/power/shutdown.asm', builders['asm'], builder_options['asm']['bin'], BINARY),
    ],
    
    '16bit mode': [
        CompileObject('./sys/real_mode/enter_real_mode.asm', builders['asm'], builder_options['asm']['bin'], BINARY),
        CompileObject('./sys/real_mode/fast_return_to_32_mode.asm', builders['asm'], builder_options['asm']['bin'], BINARY),
    ],

    'XaninOS program loaders': [
        CompileObject('./fs/loaders/bin/bit32/execute_addr.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./fs/loaders/bin/bit32/run.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./fs/loaders/bin/bit16/run16.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./fs/loaders/elf/elf_loader.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./fs/loaders/elf/elfdump.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],

    'Xanin initialization': [
        # CompileObject('./sys/initialization/init.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/initialization/init.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
    ],
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
    
create_c_library('./lib/libc/libc.o', './lib/libc/libc.a', [obj.output_name for obj in objects_to_compile['libc']] + [
        './sys/log/syslog.o', 
        './lib/screen/screen.o', 
        './sys/devices/hda/disk.o', 
        './fs/xin.o', './sys/call/xanin_sys/calls/devices/disk.o', './sys/call/xanin_sys/calls/stdio/stdio.o', 
        './sys/call/xanin_sys/calls/terminal/terminal.o', 
        './sys/call/xanin_sys/calls/vga/vga.o', 
        './sys/call/xanin_sys/calls/input/input.o', 
                ])

# create_kernel_c_library('./lib/libc/kernel_libc.o', './lib/libc/kernel_libc.a', [obj.output_name for obj in objects_to_compile['libc']] + [
#         './sys/log/syslog.o',
#         './lib/screen/screen.o', 
#         './sys/devices/hda/disk.o', 
#         './fs/xin.o', './sys/call/xanin_sys/calls/devices/disk.o', './sys/call/xanin_sys/calls/stdio/stdio.o', 
#         './sys/call/xanin_sys/calls/terminal/terminal.o', 
#         './sys/call/xanin_sys/calls/vga/vga.o', 
#         './sys/call/xanin_sys/calls/input/input.o', 
#     ])

compile_boot2()
compile_kernel(objects_to_compile)

print(colored('\nXANIN OS IMAGE BUILDED\n', 'green'))
