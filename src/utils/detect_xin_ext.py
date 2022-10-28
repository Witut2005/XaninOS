import argparse

SECTOR_SIZE = 512

def detect_xin_ext(file_name):
    file = open(file_name, 'rb')
    str = file.read()
    file.close()
    return str.find(bytes('xin_extended_table', 'ascii'))

def main():
    args = argparse.ArgumentParser()
    args.add_argument('-file', action='store', type=str, required=True)
    args = args.parse_args()
    detect_xin_ext(args.file)


if __name__ == '__main__':
    main()