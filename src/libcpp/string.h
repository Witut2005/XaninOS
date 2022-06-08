
#pragma once

#include <libcpp/cstring.h>
#include <libcpp/cstdlib.h>

namespace std
{

class string
{
    public: 

    size_t reserve(uint32_t size)
    {
        string_data = (char*)malloc(size * sizeof(char));
    }

    string() {string_data = (char*)malloc(PMMNGR_BLOCK_SIZE);} 

    string(char* str)
    {
        string_data = (char*)malloc(sizeof(char) * strlen(str));
        for(int i = 0; i < strlen(str); i++) 
            string_data[i] = str[i];
         
    }

    string(const char* str)
    {

        string_data = (char*)malloc(sizeof(char) * strlen((char*)str));
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
        str.string_data = nullptr;

    }

    string operator=(const char* x)
    {
        realloc(string_data, strlen(string_data), strlen(x));
        strcpy(string_data, x);
    }

    inline bool operator==(std::string x) 
    {
        return strcmp(this->c_str(), x.c_str()) == 0;
    }

    inline bool operator!=(std::string x) 
    {
        return strcmp(this->c_str(), x.c_str()) != 0;
    }

    std::string operator+(std::string x) 
    {

        uint32_t size = 0;
        std::string tmp(*this); 

        

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
        delete_ptr->string_data = nullptr;
    }


    char* c_str()
    {
        return string_data;
    }

    uint32_t length()
    {
        if(string_data == nullptr)
            return 0;
        else 
            return strlen(string_data); 
    }


    uint32_t* begin()
    {
        return reinterpret_cast<uint32_t*> (string_data);
    }


    uint32_t* end()
    {
        return reinterpret_cast<uint32_t*> (string_data + length());
    }

    private:
    char* string_data;

};

}

