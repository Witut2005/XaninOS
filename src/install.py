
import os
import argparse

parser = argparse.ArgumentParser()

parser.add_argument('-dest', help='Device name where XaninOS will be installed',required=True)
parser.add_argument('-src', help='XaninOS image', required=True)

args = parser.parse_args()

command = 'dd if=' + str(args.dest) + ' ' + 'of=' + str(args.src)

print(command)
os.system(command)


