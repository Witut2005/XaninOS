
import os 

def decimal_to_bcd(number):
    ascii_digits = number.encode('ascii')
    bcd_digits = b''
    for i in range(len(ascii_digits)):
        bcd_digits += (ascii_digits[i] - 48).to_bytes(1, byteorder='little')

    return bcd_digits

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

def size_to_sectors(size):
    return size // 512 + (1 if size % 512 != 0 else 0)