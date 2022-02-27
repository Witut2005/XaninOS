
#include <lib/string.h>
#include <stddef.h>

char* find_character(char character, char* str)
{

	for(int i = 0; i < strlen(str); i++)
	{
		
		if(character == str[i])
			return &str[i];
	
	}

	return nullptr;

}
