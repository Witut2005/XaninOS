
import os 
import math

def decimal_to_bcd(number):
    ascii_digits = number.encode('ascii')
    bcd_digits = b''

    index = 0

    #nieparzyste tez moze byc
    while index < len(ascii_digits):
        bcd_digits += (((ascii_digits[index] - 48) << 4) | (ascii_digits[index+1]-48)).to_bytes(1, byteorder='little')
        index += 2

    return bcd_digits

def convert_string_to_aligned_bytes(input_string, alignment_size):
    encoded_bytes = input_string.encode('ascii')
    padding_size = alignment_size - len(encoded_bytes) % alignment_size

    aligned_bytes = encoded_bytes + b'\0' * padding_size

    return aligned_bytes

def align_to(value, alignment):
    return math.ceil(value / alignment) * alignment

def pad_bytesarray(input_bytesarray, target_size):
    current_size = len(input_bytesarray)

    if current_size >= target_size:
        return input_bytesarray
    else:
        padding_size = target_size - current_size
        input_bytesarray.extend(bytes([0] * padding_size))
        return input_bytesarray


def pad_bytes(b, alignment_size):
    padding_size = alignment_size - len(b) % alignment_size

    aligned_bytes = b + b'\0' * padding_size

    return aligned_bytes

def align_file_to_size(filename, size):
    file_size = os.path.getsize(filename)

    tmp = file_size

    while tmp > size:
        tmp = tmp - size

    file = open(filename, 'ab')
    file.write(bytes([0] * (size - tmp)))
    file.flush()
    return size - tmp

def size_to_sectors(size):
    if size == None:
        return 0
    return size // 512 + (1 if size % 512 != 0 else 0)

def bcd_to_int(bcd_value):
    result = 0
    for digit in str(bcd_value):
        result = result * 10 + int(digit)
    return result

def convert_xin_date_to_str(date):
    return f'{format(date[0], "x").rjust(2, "0")}/{format(date[1], "x").rjust(2, "0")}/{format(date[2], "x") + format(date[3], "x")}'

def convert_xin_time_to_str(time):
    return f'{format(time[0], "x").rjust(2, "0")}:{format(time[1], "x").rjust(2, "0")}'