
#pragma once

uint32_t strlen(char* a)
{

    uint32_t length = 0;

    for(char* i = a; *i != '\0' ;i++)
    {
        length++;
    }

    return length;


}

void reverseStr(char* str)
{


    char buf;
    char* ptr = str + strlen(str) - 1;


    for(int i = 0; i < (strlen(str) / 2); i++)
    {
        buf = str[i];
        str[i] = *ptr;
        *ptr = buf;
        ptr--;
    }

}

bool cmpstr(char* a,char* b)
{

    uint32_t lengtha = strlen(a);
    uint32_t lengthb = strlen(b);
    
    if(lengtha != lengthb)
        return false;

    for(char* i = a; *i != '\0' ;i++)
    {
        if(*a != *b)
            return false;
        a++;
        b++;
    }

    return true;		
}


void printNum32(uint32_t x)
{

    static char strNum[10];

    for(int i = 10; i >= 10; i++)
    {
        strNum[i] = x % 10;
        x = x / 10;
    }

}