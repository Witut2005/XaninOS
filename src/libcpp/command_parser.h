
#pragma once

#include <libc/stdlibx.h>
#include <libcpp/list.h>
#include <libcpp/map.h>
#include <libcpp/string.h>

namespace std
{

enum class CommandParserErrorCodes
{
    REQUIRED_ARGUMENT_NOT_GIVEN,
    NOT_SUCH_OPTION
};

class CommandParser 
{

    private:
    std::UnorderedMap<const char*, std::pair<const char*, bool>> ValueMap;
    CommandParserErrorCodes ParserErrno;
    std::string required_argument_not_given;
    const char* nullstr;
    int argument_counter;
    char** argv;
    char** given_arguments;

    public:

    CommandParser(char** args) : nullstr("\0"), argv(args), argument_counter(0), required_argument_not_given("required argument not given: ")
    {
        this->given_arguments = (char**)calloc(sizeof(char*));
    }

    void argument_add(const char* name, bool required=false)//, const char * type)
    {
        ValueMap.insert(name, std::pair(nullstr, required));
    }

    // template <class T>
    const char* parse_arg(const char* index)
    {
        
        for(int i = 0; i < 4; i++)
        {
            if(strcmp(argv[i], (char*)index))
            {
                ValueMap[index].first = argv[i+1];
                return ValueMap[index].first;
            }
        }

        if(ValueMap[index].second)
            this->ParserErrno = CommandParserErrorCodes::REQUIRED_ARGUMENT_NOT_GIVEN;
        return (this->required_argument_not_given + std::string(index)).c_str();
        // return str.c_str();

    }

};


}