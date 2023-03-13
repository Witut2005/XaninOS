
#pragma once

#include <libcpp/cstring.h>
#include <libc/stdlibx.h>
#include <libc/stdiox.h>

namespace std
{

class ReversedStringIterator
{
       public: 
    

        ReversedStringIterator(char* ptr) : i_ptr(ptr){}

        ReversedStringIterator& operator ++ ()   //prefix operator
        {
            i_ptr--;
            return *this;
        }

        ReversedStringIterator operator ++ (int) //postfix operator
        {
            ReversedStringIterator tmp = *this;
            --(this->i_ptr); //++(*this);

            return tmp;
        }


        ReversedStringIterator& operator -- ()   //prefix operator
        {
            i_ptr++;
            return *this;
        }

        ReversedStringIterator operator -- (int) //postfix operator
        {
            ReversedStringIterator tmp = *this;
            ++(this->i_ptr);

            return tmp;
        }

        char& operator *()
        {
            return *i_ptr;
        }

        bool operator == (const ReversedStringIterator x)
        {
            return i_ptr == x.i_ptr;
        }

        bool operator != (const ReversedStringIterator x)
        {
            return i_ptr != x.i_ptr;
        }

    private:
        char* i_ptr;


};


class StringIterator
{
       public: 
    

        StringIterator(char* ptr) : i_ptr(ptr){}

        StringIterator& operator ++ ()   //prefix operator
        {
            i_ptr++;
            return *this;
        }

        StringIterator operator ++ (int) //postfix operator
        {
            StringIterator tmp = *this;
            ++(this->i_ptr); //++(*this);

            return tmp;
        }


        StringIterator& operator -- ()   //prefix operator
        {
            i_ptr--;
            return *this;
        }

        StringIterator operator -- (int) //postfix operator
        {
            StringIterator tmp = *this;
            --(this->i_ptr);

            return tmp;
        }

        char& operator *()
        {
            return *i_ptr;
        }

        bool operator == (const StringIterator x)
        {
            return i_ptr == x.i_ptr;
        }

        bool operator != (const StringIterator x)
        {
            return i_ptr != x.i_ptr;
        }

    private:
        char* i_ptr;


};

class string
{
    

    public: 

    using Type = char;
    using Iterator = StringIterator;

    size_t reserve(uint32_t size)
    {
        string_data = (char*)calloc(size * sizeof(char));
        return size;
    }

    string() {this->string_data = (char*)calloc(4096);} 
    
    string(StringIterator beg, StringIterator end)
    {
        this->string_data = (char*)calloc(4096);
        int i = 0;
        for(auto it = beg; it != end; it++, i++) 
            this->string_data[i] = *it;
    }

    string(ReversedStringIterator rbeg, ReversedStringIterator rend)
    {
        this->string_data = (char*)calloc(4096);

        int i = 0;
        for(auto it = rbeg; it != rend; it++, i++) 
            this->string_data[i] = *it;
    }

    ~string() {free(string_data);}

    string(char* str)
    {
        string_data = (char*)calloc(sizeof(char) * strlen(str));
        for(int i = 0; i < strlen(str); i++) 
            string_data[i] = str[i];
         
    }

    string(const char* str)
    {
        this->string_data = (char*)calloc(sizeof(char) * strlen((char*)str));
        for(int i = 0; i < strlen((char*)str); i++) 
            string_data[i] = str[i];

    }
    
    string(const string& str)
    {
        this->string_data = (char*)calloc(sizeof(char) * strlen((char*)str.c_str()));
        strcpy(this->string_data, str.c_str());  
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
        return *this;
    }

    string operator = (std::string x)
    {
        realloc(string_data, strlen(x.c_str()));
        strcpy(string_data, x.c_str());
        return *this;
    }

    inline bool operator == (std::string x) 
    {
        return strcmp(this->c_str(), x.c_str()) == 0;
    }

    inline bool operator!=(std::string x) 
    {
        return strcmp(this->c_str(), x.c_str()) != 0;
    }

    char& operator [](uint32_t index)
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

    std::string operator + (char character)
    {
        this->string_data[strlen(this->string_data)] = character;
        return *this;
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


    StringIterator begin() const
    {
        return string_data;
    }

    ReversedStringIterator rbegin() const
    {
        return string_data + strlen(string_data) - 1;
    }


    StringIterator end() const
    {
        return string_data + strlen(string_data);
    }

    ReversedStringIterator rend() const
    {
        return string_data-1;
    }

    private:
    char* string_data;

};

}

