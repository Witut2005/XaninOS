
import argparse
from colorama import Fore 
from colorama import Style
import sys
from datetime import datetime

from macros import * 
from app_preinstall_functions import *


args = argparse.ArgumentParser()
args.add_argument('--image', action='store', type=str, required=True)
args.add_argument('--files', action='store', type=str, required=True, nargs='+')
# args.add_argument('--errors', action='store_true')

args = args.parse_args()

xin_current_date = decimal_to_bcd(datetime.now().strftime('%d%m%Y'))
xin_current_time = decimal_to_bcd(datetime.now().strftime('%H%M'))

print(xin_current_date)

class XinEntryData: 

    xin_pointers_begin = None
    xin_entries_begin = None
    os_image = None

    #tmp data
    xin_entries_index = 0

    def __init__(self, path, type, permissions=XIN_MAX_PERMISSIONS, size=None, first_sector=None):
        self.path = convert_string_to_aligned_bytes(path, XIN_MAX_ENTRY_PATH_LENGTH)
        self.type = type
        self.creation_date = xin_current_date 
        self.creation_time = xin_current_time 
        self.modification_date = self.creation_date
        self.modification_time = self.creation_time
        self.permissions = permissions
        self.size = size
        self.first_sector = first_sector
        
    def xin_data_set(os_image, xin_pointers_begin, xin_entries_begin):
        XinEntryData.os_image = os_image
        XinEntryData.xin_pointers_begin = xin_pointers_begin
        XinEntryData.xin_entries_begin  = xin_entries_begin 

    # def xin_data_write(self, data):
    #     XinEntryData.os_image.seek(self.first_sector * SECTOR_SIZE)
    #     XinEntryData.os_image.write(data)

    def write(self, data=None):

        # if self.type != XIN_DIRECTORY:
        #     XinEntryData.os_image.seek(XinEntryData.xin_pointers_begin + self.first_sector)
        #     for i in range(size_to_sectors(self.size) - 1):
        #         XinEntryData.os_image.write(XIN_ALLOCATED)
        #     XinEntryData.os_image.write(XIN_EOF)

        XinEntryData.os_image.seek(XinEntryData.xin_entries_begin + (XinEntryData.xin_entries_index * XIN_ENTRY_SIZE))

        XinEntryData.os_image.write(self.path)
        XinEntryData.os_image.write(self.type)
        XinEntryData.os_image.write(self.creation_date)
        XinEntryData.os_image.write(self.creation_time)
        XinEntryData.os_image.write(self.modification_date)
        XinEntryData.os_image.write(self.modification_time)

        if(data == None):
            XinEntryData.os_image.write((0 if self.size == None else self.size).to_bytes(4, byteorder='little'))
            XinEntryData.os_image.write((0 if self.first_sector == None else self.first_sector).to_bytes(4, byteorder='little'))
        else:
            XinEntryData.os_image.write(len(data).to_bytes(4, byteorder='little'))
            XinEntryData.os_image.write((find_free_sectors_for_given_size(len(data))).to_bytes(4, byteorder='little'))


        XinEntryData.os_image.write(int(0).to_bytes(4, 'little')) #File Info ptr

        XinEntryData.xin_entries_index += 1

def preinstall(file): 

    xin_pointers_begin = os.path.getsize(file.name)
    xin_entries_begin = 0

    XinEntryData.xin_data_set(file, xin_pointers_begin, xin_entries_begin)

    xin_default_entries_to_preinstall = [
        XinEntryData('/',                           XIN_DIRECTORY),
        XinEntryData('/file_system.bin',            XIN_DIRECTORY),
        XinEntryData('/enter_real_mode.bin',        XIN_FILE, 0x12, 20),
        XinEntryData('/boot.bin',                   XIN_FILE, 1, 1),
        XinEntryData('/shutdown.bin',               XIN_FILE, 1, 2),
        XinEntryData('/fast_real_mode_enter.bin',   XIN_FILE, 5, 1), 
        XinEntryData('/fast_real_mode_return.bin',  XIN_FILE, 6, 1),
        XinEntryData('/screenshot/',                XIN_DIRECTORY),
        XinEntryData('/ivt',                        XIN_FILE, 1000, 2)
    ]

    for xin_entry in xin_default_entries_to_preinstall:
        xin_entry.write()

    directory_entires = set() 
    file_entires = set()

    print('\n\n\n--------------------------------')
    print('XIN FILESYSTEM PREINSTALL PHARSE')

    for current_file in args.files:
        try: 
            if os.path.isfile(current_file):
                files.add(current_file)
            elif os.path.isdir(current_file):
                print('FOLDER DETECTED') 
                for path, dirs, files in os.walk(current_file):
                    #print(path)
                    if(path[-1] != '/'):
                        path = path + '/'
                    directory_entires.add(path)
                    for f in files:
                        print(path + f)
                        file_entires.add(path + f)
                    for d in dirs:
                        directory_entires.add(path + d + '/')
                continue

        except FileNotFoundError:
            print(f'{Fore.RED}File not found:{Style.RESET_ALL} {current_file}')
            if args.errors:
                sys.exit()
            print('Skipping...')
            continue 
        
    print('--------------------------------')
    print(file_entires)

    for f in file_entires:
        entry  = XinEntryData(f[1:], XIN_FILE, XIN_MAX_PERMISSIONS)
        entry.write(open(f, 'rb').read())

    for d in directory_entires:
        entry = XinEntryData(d[1:], XIN_DIRECTORY, XIN_MAX_PERMISSIONS)
        entry.write()

def main(args):
    print(os.path.abspath(args.image))
    args.image = os.path.abspath(args.image)
    image = open(args.image, 'wb+')
    # align_file_to_size(image, 16)
    preinstall(image)
    # print(image.read()[0])

if __name__ == '__main__':
    main(args)