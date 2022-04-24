
#pragma once

#include <lib/math.h>

uint32_t strlen(char* a)
{

    uint32_t length = 0;

    for(char* i = a; *i != '\0' ;i++)
    {
        length++;
    }
    

    return length;


}

char* set_string(char* ptr, char* str)
{
    for(int i = 0; i < strlen(str); i++)
        ptr[i] = str[i];
    return ptr;
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

char* float_to_string(float number, char* str)
{
    uint32_t* float_ptr = (uint32_t*)&number;
    bool sign_bit = (*float_ptr & (1 << 31) >> 31);
    uint8_t exponent = (*float_ptr & (0xFF << 23) >> 23);
    exponent -= 127;
    exponent = pow(2,exponent);
    uint32_t mantissa = (*float_ptr & 0x7FFFFF);

    uint32_t number_high;
    uint32_t number_low;

    int i = 0;

    number_high = exponent * mantissa;

    if(sign_bit)
    {
        str[i] = '-';
        i++;
    }




}


uint32_t cmpstr(char* a,char* b)
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

    int i = 0;

    if(!x)
    {
        buf[0] = '0';
        buf[1] = '0';
        buf[2] = '\0';
        return buf;
    }
    
    //uint16_t* debug_cursor = (uint16_t*)0xb8000;

    for(int i = 0; i < how_many_chars; i++)
        buf[i] = '0';

    for(i = 0; x != 0; i++)
    {
        buf[i] = hex_values[x % 16]; 
        x = x / 16;
    }


    buf = reverse_string(buf);
    //*(buf + i) = '\0';
    
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



uint32_t strtoi(char* str, uint8_t format)
{

    uint32_t sum = 0;

    if(format > 16) 
        format = 16;

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

        if(str[i] >= '0' && str[i] <= '9')
            str[i] = str[i] - '0';

        else if(str[i] >= 'a' && str[i] <= 'f')
            str[i] = str[i] - 'a' + 0xa;
        
        else
            continue;

        sum += str[i];        
    }
    
    return sum;

}


