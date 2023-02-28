
#pragma once

#include <libcpp/cstring.h>
#include <libc/stdlibx.h>
#include <libcpp/iterator.h>

namespace std
{

class string
{
    

    public: 

    using Type = char;

    size_t reserve(uint32_t size)
    {
        string_data = (char*)calloc(size * sizeof(char));
    }

    string() {string_data = (char*)malloc(4096);} 
    ~string() {free(string_data);}

    string(char* str)
    {
        string_data = (char*)calloc(sizeof(char) * strlen(str));
        for(int i = 0; i < strlen(str); i++) 
            string_data[i] = str[i];
         
    }

    string(const char* str)
    {

        string_data = (char*)calloc(sizeof(char) * strlen((char*)str));
        for(int i = 0; i < strlen((char*)str); i++) 
            string_data[i] = str[i];

    }
    
    string(string& str)
    {
        strcpy(this->string_data, str.string_data);  
    }

    string(string&& str)
    {
        this->string_data = str.string_data;
        str.string_data = NULL;

    }

    string operator = (const char* x)
    {
        realloc(string_data, strlen(x));
        strcpy(string_data, x);
    }

    string operator = (std::string x)
    {
        realloc(string_data, strlen(x.c_str()));
        strcpy(string_data, x.c_str());
    }

    inline bool operator == (std::string x) 
    {
        return strcmp(this->c_str(), x.c_str()) == 0;
    }

    inline bool operator!=(std::string x) 
    {
        return strcmp(this->c_str(), x.c_str()) != 0;
    }

    char operator [](uint32_t index)
    {
        return this->string_data[index];
    }

    uint32_t size()
    {
        char* tmp = this->c_str();
        uint32_t size = 0;

        while(*tmp != '\0')
        {
            size++;
            tmp++;
        }

        return size;
    }


    void* operator new (size_t size)
    {
        return malloc(1 * sizeof(string));
    }

    void operator delete (void* element)
    {
        std::string* delete_ptr = (std::string*)element;
        free(delete_ptr->c_str());
        delete_ptr->string_data = NULL;
    }


    char* c_str() const
    {
        return string_data;
    }

    std::string operator + (std::string second)
    {

        char* c = (char*)malloc(second.length() + this->length());
        
        int i = 0;
        for(; this->c_str()[i] != '\0'; i++)
            c[i] = this->c_str()[i];
        
        int j = 0;
        for(; second.c_str()[j] != '\0'; i++, j++)
            c[i] = second.c_str()[j];

        std::string result(c);
        return result;

    }

    std::string operator + (const char* second)
    {

        char* c = (char*)malloc(strlen(second) + this->length());
        
        int i = 0;
        for(; this->c_str()[i] != '\0'; i++)
            c[i] = this->c_str()[i];
        
        int j = 0;
        for(; second[j] != '\0'; i++, j++)
            c[i] = second[j];

        std::string result(c);
        return result;

    }

    uint32_t length() const
    {
        if(string_data == NULL)
            return 0;
        else 
            return strlen(string_data); 
    }


    Iterator<char> begin() const
    {
        return string_data;
    }


    Iterator<char> end() const
    {
        return string_data + strlen(string_data);
    }

    private:
    char* string_data;

};

}

