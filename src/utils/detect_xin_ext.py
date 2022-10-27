

SECTOR_SIZE = 512

import argparse
args = argparse.ArgumentParser()

args.add_argument('-file', action='store', type=str, required=True)

args = args.parse_args()

file = open(args.file, 'rb+')

str = file.read()

print(hex(str.index(bytes('xin_extended_table', 'ascii'))))

file.close()

