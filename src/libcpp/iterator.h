
#pragma once

#include <libcpp/type_traits.h>

template<class T>
class Iterator
{

    public: 
    
        using Type = typename std::remove_pointer<T>::type;

        Iterator(Type* ptr) : i_ptr(ptr){}
        Iterator(const Iterator<Type>& x) : i_ptr(x.i_ptr){}

        Iterator& operator ++ ()   //prefix operator
        {
            i_ptr++;
            return *this;
        }

        Iterator operator ++ (int) //postfix operator
        {
            Iterator tmp = *this;
            ++(this->i_ptr); //++(*this);

            return tmp;
        }


        Iterator& operator -- ()   //prefix operator
        {
            i_ptr--;
            return *this;
        }

        Iterator operator -- (int) //postfix operator
        {
            Iterator tmp = *this;
            --(this->i_ptr);

            return tmp;
        }


        Type operator [](uint32_t index)
        {
            return i_ptr[index];
        }


        uint32_t* operator &()
        {
            return (uint32_t*)this;
        }

        Type& operator *()
        {
            return *i_ptr;
        }

        Type& operator = (Type x)
        {
            this->i_ptr = x.i_ptr;
            return i_ptr;
        }

        bool operator == (const Iterator x)
        {
            return i_ptr == x.i_ptr;
        }

        bool operator != (const Iterator x)
        {
            return i_ptr != x.i_ptr;
        }

        uint32_t address(void)
        {
            return (uint32_t)i_ptr;
        }

    // private:
        Type* i_ptr;


};