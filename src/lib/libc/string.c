
#include <stdint.h>
#include <lib/libc/math.h>
#include <lib/libc/colors.h>
#include <lib/libc/memory.h>
#include <lib/libc/string.h>
#include <lib/libc/stdlibx.h>
#include <lib/screen/screen.h>
#include <sys/devices/keyboard/key_map.h>

#define ASCII_CASE_OFFSET 32 
#define EXIT_ON_EQUALS_ZERO(val, return_value) if(val == 0) {return return_value;}

static uint32_t string_errno;

int char_find(const char* str, char c)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == c) {
            return i;
        }
    }
    return -1;
}

uint32_t check_string_errors(uint32_t mask)
{
    return string_errno & mask;
}

uint32_t strlen(const char* str)
{
    EXIT_ON_EQUALS_ZERO(str, 0);

    uint32_t length = 0;
    for (int i = 0; str[i] != '\0'; i++, length++);

    return length;
}

char* strcpy(char* dest, const char* src)
{
    EXIT_ON_EQUALS_ZERO(src, NULL);
    uint32_t length = strlen(src);
    memcpy(dest, src, strlen(src));
    dest[length] = '\0';
    return dest;
}

char* strncpy(char* dest, const char* src, size_t size)
{
    EXIT_ON_EQUALS_ZERO(size, NULL);

    uint32_t src_length = strlen(src);
    uint32_t length = src_length > size ? size : src_length;

    memcpy(dest, src, length);
    dest[length] = '\0';

    return dest;
}

char* strrev(char* str)
{
    int len = strlen(str);

    // for loop
    for (int i = 0, j = len - 1; i <= j; i++, j--) {
        // swapping characters
        char c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
    return str;
}

int32_t strcmp(char* a, const char* b)
{
    while (*a && (*a == *b)) {
        a++;
        b++;
    }

    return *a - *b;
}

bool bstrcmp(char* a, const char* b)
{
    return strcmp(a, b) == 0;
}

bool bstrncmp(char* a, const char* b, size_t string_size)
{
    // uint32_t length_to_check = strlen()
    for (int i = 0; i < string_size; i++)
    {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

char* int_to_decimal_string(int32_t value, char* buf)
{
    int_to_string(abs(value), buf, 10);

    if (value < 0) {
        memmove(&buf[1], buf, strlen(buf));
        buf[0] = '-';
    }
    return buf;
}

char* int_to_string(uint32_t value, char* buf, const uint8_t base)
{
    char digits[] = { '0','1','2','3','4','5','6','7','8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
                        'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
                        'T', 'U', 'V', 'W' };

    if (base > 32)
        return NULL;

    if (value == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }

    if (base == DECIMAL) {
        if ((int32_t)value < 0) {
            buf[0] = '-';
            buf++;
        }
        value = abs((int32_t)value);
    }

    int i = 0;
    while (value) {
        buf[i++] = digits[value % base];
        value = value / base;
    };
    buf[i] = '\0';

    strrev(buf);
    return buf;
}

char* bcd_to_string(uint8_t x, char* buf)
{
    buf[0] = (((x & 0xf0) >> 4) + 48);
    buf[1] = ((x & 0x0f) + 48);
    return buf;
}

void erase_spaces(char* str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == ' ') {
            memmove(&str[i], &str[+1], strlen(str) - i);
        }
    }
}

char* toupper(char* str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= ASCII_CASE_OFFSET;
        }
    }
    return str;
}

char* tolower(char* str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] += ASCII_CASE_OFFSET;
        }
    }
    return str;
}

char* xint_to_hex_str(uint32_t x, char* buf, uint8_t how_many_chars)
{
    how_many_chars *= 2;

    char hex_values[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

    if (!x)
    {
        buf[0] = '0';
        buf[1] = '0';
        buf[2] = '\0';
        return buf;
    }

    for (int i = 0; i <= how_many_chars; i++)
    {
        if (i < how_many_chars)
            buf[i] = '0';
        else
            buf[i] = '\0';
    }

    for (int i = 0; x != 0; i++)
    {
        buf[i] = hex_values[x % 16];
        x = x / 16;
    }

    buf = strrev(buf);

    return buf;
}

uint32_t atoi(char* str)
{

    uint32_t sum = 0;

    for (int i = 0; i < strlen(str); i++)
    {
        sum *= 10;
        sum += str[i] - 48;
    }

    return sum;
}

uint32_t strtoi(const char* str, uint8_t format)
{

    bool is_negative = false;

    if (str[0] == '-')
        is_negative = true;

    uint32_t sum = 0;

    if (format > 16)
        format = 16;

    if (format == 16 && (bstrncmp((char*)str, "0x", 2) || bstrncmp((char*)str, "0b", 2) || bstrncmp((char*)str, "0o", 2)))
        str += 2;

    uint8_t digit_counter = strlen(str);
    uint32_t tmp;

    for (int i = 0; i < digit_counter; i++)
    {

        if (str[i] >= '0' && str[i] <= '9')
            tmp = str[i] - 48;

        else if (str[i] >= 'a' && str[i] <= 'f')
            tmp = str[i] - 'a' + 0xa;

        else if (str[i] >= 'A' && str[i] <= 'F')
            tmp = str[i] - 'A' + 0xa;

        if (tmp >= format)
            continue;

        sum *= format;

        sum += tmp;
    }

    if (is_negative)
        sum = sum * (-1);
    return sum;
}

uint32_t str2ipv4(char* str)
{

    uint32_t tmp = 0;
    uint32_t counter = 0;

    if (strlen(str) == 15)
    {

        char str_tmp[4] = { '\0' };
        uint32_t octet;

        memcpy(str_tmp, str, 3);

        octet = strtoi(str_tmp, DECIMAL);
        if (octet > 0xFF)
            octet = 0xFF;

        tmp = octet << 8;

        if (str[3] != '.')
            return BAD_IP_ADDRESS;

        memcpy(str_tmp, str + 4, 3);

        octet = strtoi(str_tmp, DECIMAL);
        if (octet > 0xFF)
            octet = 0xFF;

        tmp = tmp | octet;
        tmp = tmp << 8;

        if (str[7] != '.')
            return BAD_IP_ADDRESS;

        memcpy(str_tmp, str + 8, 3);

        octet = strtoi(str_tmp, DECIMAL);
        if (octet > 0xFF)
            octet = 0xFF;

        tmp = tmp | octet;
        tmp = tmp << 8;

        if (str[11] != '.')
            return BAD_IP_ADDRESS;

        memcpy(str_tmp, str + 12, 3);
        octet = strtoi(str_tmp, DECIMAL);

        if (octet > 0xFF)
            octet = 0xFF;

        tmp = tmp | octet;
    }

    else
    {
        char str_tmp[4] = { '\0' };
        uint32_t octet;

        for (counter = 0; counter <= 3; counter++)
        {
            if (str[counter] == '.')
            {
                counter++;
                break;
            }

            if ((str[counter] != '.') && (counter == 3))
            {
                string_errno = string_errno | IPV4_ERRNO;
                return BAD_IP_ADDRESS;
            }

            str_tmp[counter] = str[counter];
        }

        octet = strtoi(str_tmp, DECIMAL);

        if (octet > 255)
        {
            string_errno = string_errno | IPV4_ERRNO;
            return BAD_IP_ADDRESS;
        }

        tmp = tmp | octet;
        tmp = tmp << 8;
        memset(str_tmp, 0, 4);

        uint32_t counter_start = counter;
        uint32_t counter_tmp = counter + 3;

        for (; counter <= counter_tmp; counter++)
        {
            if (str[counter] == '.')
            {
                counter++;
                break;
            }

            if ((str[counter] != '.') && (counter == counter_tmp))
            {
                string_errno = string_errno | IPV4_ERRNO;
                return BAD_IP_ADDRESS;
            }

            str_tmp[counter - counter_start] = str[counter];
        }

        octet = strtoi(str_tmp, DECIMAL);

        if (octet > 255)
        {
            string_errno = string_errno | IPV4_ERRNO;
            return BAD_IP_ADDRESS;
        }

        tmp = tmp | octet;
        tmp = tmp << 8;
        memset(str_tmp, 0, 4);

        counter_start = counter;
        counter_tmp = counter + 3;

        for (; counter <= counter_tmp; counter++)
        {
            if (str[counter] == '.')
            {
                counter++;
                break;
            }

            if ((str[counter] != '.') && (counter == counter_tmp))
            {
                string_errno = string_errno | IPV4_ERRNO;
                return BAD_IP_ADDRESS;
            }

            str_tmp[counter - counter_start] = str[counter];
        }

        octet = strtoi(str_tmp, DECIMAL);

        if (octet > 255)
        {
            string_errno = string_errno | IPV4_ERRNO;
            return BAD_IP_ADDRESS;
        }

        tmp = tmp | octet;
        tmp = tmp << 8;
        memset(str_tmp, 0, 4);

        counter_start = counter;
        counter_tmp = counter + 3;

        for (; counter <= counter_tmp; counter++)
        {
            if (str[counter] == '\0')
                break;

            if ((str[counter] != '.') && (counter == counter_tmp))
            {
                string_errno = string_errno | IPV4_ERRNO;
                return BAD_IP_ADDRESS;
            }

            str_tmp[counter - counter_start] = str[counter];
        }

        octet = strtoi(str_tmp, DECIMAL);

        if (octet > 255)
        {
            string_errno = string_errno | IPV4_ERRNO;
            return BAD_IP_ADDRESS;
        }

        tmp = tmp | octet;
    }

    string_errno = (uint32_t)NULL;

    return tmp;
}

char* substr_find(char* str, const char* substr)
{
    while (*str != '\0')
    {
        if (bstrncmp(str, substr, strlen(substr))) {
            return str;
        }
        str++;
    }
    return NULL;
}

char* substr_last_find(char* str, const char* substr)
{
    int last_index = -1;

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (bstrncmp(&str[i], substr, strlen(substr))) {
            last_index = i;
        }
        str++;
    }

    return last_index != -1 ? &str[last_index] : NULL;
}

char* strdup(char* str)
{
    char* ns = calloc(strlen(str) * SIZE_OF(char));
    strcpy(ns, str);
    return ns;
}

char* strcat(bool dest_first, char* dest, char* src)
{
    //dest = dest + src
    if (dest_first) {
        memmove(&dest[strlen(dest)], src, strlen(src)); //include '\0' too
    }

    //dest = src + dest
    else {
        char* ts = calloc(strlen(dest) + strlen(src));
        strcpy(ts, src);
        strcpy(&ts[strlen(ts)], dest);
        strcpy(dest, ts);
        free(ts);
    }
    return dest;
}

uint32_t number_of_lines_get(const char* str)
{
    uint32_t lines = 0;

    while (*str) {
        if (*str == '\n') {
            lines++;
        }
        str++;
    }
    return lines + 1;
}

uint32_t size_of_biggest_line_get(const char* str)
{
    int max_line_size = 0;
    int current_line_size = 0;

    while (*str)
    {
        if (*str == '\n') // do not include \x1e
        {
            if (current_line_size > max_line_size) {
                max_line_size = current_line_size;
            }

            current_line_size = 0;
        }
        else {
            current_line_size++;
        }
        str++;
    }

    if (current_line_size > max_line_size) {
        max_line_size = current_line_size;
    }

    return max_line_size;
}

StringRectangle* const string_rectangle_create(const char* buf, uint32_t position_x, uint32_t position_y)
{
    StringRectangle* Rect = (StringRectangle*)calloc(SIZE_OF(StringRectangle));
    Rect->size_x = size_of_biggest_line_get(buf);
    Rect->size_y = number_of_lines_get(buf);
    Rect->position_x = position_x;
    Rect->position_y = position_y;

    return Rect;
}

char* string_align_begin(char* const str, char filler, uint32_t count)
{

    uint32_t string_length = strlen(str);

    if (string_length >= count) {
        return str;
    }

    memmove(str + (count - string_length), str, string_length);

    int i;
    for (i = 0; i < count - string_length; i++)
        str[i] = filler;

    str[count] = '\0'; // put NULL terminator

    return str;
}

char* string_align_end(char* const str, char filler, uint32_t count)
{
    int i;
    for (i = strlen(str); i < count; i++) {
        str[i] = filler;
    }

    str[i] = '\0'; // put NULL terminator

    return str;
}

char* getline(XinEntry* File, int line_id)
{

    fread(File, NULL, File->size); // loads all data to buffer

    char* file_data = (char*)(File->FileInfo->buffer);
    char* line = (char*)calloc(XANIN_PMMNGR_BLOCK_SIZE);
    int current_line = 0;

    for (; *file_data != '\0'; file_data++)
    {
        if (current_line == line_id)
        {
            for (int i = 0; file_data[i] != '\n' && file_data[i] != '\0'; i++) {
                line[i] = file_data[i];
            }
            return line;
        }
        if (*file_data == '\n') {
            current_line++;
        }
    }

    free(line);
    return NULL;
}