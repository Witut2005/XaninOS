
import argparse
from colorama import Fore, init, Style
import sys
from datetime import datetime

from macros import * 
from app_preinstall_functions import *

init(autoreset=True)

args = argparse.ArgumentParser()
args.add_argument('--image', action='store', type=str, required=True)
args.add_argument('--files', action='store', type=str, required=True, nargs='+')
args.add_argument('--errors', action='store_true')

args = args.parse_args()

xin_current_date = decimal_to_bcd(datetime.now().strftime('%d%m%Y'))
xin_current_time = decimal_to_bcd(datetime.now().strftime('%H%M'))

ENTRY_PRINT_LJUST_PADDING = 35 + XIN_MAX_ENTRY_PATH_LENGTH
ENTRY_PRINT_TOP_BOTTOM_PADDING = 97

class XinEntryData: 

    image_size_in_sectors = None
    xin_pointers_begin = None
    xin_entries_begin = None
    image_data = None

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

    def find_free_sectors_for_given_size(self, size): 

        if size >= len(XinEntryData.image_data):
            XinEntryData.image_data = pad_bytesarray(XinEntryData.image_data, size)

        size_in_sectors = size_to_sectors(size)

        for i in range (XinEntryData.xin_pointers_begin, XinEntryData.xin_entries_begin):
            if XinEntryData.image_data[i] == XIN_UNALLOCATED:
                ok = True
                for j in range(size_in_sectors):
                    if XinEntryData.image_data[i+j] != XIN_UNALLOCATED:
                        ok = False
                        break
                
                if ok:
                    return i

        print(f'{Fore.RED}FREE SECTOR NOT FOUND')
        return 0
        
    def xin_data_set(image_data, image_size_in_sectors, xin_pointers_begin, xin_entries_begin):
        XinEntryData.image_data = image_data 
        XinEntryData.image_size_in_sectors = image_size_in_sectors
        XinEntryData.xin_pointers_begin = xin_pointers_begin
        XinEntryData.xin_entries_begin  = xin_entries_begin 

    def insert(self, data, index):
        if (index + len(data)) >= len(XinEntryData.image_data):
            XinEntryData.image_data = pad_bytesarray(XinEntryData.image_data, index + len(data))
            for i in range(len(data)):
                XinEntryData.image_data[index + i] = data[i]
        else:
            for i in range(len(data)):
                XinEntryData.image_data[index + i] = data[i]
    
    def print_entry_info(self):
        print('| Path:              ', self.path.replace(b'\x00', b'').decode('ascii').ljust(ENTRY_PRINT_LJUST_PADDING),            '|')
        print('| Type:              ', self.type.decode('ascii').ljust(ENTRY_PRINT_LJUST_PADDING),                                             '|')
        print('| Creation Date:     ', convert_xin_date_to_str(self.creation_date).ljust(ENTRY_PRINT_LJUST_PADDING),                '|')
        print('| Creation Time:     ', convert_xin_time_to_str(self.creation_time).ljust(ENTRY_PRINT_LJUST_PADDING),                '|')
        print('| Modification Date: ', convert_xin_date_to_str(self.modification_date).ljust(ENTRY_PRINT_LJUST_PADDING),            '|')
        print('| Modification Time: ', convert_xin_time_to_str(self.modification_time).ljust(ENTRY_PRINT_LJUST_PADDING),            '|')
        print('| Permissions:       ', str(int.from_bytes(self.permissions, byteorder='little')).ljust(ENTRY_PRINT_LJUST_PADDING),  '|')        
        print('| Size:              ', str(self.size).ljust(ENTRY_PRINT_LJUST_PADDING),                                             '|')
        print('| First Sector:      ', str(self.first_sector).ljust(ENTRY_PRINT_LJUST_PADDING),                                     '|')
        print(''.ljust(ENTRY_PRINT_TOP_BOTTOM_PADDING, '-'))

    def write(self, data=None):

        file_first_sector = 0

        if data != None:
            self.size = len(data)

        if self.type != XIN_DIRECTORY:
            try:
                file_first_sector = XinEntryData.xin_pointers_begin + self.first_sector
            except TypeError:
                file_first_sector = XinEntryData.xin_pointers_begin + (0 if data == None else self.find_free_sectors_for_given_size(len(data)))

            xin_pointers_cursor = file_first_sector 

            self.insert(bytes([XIN_ALLOCATED] * (size_to_sectors(self.size) - 1)), xin_pointers_cursor)
            xin_pointers_cursor += size_to_sectors(self.size) - 1

            self.insert(bytes([XIN_EOF]), xin_pointers_cursor)


        xin_entries_cursor = XinEntryData.xin_entries_begin + (XinEntryData.xin_entries_index * XIN_ENTRY_SIZE)

        self.insert(self.path,               xin_entries_cursor + XIN_PATH_OFFSET)
        self.insert(self.type,               xin_entries_cursor + XIN_TYPE_OFFSET)
        self.insert(self.creation_date,      xin_entries_cursor + XIN_CREATION_DATE_OFFSET)
        self.insert(self.creation_time,      xin_entries_cursor + XIN_CREATION_TIME_OFFSET)
        self.insert(self.modification_date,  xin_entries_cursor + XIN_MODIFICATION_DATE_OFFSET)
        self.insert(self.modification_time,  xin_entries_cursor + XIN_MODIFICATION_TIME_OFFSET)
        self.insert(self.permissions,        xin_entries_cursor + XIN_PERMISSIONS_OFFSET)

        if(data == None):
            self.insert((0 if self.size == None else self.size).to_bytes(4, byteorder='little'),                   xin_entries_cursor + XIN_SIZE_OFFSET)
            self.insert((0 if self.first_sector == None else self.first_sector).to_bytes(4, byteorder='little'),   xin_entries_cursor + XIN_FIRST_SECTOR_OFFSET)

        else:
            self.first_sector = file_first_sector
            self.insert(len(data).to_bytes(4, byteorder='little'),                                                 xin_entries_cursor + XIN_SIZE_OFFSET)
            self.insert(file_first_sector.to_bytes(4, byteorder='little'),                                         xin_entries_cursor + XIN_FIRST_SECTOR_OFFSET)


        self.insert(int(0).to_bytes(4, 'little'), xin_entries_cursor + XIN_FILE_INFO_OFFSET) #File Info ptr

        if data != None:
            self.insert(pad_bytes(data, SECTOR_SIZE), self.first_sector * SECTOR_SIZE)
        
        self.print_entry_info()

        XinEntryData.xin_entries_index += 1
    
def preinstall(image_size_in_sectors): 

    xin_image = open('xinFs.tmp', 'wb+')
    data = bytearray()

    xin_pointers_begin = 0
    xin_entries_begin = xin_pointers_begin + (SECTOR_SIZE * 6)

    print(f'\nXaninOS image sectors: {image_size_in_sectors}')
    print(f'Xin pointers offset: {hex(xin_pointers_begin)}')
    print(f'Xin entries offset: {hex(xin_entries_begin)}\n')

    XinEntryData.xin_data_set(data, image_size_in_sectors, xin_pointers_begin, xin_entries_begin)

    xin_default_entries_to_preinstall = [
        XinEntryData('/',                           XIN_DIRECTORY,  XIN_MAX_PERMISSIONS),
        XinEntryData('/screenshot/',                XIN_DIRECTORY,  XIN_MAX_PERMISSIONS),
        XinEntryData('/kernel',                     XIN_FILE,       XIN_MAX_PERMISSIONS, SECTOR_SIZE * 800, 0), # okolo 400KB
        XinEntryData('/fast_real_mode_enter.bin',   XIN_FILE,       XIN_MAX_PERMISSIONS, SECTOR_SIZE,       5), 
        XinEntryData('/boot.bin',                   XIN_FILE,       XIN_MAX_PERMISSIONS, SECTOR_SIZE,       0),
        XinEntryData('/fast_real_mode_return.bin',  XIN_FILE,       XIN_MAX_PERMISSIONS, SECTOR_SIZE,       6),
        XinEntryData('/enter_real_mode.bin',        XIN_FILE,       XIN_MAX_PERMISSIONS, SECTOR_SIZE * 20,  1),
        XinEntryData('/shutdown.bin',               XIN_FILE,       XIN_MAX_PERMISSIONS, SECTOR_SIZE * 2,   1),
        XinEntryData('/ivt',                        XIN_FILE,       XIN_MAX_PERMISSIONS, SECTOR_SIZE * 2,   1000)
    ]

    print(f'{Fore.GREEN}\n\nXIN INTERNAL ENTRIES PREINSTALL PHARSE')
    print(''.ljust(ENTRY_PRINT_TOP_BOTTOM_PADDING, '-'))

    for xin_entry in xin_default_entries_to_preinstall:
        xin_entry.write()
    
    directory_entires = set() 
    file_entires = set()

    print(f'{Fore.GREEN}\n\nXIN EXTERNAL ENTRIES PREINSTALL PHARSE')
    print(''.ljust(ENTRY_PRINT_TOP_BOTTOM_PADDING, '-'))


    for current_file in args.files:
        try: 
            if os.path.isfile(current_file):
                files.add(current_file)
            elif os.path.isdir(current_file):
                # print('FOLDER DETECTED') 
                for path, dirs, files in os.walk(current_file):
                    if(path[-1] != '/'):
                        path = path + '/'
                    directory_entires.add(path)
                    for f in files:
                        # print(path + f)
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
        
    for f in file_entires:
        entry  = XinEntryData(f[1:], XIN_FILE)
        entry.write(open(f, 'rb').read())

    for d in directory_entires:
        entry = XinEntryData(d[1:], XIN_DIRECTORY)
        entry.write()

    # write changes to XaninOS image
    xin_image.write(XinEntryData.image_data)
    xin_image.close()

    os.system(f'cat {args.image} xinFs.tmp > xanin.img')

def main(args):
    # os.system(f'dd if=/dev/zero of={os.path.abspath(args.image)} bs=10K count=1')
    print(f"\nPath: {os.path.abspath(args.image)}")

    args.image = os.path.abspath(args.image)
    kernel_image = open(args.image, 'rb+')

    align_file_to_size(kernel_image, SECTOR_SIZE)
    kernel_image.seek(XIN_FS_BOOT_OFFSET)
    kernel_image.write(os.path.getsize(args.image).to_bytes(4, 'little'))

    kernel_image.flush()
    preinstall(size_to_sectors(os.path.getsize(os.path.abspath(args.image))))

if __name__ == '__main__':
    main(args)