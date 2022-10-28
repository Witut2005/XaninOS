
import numpy as np
from detect_xin_ext import *
import argparse

args = argparse.ArgumentParser()
args.add_argument('-image', action='store', type=str, required=True)
args.add_argument('-files', action='store', type=str, required=True, nargs='+')
args = args.parse_args()

xin_extended_table = detect_xin_ext(args.image)

print(xin_extended_table)
# print(args.image)
file = open(args.image, 'rb+')
file.write(bytes('HUJJJ', 'ascii'))
file.seek(xin_extended_table)
print('aha:', hex(file.tell()))

for current_file in args.files:
    aha = open(current_file, 'rb')
    data = aha.read()
    file.write(data)
    aha.close()
