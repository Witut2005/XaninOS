

import os
import argparse
import sys
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

    def output_modifcation_time(self):
        return os.path.getmtime(self.output_name)

    def source_modifcation_time(self):
        return os.path.getmtime(self.path)

    def needs_to_be_recompiled(self):
        try:
            return self.source_modifcation_time() >= self.output_modifcation_time() 
        except:
            return True
    
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
        f"{builders['c']} -O0 -masm=intel -Wimplicit-function-declaration -nostdlib -Ttext 0xA00000 -I ./ ./boot/boot2.c ./boot/boot_libs/boot_lib.o -o ./boot/boot2.elf",
        'python3 ./build/align_file.py -f ./boot/boot2.elf -size 7168',
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
        f'{builders["c"]} {builder_options["c"]["kernel"]} {("-g" if args.dwarf else "")} ./sys/kernel.c {final_string} -o ./kernel.bin',
        'cat ./programs/power/shutdown.bin ./lib/libc/real_mode_fswitch_asm ./lib/libc/fast_return_to_32_mode > ./programs/xanin_external_apps',
        'dd if=./programs/xanin_external_apps of=./programs/xanin_apps_space bs=512 count=16 conv=notrunc',
        'cat ./boot/boot.bin ./lib/libc/enter_real_mode ./programs/xanin_apps_space ./programs/blank_sector ./fs/xin_pointers ./fs/entries_table ./boot/kernel_loader.bin ./boot/boot2.elf kernel.bin > xanin.bin',
        'dd if=xanin.bin of=xanin.img',
        'python3 ./build/align_file.py -f ./xanin.img -size 400000',

        f'python3 ./build/app_preinstall2.py --files external_apps/ etc/ --image xanin.img {"--dont_print_xin_info" if args.dont_print_xin_info == True else ""}',

        'mv xanin.img -f ../bin',
        'mv xanin.bin -f ../bin'
    ]

    for command in commands:
        terminate_if_error(os.system(command))

parser = argparse.ArgumentParser()

parser.add_argument('--dont_print_xin_info', action='store_true')
parser.add_argument('--srcpath', type=str)
parser.add_argument('--binpath', type=str)

parser.add_argument('--assembler', type=str, default='nasm')
parser.add_argument('--cbuilder', type=str, default='i386-elf-gcc')
parser.add_argument('--ccbuilder', type=str, default='i386-elf-g++')
parser.add_argument('--linker', type=str, default='i386-elf-ld')
parser.add_argument('--archive', type=str, default='i386-elf-ar')
parser.add_argument('--dwarf', action='store_true', default=False)

parser.add_argument('--long', action='store_true')
parser.add_argument('--build_all', action='store_true')

args = parser.parse_args()

builders = {
    'asm': args.assembler,
    'c': args.cbuilder,
    'cc': args.ccbuilder,
}

c_compilation_options = '-O0 -Wall -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -I ./'
cc_compilation_options = '-O0 -std=c++2a -fno-exceptions -masm=intel -Wno-builtin-declaration-mismatch -Wno-unused-function -Wno-write-strings -fno-rtti -fconcepts-ts -nostdlib -ffreestanding -I ./ -c'

def compiler_cast_given_warning(warning):
    return f'-Werror={warning}'

def compiler_ignore_given_warning(warning):
    return f'-Wno-{warning}'

c_compiler_ignored_warnings_li = ['unused-function', 'address-of-packed-member']
c_compiler_ignored_warnings = ' '.join([compiler_ignore_given_warning(warning) for warning in c_compiler_ignored_warnings_li])

cpp_compiler_ignored_warnings_li = ['unused-function', 'address-of-packed-member', 'literal-suffix']
cpp_compiler_ignored_warnings = ' '.join([compiler_ignore_given_warning(warning) for warning in cpp_compiler_ignored_warnings_li])

c_compiler_casted_warnings_li = ['int-conversion', 'implicit-function-declaration', 'return-type']
c_compiler_casted_warnings = ' '.join([compiler_cast_given_warning(warning) for warning in c_compiler_casted_warnings_li])

cpp_compiler_casted_warnings_li = ['return-type']
cpp_compiler_casted_warnings = ' '.join([compiler_cast_given_warning(warning) for warning in cpp_compiler_casted_warnings_li])

builder_options = {
    'asm': {
        'bin': '-fbin',
        'elf32': '-f elf32'
    },

    'c':{
        'default': f"{c_compilation_options} {c_compiler_casted_warnings} {c_compiler_ignored_warnings} -c",
        'lib': f"{c_compilation_options} {c_compiler_casted_warnings} {c_compiler_ignored_warnings} -fpic -c",
        'kernel': f"{c_compilation_options} {c_compiler_casted_warnings} {c_compiler_ignored_warnings} -Ttext 0xF00000"
    },

    'cc':{
        'default': f"{cc_compilation_options} {cpp_compiler_casted_warnings} {cpp_compiler_ignored_warnings} -c",
        'lib': f"{cc_compilation_options} {cpp_compiler_casted_warnings} {cpp_compiler_ignored_warnings} -fpic -c"
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
        CompileObject('./fs/xin.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./fs/xin_extended_table.asm', builders['asm'], builder_options['asm']['bin'], BINARY),
        # CompileObject('./fs/xanin_apps_space.asm', builders['asm'], builder_options['asm']['bin'], BINARY),
    ],

    'drivers': [
        CompileObject('./sys/devices/keyboard/keyboard.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/devices/keyboard/keyboard_entry.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
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
        CompileObject('./sys/call/xanin_sys/calls/terminal/terminal.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/call/xanin_sys/calls/vga/vga.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./sys/call/xanin_sys/handler/xanin_sys.c', builders['c'], builder_options['c']['default'], OBJECT),
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
        CompileObject('./sys/net/manager/manager.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
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
        CompileObject('./sys/panic/panic.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/pmmngr/alloc.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/input/input.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/log/syslog.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/lock/lock.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./sys/paging/paging.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/paging/paging.asm', builders['asm'], builder_options['asm']['elf32'], './sys/paging/paging_asm.o'),
    ],

    'compiler': [
        CompileObject('./compiler/files/crt0.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./compiler/files/crti.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./compiler/files/crtn.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
    ],

    'libc':[
        # CompileObject('./lib/libc/real_mode_fswitch.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        # CompileObject('./lib/libc/real_mode_fswitch.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./lib/cpu/code/cpu_state_info.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
        CompileObject('./lib/libc/hal.c', builders['c'], builder_options['c']['lib'], OBJECT),
        CompileObject('./lib/libc/math.c', builders['c'], builder_options['c']['lib'], OBJECT),
        CompileObject('./lib/libc/memory.c', builders['c'], builder_options['c']['lib'], OBJECT),
        CompileObject('./lib/libc/stdiox.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./lib/libc/stdlibx.c', builders['c'], builder_options['c']['lib'], OBJECT),
        CompileObject('./lib/libc/string.cpp', builders['cc'], builder_options['cc']['lib'], OBJECT),
        CompileObject('./lib/libc/data_structures.c', builders['c'], builder_options['c']['lib'], OBJECT),
        CompileObject('./lib/libc/algorithm.c', builders['c'], builder_options['c']['lib'], OBJECT),
        CompileObject('./lib/libc/time.c', builders['c'], builder_options['c']['lib'], OBJECT),
        CompileObject('./lib/libc/process.c', builders['c'], builder_options['c']['lib'], OBJECT),
        CompileObject('./lib/libc/stdiox_legacy.c', builders['c'], builder_options['c']['lib'], OBJECT),
        CompileObject('./lib/libc/canvas.c', builders['c'], builder_options['c']['lib'], OBJECT),
        CompileObject('./lib/libc/hash.c', builders['c'], builder_options['c']['lib'], OBJECT),
    ],

    'libcpp': [
        CompileObject('./lib/libcpp/lexer.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libcpp/regex.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libcpp/istream.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libcpp/ostream.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libcpp/string.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libcpp/time.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libcpp/endian.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libcpp/hash.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
    ],

    'system': [
        CompileObject('./lib/system/system.c', builders['c'], builder_options['c']['default'], OBJECT)
    ],

    'vty': [
        CompileObject('./lib/screen/screen.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/terminal/frontend/frontend.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/terminal/backend/backend.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./sys/terminal/handlers/handlers.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],

    'shell': [
        CompileObject('./sys/terminal/interpreter/interpreter.cpp', builders['cc'], builder_options['cc']['default'], OBJECT)
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

    'Tests Cpp': [
        CompileObject('./programs/tests/cpp/array.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./programs/tests/cpp/string.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./programs/tests/cpp/lexer.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./programs/tests/cpp/xin.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./programs/tests/cpp/algo.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./programs/tests/cpp/vector.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./programs/tests/cpp/global_constructors.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
    ],

    'Tests C': [
        CompileObject('./programs/tests/c_test.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/tests/c/alloc.c', builders['c'], builder_options['c']['default'], OBJECT),
    ],

    'Drivers test': [],
    'Network test': [],
    'System utils test': [],

    'built-in programs': [
        CompileObject('./programs/fs/edit.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/fs/xin_check.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/xagames/xagame_test.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./programs/xagames/tetris.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./programs/misc/screenshot.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./programs/tests/elf_loader_test.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),

        CompileObject('./programs/stdio/stdio_apply.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/file_format_tools/bmp_info.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/network/netplan_apply.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/network/netapi_check.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/usb/usb_controller_info.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/tests/disk_func.c', builders['c'], builder_options['c']['default'], OBJECT),
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
        CompileObject('./programs/tests/sprintf_test.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/tests/paging_test.c', builders['c'], builder_options['c']['default'], OBJECT),
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
        # CompileObject('./programs/fs/xin_note.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/fs/list_files.c', builders['c'], builder_options['c']['default'], OBJECT),
        CompileObject('./programs/misc/logo.c', builders['c'], builder_options['c']['default'], OBJECT),
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
        CompileObject('./fs/loaders/elf/elf_loader.cpp', builders['cc'], builder_options['cc']['default'], OBJECT),
        CompileObject('./lib/libc/crt0.asm', builders['asm'], builder_options['asm']['elf32'], OBJECT),
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

        if object.needs_to_be_recompiled() or args.build_all: 
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
        './lib/screen/screen.o', 
        './lib/system/system.o', 
        './sys/call/xanin_sys/calls/terminal/terminal.o', 
        './sys/call/xanin_sys/calls/vga/vga.o']
                )

compile_boot2()
compile_kernel(objects_to_compile)

print(colored('\nXANIN OS IMAGE BUILDED\n', 'green'))
