
import argparse
from colorama import Fore 
from colorama import Style
import sys
import os 
from datetime import datetime
from macros import * 


args = argparse.ArgumentParser()
args.add_argument('--image', action='store', type=str, required=True)
# args.add_argument('--files', action='store', type=str, required=True, nargs='+')
# args.add_argument('--errors', action='store_true')

args = args.parse_args()

def decimal_to_bcd(number):
    ascii_digits = number.encode('ascii')
    bcd_digits = b''
    for i in range(len(ascii_digits)):
        bcd_digits += (ascii_digits[i] - 48).to_bytes(1, byteorder='little')

    return bcd_digits

def write_xin_entry_to_file(file, entry):
    return

def convert_string_to_aligned_bytes(input_string, alignment_size):
    encoded_bytes = input_string.encode('ascii')
    padding_size = alignment_size - len(encoded_bytes) % alignment_size

    aligned_bytes = encoded_bytes + b'\0' * padding_size

    return aligned_bytes


def align_file_to_size(file, size):
    file_size = os.path.getsize(file.name)
    print(f"file name: {file.name}")
    print(f"file size: {file_size}")

    tmp = file_size

    while tmp > size:
        tmp = tmp - size

    file.write(bytes([0] * (size - tmp)))
    print('padded ', size - tmp, ' bytes')

# class PreinstallableObject: 
#     def __init__(self, name, type, first_sector, sectors_occupied):
#         self.name = name
#         self.type = type
#         self.first_sector = first_sector
#         self.sectors_occupied = sectors_occupied
    
#     def display_information(self):
#         print(f"name: {self.name}\ntype: {self.type}\nfirst_sector: {self.first_sector}\nentry_size: {self.sectors_occupied}\n")

xin_current_date = decimal_to_bcd(datetime.now().strftime('%d%m%Y'))
xin_current_time = decimal_to_bcd(datetime.now().strftime('%H%M'))

print("NORMAL: ", datetime.now().strftime('%d%m%Y'))
# print("BCD", xin_current_time[0])
    
class XinEntryData: 
    def __init__(self, path, type, permissions=XIN_MAX_PERMISSIONS, size=0, first_sector=0):
        self.path = convert_string_to_aligned_bytes(path, XIN_MAX_ENTRY_PATH_LENGTH)
        self.type = type
        self.creation_date = xin_current_date 
        self.creation_time = xin_current_time 
        self.modification_date = self.creation_date
        self.modification_time = self.creation_time
        self.permissions = permissions
        
        if(self.type != XIN_DIRECTORY):
            self.size = size.to_bytes(4, byteorder='little')
            self.first_sector = size.to_bytes(4, byteorder='little')

    def write(self, file, xin_pointers_begin=None):

        if self.type != XIN_DIRECTORY:
            file.seek(xin_pointers_begin + self.first_sector)
            for i in range(self.sectors_occupied - 1):
                file.write(XIN_ALLOCATED)
            file.write(XIN_EOF)


        file.write(self.path)
        file.write(self.type)
        file.write(self.creation_date)
        file.write(self.creation_tiime)
        file.write(self.modification_date)
        file.write(self.modification_tiime)
        file.write(self.size)
        file.write(self.first_sector)
        file.write(int(0).to_bytes(4, 'little')) #File Info ptr



def preinstall(file): 

    xin_pointers_begin = os.path.getsize(file.name)
    xin_entries_begin = xin_pointers_begin + (SECTOR_SIZE * 512)

    # TUTAJ KURSOR DLA klasay XIN_ENTRY_DATA
    # xin_pointers_cursor = 
    # xin_entries_cursor = 

    xin_preinstalled_objects_cursor = xin_entries_begin + (SECTOR_SIZE * 256) #current preintall objects data file cursor

    xin_entries_current_entry_index = 0
    # xin_filesystem_pointers = xin_filesystem_pointers_begin 

    xanin_entries_to_preinstall = [
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

    file.seek(xin_entries_begin)

    for entry in xanin_entries_to_preinstall:

        if entry[1] != XIN_DIRECTORY:
            file.seek(xin_pointers_begin + entry.first_sector)
            for i in range(entry.sectors_occupied - 1):
                file.write(XIN_ALLOCATED)
            file.write(XIN_EOF)

        # xin_entries_cursor = xin_entries_begin + (xin_entries_current_entry_index * XIN_ENTRY_SIZE)

        file.write(bytes(entry.name, 'ascii', ))
        file.seek(xin_entries_cursor + 38) #entry path
        file.write(bytes(entry.name, 'ascii'))
        file.write(bytes(13))
        file.write((entry.sectors_occupied * SECTOR_SIZE).to_bytes(4, 'little')) #entry size
        file.write((entry.first_sector).to_bytes(4, 'little')) #first sector
        xin_entries_current_entry_index += 1

    # file.close()

    print(f"EXTERNAL APPS LOCATION: {hex(xin_pointers_begin)} {hex(xin_preinstall_objects_data)}")

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
    print(datetime.now().strftime('%d%m%Y'))
    print(datetime.now().strftime('%H%M'))
    # print(os.path.abspath(args.image))
    # args.image = os.path.abspath(args.image)
    # image = open(args.image, 'rb+')
    # align_to_size(image, 16)
    # print(image.read()[0])

if __name__ == '__main__':
    main(args)