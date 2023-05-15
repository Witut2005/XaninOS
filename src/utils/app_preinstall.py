
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

file = open(str(args.image), 'rb+')
tmp = file.read()

file.seek(0)

xin_filesystem_pointers_begin = int(SECTOR_SIZE * 0x12)

xin_filesystem_pointers = int(SECTOR_SIZE * 0x12) # must be higher the begin
xin_filesystem_entries = int(SECTOR_SIZE * 0x1A)

NAME = 0,
TYPE = 1,
FIRST_SECTOR = 2,
SECTORS_OCCUPIED = 3

xanin_entries_to_preinstall = [
    ['/',                           XIN_DIRECTORY, 0, 0],
    ['/ivt',                        XIN_FILE, 0, 3],
    ['/file_system.bin',            XIN_FILE, 4, 20],
    ['/enter_real_mode.bin',        XIN_FILE, 1, 1],
    ['/boot.bin',                   XIN_FILE, 0, 1],
    ['/shutdown.bin',               XIN_FILE, 2, 1],
    ['/fast_real_mode_enter.bin',   XIN_FILE, 5, 1],
    ['/fast_read_mode_return.bin',  XIN_FILE, 6, 1],
    ['/screenshot/',                XIN_DIRECTORY, 0, 0] 
]

for entry in xanin_entries_to_preinstall:

    if entry[1] != XIN_DIRECTORY:
        file.seek(xin_filesystem_pointers + entry[2])
        for sectors_occupied in range(0, entry[3] - 1):
            file.write(XIN_ALLOCATED)
        file.write(XIN_EOF)
        xin_filesystem_pointers += entry[3]

    file.seek(xin_filesystem_entries)
    file.write(bytes(entry[0], 'ascii'))
    file.seek(xin_filesystem_entries + 38) #entry path
    file.write(bytes(entry[1], 'ascii'))
    file.write(bytes(13))
    file.write((entry[2] * SECTOR_SIZE).to_bytes(4, 'little')) #entry size
    file.write((entry[3]).to_bytes(4, 'little')) #first sector
    xin_filesystem_entries += XIN_ENTRY_SIZE

file.close()

# print('EXTERNAL APPS LOCATION: ', hex(xin_filesystem_entries), ' ', hex(xin_filesystem_pointers))

# directories = set() 

# print('\n\n\n--------------------------------')
# print('XIN FILESYSTEM PREINSTALL PHARSE')

# for current_file in args.files:
#     try: 
#         aha = open(current_file, 'rb')
#     except FileNotFoundError:
#         print(f'{Fore.RED}File not found:{Style.RESET_ALL} {current_file}')
#         if args.errors:
#             sys.exit()
#         print('Skipping...')
#         continue

#     except IsADirectoryError:
#         print('FOLDER DETECTED') 
#         for path, dirs, files in os.walk(current_file):
#             #print(path)
#             if(path[-1] != '/'):
#                 path = path + '/'
#             directories.add(path)
#             for f in files:
#                 print(path + f)
#                 args.files.append(path + f)
#             for d in dirs:
#                 directories.add(path + d + '/')
#         continue

#     data = aha.read()
    
#     if(xin_filesystem_pointers == 0):
#         print('ABORT')

    
#     file.seek(xin_filesystem_pointers)
#     x = 0
#     file_lenght = int(len(data) / SECTOR_SIZE)
#     if(len(data) % 512 != 0):
#         file_lenght += 1 
    
#     print(str(current_file).ljust(40, ' '), file_lenght, 'sectors')
#     if(file_lenght == 0):
#         file_lenght += 1

#     while x < file_lenght-1:
#         file.write(XIN_ALLOCATED)
#         x += 1

#     file.write(XIN_EOF)
    
#     file.seek(xin_filesystem_entries)
#     file.write(bytes('/' + current_file, 'ascii'))
#     file.seek(xin_filesystem_entries + 38) #entry path
#     file.write(bytes(XIN_FILE, 'ascii'))
#     file.write(bytes(13))
#     file.write(len(data).to_bytes(4, 'little')) #entry size
#     file.write((xin_filesystem_pointers - xin_filesystem_pointers_begin).to_bytes(4, 'little')) #first sector 
    
#     file.seek(SECTOR_SIZE * (xin_filesystem_pointers - xin_filesystem_pointers_begin)) #write data to image

#     if(file.tell() == 0):
#         print('ABORT')
    
#     file.write(data)

#     xin_filesystem_entries += XIN_ENTRY_SIZE
#     xin_filesystem_pointers += file_lenght

#     aha.close()

    
# print('--------------------------------')
# for d in directories:
#     file.seek(xin_filesystem_entries)
#     file.write(bytes('/' + d, 'ascii'))
#     file.seek(xin_filesystem_entries + 38)
#     file.write(bytes(XIN_DIRECTORY, 'ascii'))
#     file.write(bytes(13))
#     file.write(bytes(4))
#     file.write(bytes(4))
#     xin_filesystem_entries += XIN_ENTRY_SIZE