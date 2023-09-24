
#include "./command_parser.h"

void std::CommandParser::argument_add(const char* name, bool required)
{
    ValueMap.insert(name, std::pair(nullstr, required));
}

uint32_t std::CommandParser::errno_get(void) 
{
    return this->errno;
}

void std::CommandParser::errno_clear(void)
{
    this->errno = (uint32_t)NULL;
}