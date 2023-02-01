
#include <libc/string.h>
#include <stdarg.h>
#include <stdint.h>
#include <libc/colors.h>
#include <headers/macros.h>
#include <terminal/vty.h>
#include <libc/stdlibx.h>
#include <libc/math.h>
#include <keyboard/key_map.h>
#include <libc/memory.h>


uint32_t strlen(const char* a)
{

    if(a == nullptr)
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


    uint8_t length = strlen(str);

    char buf;
    char* end = str + strlen(str) - 1;


    for(int i = 0; i < (strlen(str) / 2); i++)
    {
        buf = str[i];
        str[i] = *end;
        *end = buf;
        end--;
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

uint32_t strncmp(char* a, const char* b, size_t string_size)
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


    buf = reverse_string(buf);
    *(buf + i) = '\0';
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
    
    //uint16_t* debug_cursor = (uint16_t*)0xb8000;

    for(i = 0; x != 0; i++)
    {
        buf[i] = hex_values[x % 16]; 
        x = x / 16;
    }


    buf = reverse_string(buf);
    //*(buf + i) = '\0';
    
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

    uint32_t sum = 0;

    if(format > 16) 
        format = 16;

    if(format == 16 && (strncmp((char*)str, "0x", 2) || strncmp((char*)str, "0b", 2) || strncmp((char*)str, "0o", 2)))
        str += 2;

    uint16_t* tmp_text = (uint16_t*)VGA_TEXT_MEMORY;
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
    
    return sum;

}


uint32_t str2ipv4(char* str)
{
    uint32_t tmp = 0;
    char str_tmp[4] = {'\0'}; 
    uint32_t octet;

    memcpy(str_tmp, str, 3);

    octet = strtoi(str_tmp, 10);
    if(octet > 0xFF)
        octet = 0xFF;

    tmp = octet << 8;

    memcpy(str_tmp, str + 4, 3);

    octet = strtoi(str_tmp, 10);
    if(octet > 0xFF)
        octet = 0xFF;

    tmp = tmp | octet;
    tmp = tmp << 8;

    memcpy(str_tmp, str + 8, 3);

    octet = strtoi(str_tmp, 10);
    if(octet > 0xFF)
        octet = 0xFF;

    tmp = tmp | octet;
    tmp = tmp << 8;

    memcpy(str_tmp, str + 12, 3);

    octet = strtoi(str_tmp, 10);
    if(octet > 0xFF)
        octet = 0xFF;

    tmp = tmp | octet;


    return tmp;

}


char* substr_find(char *str, const char *substr)
{
    while(*str != '\0')
    {
        if(strncmp(str, substr, strlen(substr)))
            return str;
        str++;
    }
    return nullptr;
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

