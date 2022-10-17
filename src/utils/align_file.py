
import numpy as np

SECTOR_SIZE = 512

import argparse
args = argparse.ArgumentParser()

args.add_argument('-file', action='store', type=str, required=True)
args.add_argument('-size', action='store', type=int, required=True)

args = args.parse_args()

file = open(args.file, 'ab')

print('size before aligment: ', file.tell())


# while file.tell() % args.size != 0:
file.write(bytes(np.zeros(args.size - (file.tell() % args.size))))

print('size after aligment: ', file.tell())


file.close()

