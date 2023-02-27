
#pragma once

#include <libc/stdlibx.h>
#include <libcpp/list.h>
#include <libcpp/map.h>
#include <libcpp/string.h>

namespace std
{

class CommandParser 
{

    private:
    // std::List<std::string> ValueMap;
    const char* nullstr;
    int argument_counter;
    char** argv;
    char** given_arguments;

    public:

    CommandParser(char** args) : nullstr("0"), argv(args), argument_counter(0)
    {
        this->given_arguments = (char**)calloc(sizeof(char*));
    }

    void argument_add(const char* name)//, const char * type)
    {
        this->argument_counter++;
        this->given_arguments = (char**)realloc(this->given_arguments, sizeof(char**) * this->argument_counter);

        this->given_arguments[argument_counter-1] = (char*)calloc(sizeof(char) * strlen(name));

        strcpy(this->given_arguments[argument_counter-1], (char*)name);


    }

    // template <class T>
    const char* parse_arg(const char* index)
    {

        bool ok = false;
        for(int i = 0; i < this->argument_counter; i++)
        {
            if(strcmp(this->given_arguments[i], (char*)index))
                ok = true;
        }

        if(!ok)
            return nullptr;

        for(int i = 0; i < 5; i++)
        {
            if(strcmp(this->argv[i], (char*)index) && strlen(this->argv[i + 1]) > 0)
                return this->argv[i + 1];
            else 
                nullptr;
                // return this->ValueMap[index]
        }
    }

};


}