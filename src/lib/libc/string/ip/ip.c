#include "./ip.h"
#include "../info/info.h"
#include "../parse/parse.h"
#include <lib/libc/memory.h>
#include <stddef.h>
#include <stdint.h>

static uint32_t string_errno;

uint32_t check_string_errors(uint32_t mask)
{
    return string_errno & mask;
}

uint32_t str2ipv4(char const* str)
{

    uint32_t tmp = 0;
    uint32_t counter = 0;

    if (strlen(str) == 15)
    {

        char str_tmp[4] = { '\0' };
        uint32_t octet;

        memcpy(str_tmp, str, 3);

        octet = strtoi(str_tmp, STRTOI_DECIMAL);
        if (octet > 0xFF)
            octet = 0xFF;

        tmp = octet << 8;

        if (str[3] != '.')
            return LIBC_STR_BAD_IP_ADDRESS;

        memcpy(str_tmp, str + 4, 3);

        octet = strtoi(str_tmp, STRTOI_DECIMAL);
        if (octet > 0xFF)
            octet = 0xFF;

        tmp = tmp | octet;
        tmp = tmp << 8;

        if (str[7] != '.')
            return LIBC_STR_BAD_IP_ADDRESS;

        memcpy(str_tmp, str + 8, 3);

        octet = strtoi(str_tmp, STRTOI_DECIMAL);
        if (octet > 0xFF)
            octet = 0xFF;

        tmp = tmp | octet;
        tmp = tmp << 8;

        if (str[11] != '.')
            return LIBC_STR_BAD_IP_ADDRESS;

        memcpy(str_tmp, str + 12, 3);
        octet = strtoi(str_tmp, STRTOI_DECIMAL);

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
                string_errno = string_errno | LIBC_STR_IPV4_ERRNO;
                return LIBC_STR_BAD_IP_ADDRESS;
            }

            str_tmp[counter] = str[counter];
        }

        octet = strtoi(str_tmp, STRTOI_DECIMAL);

        if (octet > 255)
        {
            string_errno = string_errno | LIBC_STR_IPV4_ERRNO;
            return LIBC_STR_BAD_IP_ADDRESS;
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
                string_errno = string_errno | LIBC_STR_IPV4_ERRNO;
                return LIBC_STR_BAD_IP_ADDRESS;
            }

            str_tmp[counter - counter_start] = str[counter];
        }

        octet = strtoi(str_tmp, STRTOI_DECIMAL);

        if (octet > 255)
        {
            string_errno = string_errno | LIBC_STR_IPV4_ERRNO;
            return LIBC_STR_BAD_IP_ADDRESS;
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
                string_errno = string_errno | LIBC_STR_IPV4_ERRNO;
                return LIBC_STR_BAD_IP_ADDRESS;
            }

            str_tmp[counter - counter_start] = str[counter];
        }

        octet = strtoi(str_tmp, STRTOI_DECIMAL);

        if (octet > 255)
        {
            string_errno = string_errno | LIBC_STR_IPV4_ERRNO;
            return LIBC_STR_BAD_IP_ADDRESS;
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
                string_errno = string_errno | LIBC_STR_IPV4_ERRNO;
                return LIBC_STR_BAD_IP_ADDRESS;
            }

            str_tmp[counter - counter_start] = str[counter];
        }

        octet = strtoi(str_tmp, STRTOI_DECIMAL);

        if (octet > 255)
        {
            string_errno = string_errno | LIBC_STR_IPV4_ERRNO;
            return LIBC_STR_BAD_IP_ADDRESS;
        }

        tmp = tmp | octet;
    }

    string_errno = (uint32_t)NULL;

    return tmp;
}
