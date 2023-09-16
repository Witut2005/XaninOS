
#include <lib/libc/string.h>
#include <stdarg.h>
#include <stdint.h>
#include <lib/libc/colors.h>
#include <lib/screen/screen.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/math.h>
#include <sys/devices/keyboard/key_map.h>
#include <lib/libc/memory.h>

extern void xprintf(char* str, ...);

static uint32_t string_errno;

uint32_t check_string_errors(uint32_t mask)
{
    return string_errno & mask;
}

uint32_t strlen(const char* a)
{

    if(a == NULL)
        return 0;

    uint32_t length = 0;

    for(const char* i = a; *i != '\0' ;i++)
        length++;

    return length;
}

char* strcpy(char* dest, const char* src)
{
    for(; *src != '\0'; dest++, src++)
        *dest = *src;
    *dest = *src;
    return dest;
}


char* strncpy(char* x, const char* y, size_t size)
{
    for(; *y != '\0' && size == 0; x++, y++, size--)
        *x = *y;

    *x = *y;

    return x;
}



char* reverse_string(char* str)
{
    char buf;
    char* end = str + strlen(str) - 1;

    for(char* begin = str; (uint32_t)begin < (uint32_t)end; begin++, end--)
    {
        char buf = *begin;

        *begin = *end;
        *end = buf;
    }
    return str;
}

// char* float_to_string(float number, char* str)
// {
//     uint32_t* float_ptr = (uint32_t*)&number;
//     bool sign_bit = (*float_ptr & (1 << 31) >> 31);
//     uint8_t exponent = (*float_ptr & (0xFF << 23) >> 23);
//     exponent -= 127;
//     exponent = pow(2,exponent);
//     uint32_t mantissa = (*float_ptr & 0x7FFFFF);

//     uint32_t number_high;
//     uint32_t number_low;

//     int i = 0;

//     number_high = exponent * mantissa;

//     if(sign_bit)
//     {
//         str[i] = '-';
//         i++;
//     }




// }

uint32_t strcmp(char* a, const char* b)
{
    uint32_t diffrences = 0;

    if(strlen(a) != strlen(b))
        return abs(strlen(a) - strlen(b));

    for(int i = 0; i < strlen(a); i++)
    {
        if(a[i] != b[i])
            diffrences++;
    }

    return diffrences;
}

bool bstrcmp(char* a, const char* b)
{

    uint32_t lengtha = strlen(a);
    uint32_t lengthb = strlen(b);
    
    if(lengtha != lengthb)
        return 0;

    for(char* i = a; *i != '\0' ;i++)
    {
        if(*a != *b)
            return 0;
        a++;
        b++;
    }

    
    return 1;		
}

bool bstrncmp(char* a, const char* b, size_t string_size)
{
    for(int i = 0; i < string_size;i ++)
    {
        if(a[i] != b[i])
            return 0;
    }
    return 1;		
}



char* int_to_str(int x, char* buf)
{
    int i = 0;

    if(!x)
    {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }

    for(i = 0; x != 0; i++)
    {
        buf[i] = (x % 10) + '0';
        x = x / 10;
    }

    buf[i] = '\0';
    buf = reverse_string(buf);
    return buf;
}


char* bin_to_str(int x, char* buf)
{

    int i = 0;

    if(!x)
    {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }

    for(i = 0; x != 0; i++)
    {
        buf[i] = (x & 1) + '0';
        x = x >> 1;
    }

    buf = reverse_string(buf);
    
    *(buf + i) = '\0';
    
    return buf;

}

char* bcd_to_str(uint8_t x, char* buf)
{

    buf[0] = (((x & 0xf0) >> 4) + 48); 
    buf[1] = ((x & 0x0f) + 48); 
    return buf;

}

void erase_spaces(char* buf)
{
    for(int i = 0; buf[i] != '\0'; i++)
    {
        if(buf[i] == ' ')
        {
            buf[i] = '\0';
        }
    }
}

char* toupper(char* str)
{

    for(char* i = str; *i != '\0'; i++)
    {
        if(*i >= 'a' && *i <= 'z')
            *i -= 32;
    
    }

    return str;

}

char* tolower(char* str)
{
    for(char* i = str; *i != '\0'; i++)
    {
        if(*i >= 'A' && *i <= 'Z')
            *i += 32;
    
    }

    return str;
}


char* int_to_hex_str(uint32_t x, char* buf)
{
    char hex_values[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

    int i = 0;

    if(!x)
    {
        buf[0] = '0';
        buf[1] = '0';
        buf[2] = '\0';
        return buf;
    }
    
    for(i = 0; x != 0; i++)
    {
        buf[i] = hex_values[x % 16]; 
        x = x / 16;
    }

    buf = reverse_string(buf);
    return buf;
}

uint32_t hex_str_to_int(char* str)
{
    
    uint32_t value = 0;

    for(char* i = str; *i != '\0' && *i != ' '; i++)
    {
        value = value * 0x10;

        if(*i >= '0' && *i <= '9')
            value += *i - '0';
        
        else if(*i >= 'A' && *i <= 'F')
            value += *i - 'A' + 0xa;

    }

    return value;

}

char* xint_to_hex_str(uint32_t x, char* buf, uint8_t how_many_chars)
{
    how_many_chars *= 2;

    char hex_values[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};


    if(!x)
    {
        buf[0] = '0';
        buf[1] = '0';
        buf[2] = '\0';
        return buf;
    }
    
    //uint16_t* debug_cursor = (uint16_t*)0xb8000;

    for(int i = 0; i <= how_many_chars; i++)
    {
        if(i < how_many_chars)
            buf[i] = '0';
        else
            buf[i] = '\0';
    }


    for(int i = 0; x != 0; i++)
    {
        buf[i] = hex_values[x % 16]; 
        x = x / 16;
    }


    buf = reverse_string(buf);
    
    return buf;

}

char* int_to_oct_str(int x, char* buf)
{
    int i = 0;

    
    if(!x)
    {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }


    for(i = 0; x != 0; i++)
    {
        buf[i] = (x % 8) + 48;
        x = x / 8;
    }

    *(buf + i + 1) = '\0';

    buf = reverse_string(buf);
    
    return buf;
}
/*
char* strcpy(char* dest, char* src)
{
    while(*src)
    {
        *dest = *src;
        dest++;
        src++;
    }

    *(++dest) = '\0';
    return dest;

}
*/
uint32_t atoi(char* str)
{
    
    
    uint32_t sum = 0;

    for(int i = 0; i < strlen(str); i++)
    {
        sum *= 10;
        sum += str[i] - 48;  
    }
    
    return sum;
}



uint32_t strtoi(const char* str, uint8_t format)
{

    bool is_negative = false;

    if(str[0] == '-')
        is_negative = true;

    uint32_t sum = 0;

    if(format > 16) 
        format = 16;

    if(format == 16 && (bstrncmp((char*)str, "0x", 2) || bstrncmp((char*)str, "0b", 2) || bstrncmp((char*)str, "0o", 2)))
        str += 2;

    uint8_t digit_counter = strlen(str);
	uint32_t tmp;

    for(int i = 0; i < digit_counter; i++)
    {
    
    	if(str[i] >= '0' && str[i] <= '9')
    		tmp = str[i] - 48;
    	
    	else if(str[i] >= 'a' && str[i] <= 'f')
            tmp = str[i] - 'a' + 0xa;
            
        else if(str[i] >= 'A' && str[i] <= 'F')
        	tmp = str[i] - 'A' + 0xa; 
    
    	if(tmp >= format)
    		continue;

        sum *= format;

        sum += tmp;

    }

    if(is_negative) 
        sum = sum * (-1);
    return sum;

}


uint32_t str2ipv4(char* str)
{
    
    uint32_t tmp = 0;
    uint32_t counter = 0;
    
    if(strlen(str) == 15)
    {

        char str_tmp[4] = {'\0'}; 
        uint32_t octet;

        memcpy(str_tmp, str, 3);

        octet = strtoi(str_tmp, DECIMAL);
        if(octet > 0xFF)
            octet = 0xFF;

        tmp = octet << 8;

        if(str[3] != '.')
            return BAD_IP_ADDRESS;

        memcpy(str_tmp, str + 4, 3);

        octet = strtoi(str_tmp, DECIMAL);
        if(octet > 0xFF)
            octet = 0xFF;

        tmp = tmp | octet;
        tmp = tmp << 8;

        if(str[7] != '.')
            return BAD_IP_ADDRESS;

        memcpy(str_tmp, str + 8, 3);

        octet = strtoi(str_tmp, DECIMAL);
        if(octet > 0xFF)
            octet = 0xFF;

        tmp = tmp | octet;
        tmp = tmp << 8;

        if(str[11] != '.')
            return BAD_IP_ADDRESS;

        memcpy(str_tmp, str + 12, 3);
        octet = strtoi(str_tmp, DECIMAL);

        if(octet > 0xFF)
            octet = 0xFF;

        tmp = tmp | octet;

    }

    else
    {
        char str_tmp[4] = {'\0'}; 
        uint32_t octet;

        for(counter = 0; counter <= 3; counter++)
        {
            if(str[counter] == '.')
            {
                counter++;
                break;
            }

            if((str[counter] != '.') && (counter == 3))
            {
                string_errno = string_errno | IPV4_ERRNO;
                return BAD_IP_ADDRESS;
            }

            str_tmp[counter] = str[counter];

        }

        octet = strtoi(str_tmp, DECIMAL);

        if(octet > 255)
        {
            string_errno = string_errno | IPV4_ERRNO;
            return BAD_IP_ADDRESS;
        }

        tmp = tmp | octet;
        tmp = tmp << 8;
        memset(str_tmp, 0, 4);

        uint32_t counter_start = counter;
        uint32_t counter_tmp = counter + 3;

        for(; counter <= counter_tmp; counter++)
        {
            if(str[counter] == '.')
            {
                counter++;
                break;
            }

            if((str[counter] != '.') && (counter == counter_tmp))
            {
                string_errno = string_errno | IPV4_ERRNO;
                return BAD_IP_ADDRESS;
            }
            
            str_tmp[counter-counter_start] = str[counter];
        }

        octet = strtoi(str_tmp, DECIMAL);

        if(octet > 255)
        {
            string_errno = string_errno | IPV4_ERRNO;
            return BAD_IP_ADDRESS;
        }

        tmp = tmp | octet;
        tmp = tmp << 8;
        memset(str_tmp, 0, 4);

        counter_start = counter;
        counter_tmp = counter + 3;

        for(; counter <= counter_tmp; counter++)
        {
            if(str[counter] == '.')
            {
                counter++;
                break;
            }

            if((str[counter] != '.') && (counter == counter_tmp))
            {
                string_errno = string_errno | IPV4_ERRNO;
                return BAD_IP_ADDRESS;
            }

            str_tmp[counter-counter_start] = str[counter];
        }

        octet = strtoi(str_tmp, DECIMAL);

        if(octet > 255)
        {
            string_errno = string_errno | IPV4_ERRNO;
            return BAD_IP_ADDRESS;
        }

        tmp = tmp | octet;
        tmp = tmp << 8;
        memset(str_tmp, 0, 4);

        counter_start = counter;
        counter_tmp = counter + 3;

        for(; counter <= counter_tmp; counter++)
        {
            if(str[counter] == '\0')
                break;

            if((str[counter] != '.') && (counter == counter_tmp))
            {
                string_errno = string_errno | IPV4_ERRNO;
                return BAD_IP_ADDRESS;
            }

            str_tmp[counter-counter_start] = str[counter];
        }

        octet = strtoi(str_tmp, DECIMAL);

        if(octet > 255)
        {
            string_errno = string_errno | IPV4_ERRNO;
            return BAD_IP_ADDRESS;
        }

        tmp = tmp | octet;

    }

    string_errno = (uint32_t)NULL;    
    
    return tmp;

}


char* substr_find(char *str, const char *substr)
{
    while(*str != '\0')
    {
        if(bstrncmp(str, substr, strlen(substr)))
            return str;
        str++;
    }
    return NULL;
}

char* strconcat(char* str1, char* buf) //concatenate str1 and buf (first str1)
{
    char* tmp = (char*)calloc(XANIN_PMMNGR_BLOCK_SIZE);

    strcpy(tmp, buf);

    int i = 0;
    
    for(; i < strlen(str1); i++)
        buf[i] = str1[i];

    for(; i < strlen(buf) + strlen(buf); i++)
        buf[i] = tmp[i - strlen(str1)];

    buf[i] = '\0';

    free(tmp);
    return buf;
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
    return lines+1;
}

uint32_t size_of_biggest_line_get(const char* str)
{

    int max_line_size = 0;
    int current_line_size = 0;

    while (*str) 
    {
        if (*str == '\n') // do not include \x1e
        {
            if (current_line_size > max_line_size) 
                max_line_size= current_line_size;

            current_line_size = 0;
        } 
        else 
            current_line_size++;

        str++;
    }

    if (current_line_size > max_line_size) 
        max_line_size = current_line_size;

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

    if(string_length >= count) return str;

    memmove(str + (count - string_length), str, string_length);

    int i;
    for(i = 0; i < count - string_length; i++)
        str[i] = filler;

    str[count] = '\0'; // put NULL terminator

    return str;
}

char* string_align_end(char* const str, char filler, uint32_t count)
{
    int i;
    for(i = strlen(str); i < count; i++)
        str[i] = filler;

    str[i] = '\0'; // put NULL terminator

    return str;
}