
import argparse
from colorama import Fore 
from colorama import Style
import sys
from datetime import datetime

from macros import * 
from app_preinstall_functions import *


args = argparse.ArgumentParser()
args.add_argument('--image', action='store', type=str, required=True)
# args.add_argument('--files', action='store', type=str, required=True, nargs='+')
# args.add_argument('--errors', action='store_true')

args = args.parse_args()

xin_current_date = decimal_to_bcd(datetime.now().strftime('%d%m%Y'))
xin_current_time = decimal_to_bcd(datetime.now().strftime('%H%M'))

class XinEntryData: 

    xin_pointers_begin = None
    xin_entries_begin = None
    os_image = None

    #tmp data
    xin_entries_index = 0

    def __init__(self, path, type, permissions=XIN_MAX_PERMISSIONS, size=0, first_sector=0):
        self.path = convert_string_to_aligned_bytes(path, XIN_MAX_ENTRY_PATH_LENGTH)
        self.type = type
        self.creation_date = xin_current_date 
        self.creation_time = xin_current_time 
        self.modification_date = self.creation_date
        self.modification_time = self.creation_time
        self.permissions = permissions
        self.size = size
        self.first_sector = first_sector
        # self.size = size.to_bytes(4, byteorder='little')
        # self.first_sector = size.to_bytes(4, byteorder='little')
        
    def xin_data_set(os_image, xin_pointers_begin, xin_entries_begin):
        XinEntryData.os_image = os_image
        XinEntryData.xin_pointers_begin = xin_pointers_begin
        XinEntryData.xin_entries_begin  = xin_entries_begin 

    def write(self):

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
        XinEntryData.os_image.write(self.size.to_bytes(4, byteorder='little'))
        XinEntryData.os_image.write(self.first_sector.to_bytes(4, byteorder='little'))
        XinEntryData.os_image.write(int(0).to_bytes(4, 'little')) #File Info ptr

        XinEntryData.xin_entries_index += 1



def preinstall(file): 

    # xin_pointers_begin = os.path.getsize(file.name)
    # xin_entries_begin = xin_pointers_begin + (SECTOR_SIZE * 512)

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

    return

    directories = set() 

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
                directories.add(path)
                for f in files:
                    print(path + f)
                    args.files.append(path + f)
                for d in dirs:
                    directories.add(path + d + '/')
            continue

        data = aha.read()
        
        file.seek(xin_preinstalled_objects_cursor)
        x = 0
        file_lenght = int(len(data) / SECTOR_SIZE)
        if(len(data) % 512 != 0):
            file_lenght += 1 
        
        print(str(current_file).ljust(40, ' '), file_lenght, 'sectors')
        if(file_lenght == 0):
            file_lenght += 1

        while x < file_lenght-1:
            file.write(XIN_ALLOCATED)
            x += 1

        file.write(XIN_EOF)
        
        file.seek(xin_entries_begin)
        file.write(bytes('/' + current_file, 'ascii'))
        file.seek(xin_filesystem_entries + 38) #entry path
        file.write(bytes(XIN_FILE, 'ascii'))
        file.write(bytes(13))
        file.write(len(data).to_bytes(4, 'little')) #entry size
        file.write((xin_filesystem_pointers - xin_filesystem_pointers_begin).to_bytes(4, 'little')) #first sector 
        
        file.seek(SECTOR_SIZE * (xin_filesystem_pointers - xin_filesystem_pointers_begin)) #write data to image

        if(file.tell() == 0):
            print('ABORT')
        
        file.write(data)

        xin_filesystem_entries += XIN_ENTRY_SIZE
        xin_filesystem_pointers += file_lenght

        aha.close()

        
    print('--------------------------------')
    for d in directories:
        file.seek(xin_filesystem_entries)
        file.write(bytes('/' + d, 'ascii'))
        file.seek(xin_filesystem_entries + 38)
        file.write(bytes(XIN_DIRECTORY, 'ascii'))
        file.write(bytes(13))
        file.write(bytes(4))
        file.write(bytes(4))
        xin_filesystem_entries += XIN_ENTRY_SIZE

def main(args):
    print(os.path.abspath(args.image))
    args.image = os.path.abspath(args.image)
    image = open(args.image, 'rb+')
    # align_file_to_size(image, 16)
    preinstall(image)
    # print(image.read()[0])

if __name__ == '__main__':
    main(args)