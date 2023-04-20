
#pragma once

#include <libc/stdlibx.h>
#include <libcpp/list.h>
#include <libcpp/map.h>
#include <libcpp/string.h>

namespace std
{

enum CommandParserErrorCodes
{
    REQUIRED_ARGUMENT_NOT_GIVEN = 1 << 0,
    NOT_SUCH_OPTION = 1 << 1
};

class CommandParser 
{

    private:
    std::UnorderedMap<const char*, std::pair<const char*, bool>> ValueMap;
    uint32_t errno;
    std::string required_argument_not_given;
    const char* nullstr;
    int argument_counter;
    char** argv;
    char** given_arguments;

    public:

    CommandParser(char** args) : nullstr("\0"), argv(args), argument_counter(0), required_argument_not_given("required argument not given: "), errno(0){}

    void argument_add(const char* name, bool required=false)//, const char * type)
    {
        ValueMap.insert(name, std::pair(nullstr, required));
    }

    uint32_t errno_get(void) 
    {
        return this->errno;
    }

    void errno_clear(void)
    {
        this->errno = (uint32_t)NULL;
    }

    // template <class T>
    template <typename K>
    static K parse_arg(CommandParser& parser, const char* index);


};

template<typename K>
K CommandParser::parse_arg(CommandParser& parser, const char* index)
{

    bool no_such_element = true;
    
    for(int i = 0; i < 4; i++)
    {
        if(strcmp(parser.argv[i], (char*)index))
        {
            parser.ValueMap[index].first = parser.argv[i+1];
            no_such_element = false;
        }
    }


    if(parser.ValueMap[index].second & no_such_element)
    {
        parser.errno |= CommandParserErrorCodes::REQUIRED_ARGUMENT_NOT_GIVEN;
        return (K)NULL;
    }

    if constexpr(is_char_ptr(K))
    {
        K tmp = parser.ValueMap[index].first;
        return tmp;
    }

    else if constexpr(is_int(K))
    {
        K tmp = strtoi((char*)parser.ValueMap[index].first, DECIMAL);
        return tmp;
    }

    else if constexpr(is_short(K))
    {
        K tmp = (int16_t)strtoi((char*)parser.ValueMap[index].first, DECIMAL);
        return tmp;
    }


    else if constexpr(is_char(K))
    {
        K tmp = parser.ValueMap[index].first[0];
        return tmp;
    }



    return (K)NULL;

}

}