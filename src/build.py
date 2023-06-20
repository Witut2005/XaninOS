

import os
import argparse
from colorama import init
from termcolor import colored

class CompileObject:

    def __init__(self, path, builder, options):
        self.path = path
        self.builder = builder
        self.options = options
    
    def command(self):
        return self.builder + ' ' + self.options + ' ' + self.path
        
C = 'i386-elf-gcc'
CC = 'i386-elf-g++'

C_COMPILE = '-O0 -Wall -Wno-discarded-qualifiers -Wno-parentheses -Wno-comment -Wno-address-of-packed-member -Wno-maybe-uninitialized -Wno-pointer-sign -Wno-div-by-zero -Wno-duplicate-decl-specifier -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding  -Wno-unused-function -Wno-div-by-zero -I $(XANIN_PATH)'
CC_OPTIONS = '-O0 -fno-exceptions -lstdc++ -masm=intel -std=c++17 -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -Wno-write-strings -fno-rtti -fconcepts-ts -I $(XANIN_PATH) -c'
C_OPTIONS = '-O0 -Wall -Wno-discarded-qualifiers -Wno-parentheses -Wno-comment -Wno-address-of-packed-member -Wno-maybe-uninitialized -Wno-pointer-sign -Wno-div-by-zero -Wno-duplicate-decl-specifier -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -Werror=return-type -I $(XANIN_PATH) -c'

# APPS = ./programs
# HANDLERS = ./sys/interrupts/handlers

parser = argparse.ArgumentParser()

parser.add_argument('--srcpath', type=str)
parser.add_argument('--binpath', type=str)
parser.add_argument('--preinstall', type=bool, default=True)
parser.add_argument('--assembler', type=str, default='nasm')

args = parser.parse_args()

assembler = args.assembler
if_preinstall = args.preinstall

builder_options = {
    'asm': {
        'bin': '-fbin',
        'elf32': '-f elf32'
    },

    'c':{

    },

    'cc':{

    }
}

objects_to_compile = {
    
    'boot': [
        CompileObject('./boot/boot.asm', assembler, builder_options['asm']['bin']),
    ]
    
    }

for os_module, objects in objects_to_compile.items():
    print(colored('compling {} module'.format(os_module).upper(), 'green'))
    for object in objects:
        os.system(object.command())
        print(object.path.ljust(20, ' '), end='')
        print(colored('OK', 'green'))
    