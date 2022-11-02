
import numpy as np
from detect_xin_ext import *
import argparse

args = argparse.ArgumentParser()
args.add_argument('-image', action='store', type=str, required=True)
args.add_argument('-files', action='store', type=str, required=True, nargs='+')
args = args.parse_args()

xin_extended_table = detect_xin_ext(args.image)
print(hex(xin_extended_table))
file = open(str(args.image), 'rb+')
file.seek(xin_extended_table + len('xin_extended_table'))

for current_file in args.files:
    aha = open(current_file, 'rb')
    data = aha.read()
    file.write(data)
    aha.close()
