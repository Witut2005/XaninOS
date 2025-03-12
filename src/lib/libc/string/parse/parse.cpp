#include <stdint.h>
#include <lib/libc/time.h>
#include <lib/libc/endian.h>
#include <lib/libc/math.h>
#include <lib/libc/memory.h>
#include "../manip/manip.h"
#include "../info/info.h"
#include "./parse.h"

extern "C" {

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

        if (base == STRTOI_DECIMAL) {
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

} 