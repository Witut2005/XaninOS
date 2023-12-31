
import argparse
from colorama import Fore 
from colorama import Style
import sys
import os 

args = argparse.ArgumentParser()
args.add_argument('--path', action='store', type=str, required=True)

args = args.parse_args()

def align_to_size(file, size):
    file_size = file.seek(0, os.SEEK_END)

    tmp = file_size

    while tmp > size:
        tmp = tmp - size

    file.write(bytes([40] * (size - tmp)))
    print('padded ', size - tmp, ' bytes')

def main(args):
    print(os.path.abspath(args.path))
    args.path = os.path.abspath(args.path)
    image = open(args.path, 'rb+')
    align_to_size(image, 16)
    # print(image.read()[0])

if __name__ == '__main__':
    main(args)