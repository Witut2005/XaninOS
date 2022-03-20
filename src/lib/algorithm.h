#pragma once

#include <lib/string.h>
#include <stddef.h>
#include <lib/stdiox.h>
char* find_character(char character, char* str)
{

	for(int i = 0; i < strlen(str); i++)
	{
		if(character == str[i])
			return &str[i];
	}
	
	return nullptr;
}

void print_array(int array[], int n)  //n=size of array
{
    for(int i=0; i<n; i++)
        xprintf("%d ", array[i]);
}
