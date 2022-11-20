
import numpy as np
from detect_xin_ext import *
import argparse
from colorama import Fore 
from colorama import Style
import sys
import os 

XIN_ENTRY_SIZE = 64
XIN_ALLOCATED = b'\x01'
XIN_EOF = b'\xff'
XIN_FILE = 'F'
XIN_DIRECTORY = 'D'
XIN_MAX_PERMISSIONS = b'\xFF'
FAIL = '\033[91m'

args = argparse.ArgumentParser()
args.add_argument('-image', action='store', type=str, required=True)
args.add_argument('-files', action='store', type=str, required=True, nargs='+')
args.add_argument('-errors', action='store_true')
args = args.parse_args()


xin_extended_table = detect_xin_ext(args.image)
print(hex(xin_extended_table))
file = open(str(args.image), 'rb+')
xin_filesystem_pointers_begin = int(SECTOR_SIZE * 18)
xin_filesystem_pointers = int(SECTOR_SIZE * 18 + 0x180)
xin_filesystem_entries = int(SECTOR_SIZE * 26 + 0xB40 + (64 * 7))
# file.seek(SECTOR_SIZE * 18 + 0xB00)

print('\n\n\n--------------------------------')
print('XIN FILESYSTEM PREINSTALL PHARSE')

for current_file in args.files:
    try: 
        aha = open(current_file, 'rb')
    except FileNotFoundError:
        print(f'{Fore.RED}File not found:{Style.RESET_ALL} {current_file}')
        if args.errors:
            sys.exit()
        print('Skipping...')
        continue

    except IsADirectoryError:
        print('FOLDER DETECTED') 
        for path, dirs, files in os.walk(current_file):
            #print(path)
            if(path[-1] != '/'):
                path = path + '/'
            for f in files:
                print(path + f)
                args.files.append(path + f)

        file.seek(xin_filesystem_entries)
        file.write(bytes('/' + current_file, 'ascii'))
        file.seek(xin_filesystem_entries + 38)
        file.write(bytes(XIN_DIRECTORY, 'ascii'))
        file.write(bytes(13))
        file.write(bytes(4))
        file.write(bytes(4))

        xin_filesystem_entries += XIN_ENTRY_SIZE
        continue

    data = aha.read()
    
    file.seek(xin_filesystem_pointers)
    x = 0
    file_lenght = int(len(data) / SECTOR_SIZE)
    if(len(data) % 512 != 0):
        file_lenght += 1 
    print('file lenght(in sectors):', file_lenght)
    if(file_lenght == 0):
        file_lenght += 1

    while x < file_lenght-1:
        file.write(XIN_ALLOCATED)
        x += 1

    file.write(XIN_EOF)
    
    file.seek(xin_filesystem_entries)
    file.write(bytes('/' + current_file, 'ascii'))
    file.seek(xin_filesystem_entries + 38)
    file.write(bytes(XIN_FILE, 'ascii'))
    file.write(bytes(13))
    file.write(file_lenght.to_bytes(4, 'little'))
    file.write((xin_filesystem_pointers - xin_filesystem_pointers_begin).to_bytes(4, 'little'))
    
    file.seek(SECTOR_SIZE * (xin_filesystem_pointers - xin_filesystem_pointers_begin))
    file.write(data)

    xin_filesystem_entries += XIN_ENTRY_SIZE
    xin_filesystem_pointers += file_lenght

    aha.close()

print('--------------------------------')