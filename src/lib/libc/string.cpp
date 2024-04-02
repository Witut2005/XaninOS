
#include "endian.h"
#include <stdarg.h>
#include <stdint.h>
#include <lib/libc/math.h>
#include <lib/libc/colors.h>
#include <lib/libc/memory.h>
#include <lib/libc/string.h>
#include <lib/libc/stdlibx.h>
#include <lib/screen/screen.h>
#include <sys/devices/com/com.h>

#define ASCII_CASE_OFFSET 32 
#define EXIT_ON_EQUALS_ZERO(val, return_value) if(val == 0) {return return_value;}

static uint32_t string_errno;

extern "C"
{
    bool is_digit(char c)
    {
        return c >= '0' && c <= '9';
    }

    bool is_in_char_range(char r1, char r2, char c)
    {
        return  c > r1 && c < r2;
    }

    bool is_char(char c)
    {
        return c > '0' && c < '~';
    }

    char* char_find(char* str, char c)
    {
        if (c == CHAR_FIND_LETTERS)
        {
            for (int i = 0; str[i] != '\0'; i++)
            {
                if ((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z')) {
                    return &str[i];
                }
            }
        }

        else if (c == CHAR_FIND_DIGITS)
        {
            for (int i = 0; str[i] != '\0'; i++)
            {
                if (str[i] >= '0' && str[i] <= '9') {
                    return &str[i];
                }
            }
        }

        else
        {
            for (int i = 0; str[i] != '\0'; i++)
            {
                if (str[i] == c) {
                    return &str[i];
                }
            }
        }

        return NULL;
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

    int32_t strcmp(const char* a, const char* b)
    {
        while (*a && (*a == *b)) {
            a++;
            b++;
        }

        return *a - *b;
    }

    bool bstrcmp(const char* a, const char* b)
    {
        return strcmp(a, b) == 0;
    }

    bool bstrncmp(const char* a, const char* b, size_t string_size)
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

    char* time_to_string(bcd_time_t time, char* buf)
    {
        constexpr uint32_t time_lenght = 5; // 5 characters

        time = endian_switch16(time);
        char* sp = (char*)&time;

        for (int si = 0, di = 0; di < time_lenght;)
        {
            if (di == 2) {
                buf[di++] = ':';
            }

            bcd_to_string(sp[si], &buf[di]);
            si++;
            di = di + 2;
        }

        buf[time_lenght] = '\0';
        return buf;
    }

    char* date_to_string(bcd_date_t date, char* buf)
    {
        constexpr uint32_t date_lenght = 10; // 10 characters

        date = endian_switch32(date);
        char* sp = (char*)&date; //source pointer

        for (int si = 0, di = 0; di < date_lenght; )
        {
            if (di == 2 || di == 5) {
                buf[di++] = '-';
            }

            bcd_to_string(sp[si], &buf[di]);
            si++;
            di = di + 2;
        }

        buf[date_lenght] = '\0';
        return buf;
    }

    char* int_to_decimal_string(bool _signed, int32_t value, char* buf)
    {
        if (_signed && value < 0) {
            buf[0] = '-';
            buf++;
        }

        uint32_t vt = _signed ? abs(value) : value;
        int i = 0;
        while (vt) {
            buf[i++] = '0' + (vt % 10);
            vt = vt / 10;
        };

        buf[i] = '\0';
        strrev(buf);

        return (_signed && (value < 0)) ? buf - 1 : buf;
    }

    char* int_to_string(uint32_t value, char* buf, const uint8_t base)
    {
        char digits[] = { '0','1','2','3','4','5','6','7','8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
                            'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
                            't', 'u', 'v', 'w' };

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
        buf[0] = (((x & 0xf0) >> 4) % 10 + '0');
        buf[1] = ((x & 0x0f) % 10 + '0');
        buf[2] = '\0';
        return buf;
    }

    char* bcd_stream_to_string(uint8_t* value, uint32_t value_size, char* buf)
    {
        for (int i = 0; i < value_size; i++) {
            bcd_to_string(value[i], &buf[i * 2]);
        }

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

    uint32_t atoi(char* str)
    {
        uint32_t sum = 0;

        for (int i = 0; i < strlen(str); i++) {
            if (is_digit(str[i])) {
                sum *= 10;
                sum += str[i] - '0';
            }
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

    uint32_t str2ipv4(const char* str)
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

    char* strdup(const char* str)
    {
        char* ns = (char*)calloc(strlen(str) * SIZE_OF(char));
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
            char* ts = (char*)calloc(strlen(dest) + strlen(src));
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


    enum class SPrintfExpect
    {
        NormalChar,
        Filler,
        FillerCounter,
        Format
    };

    char* xvsnprintf(char* str, size_t n, char* fmt, va_list args)
    {
        #warning "TO DO finish sprintf";

        // va_list args;
        // va_start(args, fmt);

        SPrintfExpect expect = SPrintfExpect::NormalChar;
        char filler = ' ';
        uint32_t filler_counter = 0;

        constexpr char formats[] = { 'd', 'i', 'u', 'o', 'x', 'X', 'c', 's', 'p' , 'n', 'q', 'y', 't' };

        auto is_format_char = [formats](char c) {
            for (int i = 0; i < ARRAY_LENGTH(formats); i++) {
                if (c == formats[i]) return true;
            }
            return false;
        };

        auto format_base_get = [](char c) -> uint8_t {
            switch (c) {
            case 'b': return BINARY;
            case 'o': return OCTAL;
            case 'd': return DECIMAL;
            case 'x':
            case 'X': return HEXADECIMAL;
            }};

        auto toupper_if_needed = [](char c, char* str) -> char* {if (c >= 'A' && c <= 'Z') { toupper(str); } return str;};

        n--; // last character cant be overriden

        for (int si = 0, di = 0; fmt[si] != '\0' && di <= n; )
        {
            switch (expect)
            {

            case SPrintfExpect::NormalChar: {

                if (fmt[si + 1] == '%' && fmt[si] == '%') {

                    if (fmt[si + 1] == '%') {
                        str[di++] = '%';
                        si++;
                    }
                }

                else if (fmt[si] == '%') {
                    expect = SPrintfExpect::Filler;
                }

                else {
                    str[di++] = fmt[si];
                }

                si++;
                break;
            }

            case SPrintfExpect::Filler:
            {
                // dbg_info(DEBUG_LABEL_LIBC, "Expecting filler");
                if (is_format_char(fmt[si])) {
                    expect = SPrintfExpect::Format;
                    break;
                }

                if (!(fmt[si] >= '1' && fmt[si] <= '9')) {
                    filler = fmt[si];
                    si++;
                }
                expect = SPrintfExpect::FillerCounter;
                break;
            }

            case SPrintfExpect::FillerCounter:
            {
                // dbg_info(DEBUG_LABEL_LIBC, "Expecting filler counter");
                char counter_str[64] = { 0 };
                for (int j = 0; fmt[si + j] != '\0'; j++)
                {
                    if (is_format_char(fmt[si + j])) {
                        memcpy(counter_str, &fmt[si], j);
                        filler_counter = atoi(counter_str);
                        si = si + j;
                        expect = SPrintfExpect::Format;
                        break;
                    }
                }
                break;
            }

            case SPrintfExpect::Format:
            {
                char st[64] = { 0 };
                memset(&str[di], filler, filler_counter); //fill with filler 

                switch (fmt[si]) {

                case '\0': {
                    break;
                }

                case 'c': {
                    str[di + (filler_counter > 1 ? filler_counter - 1 : 0)] = (char)va_arg(args, uint32_t);
                    di++;
                    break;
                }

                case 's': {
                    char* sa = va_arg(args, char*); // sa = string_argument
                    uint32_t sa_length = strlen(sa);
                    strncpy(&str[di + (sa_length >= filler_counter ? 0 : (filler_counter - sa_length))], sa, n - di);

                    di = di + (sa_length > filler_counter ? sa_length : filler_counter);
                    break;
                }

                case 'q': {
                    //%h is used to print BCD digits
                    constexpr uint32_t bcd_length = 2;
                    bcd_to_string((uint8_t)va_arg(args, int), st);

                    strncpy(&str[di + (bcd_length >= filler_counter ? 0 : (filler_counter - bcd_length))], st, n - di);

                    di = di + (bcd_length > filler_counter ? bcd_length : filler_counter);
                    break;
                }

                case 'u': {
                    int_to_decimal_string(STRING_UNSIGNED, va_arg(args, int), st);
                    uint32_t st_length = strlen(st);
                    strncpy(&str[di + (st_length >= filler_counter ? 0 : (filler_counter - st_length))], st, n - di);
                    di = di + (st_length > filler_counter ? st_length : filler_counter);
                    break;
                }

                case 'n': {
                    //weird character counter
                    *(va_arg(args, uint32_t*)) = di;
                    break;
                }

                case 'y': {
                    //date
                    constexpr uint32_t date_length = 10;
                    date_to_string(va_arg(args, bcd_date_t), st);

                    strncpy(&str[di + (date_length >= filler_counter ? 0 : (filler_counter - date_length))], st, n - di);

                    di = di + (date_length > filler_counter ? date_length : filler_counter);
                    break;
                }

                case 't': {
                    //time
                    constexpr uint32_t time_length = 5;
                    time_to_string((bcd_time_t)va_arg(args, int), st);

                    strncpy(&str[di + (time_length >= filler_counter ? 0 : (filler_counter - time_length))], st, n - di);

                    di = di + (time_length > filler_counter ? time_length : filler_counter);
                    break;
                }

                default: {
                    int_to_string(va_arg(args, int), st, format_base_get(fmt[si]));
                    uint32_t st_length = strlen(st);
                    toupper_if_needed(fmt[si], st);

                    strncpy(&str[di + (st_length >= filler_counter ? 0 : (filler_counter - st_length))], st, n - di);
                    di = di + (st_length > filler_counter ? st_length : filler_counter);
                }
                }

                si++;
                expect = SPrintfExpect::NormalChar;
                filler = ' ';
                filler_counter = 0;

                break;
            }
            }
        }
        return str;
    }

    char* xsnprintf(char* str, size_t n, char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);

        return xvsnprintf(str, n, fmt, args);
    }


    char* xsprintf(char* str, char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);

        return xvsnprintf(str, UINT32_MAX, fmt, args);
    }

    int vsnprintf(char* str, size_t n, char* fmt, va_list args) {
        return strlen(xvsnprintf(str, n, fmt, args));
    }

    int snprintf(char* str, size_t n, char* fmt, ...) {
        va_list args;
        va_start(args, fmt);

        return strlen(xvsnprintf(str, n, fmt, args));
    }

    int sprintf(char* str, char* fmt, ...) {
        va_list args;
        va_start(args, fmt);

        return strlen(xvsnprintf(str, UINT32_MAX, fmt, args));
    }

} //extern "C"