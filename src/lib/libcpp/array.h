
#pragma once

#include <stdarg.h>
#include <lib/libcpp/ostream.h>
#include <lib/libcpp/initializer_list.hpp>

namespace std
{


template<class Arr>
class ArrayIterator
{
    public: 
    
        using Type = typename Arr::Type;

        ArrayIterator(Type* ptr) : i_ptr(ptr){}

        ArrayIterator& operator ++ ()   //prefix operator
        {
            i_ptr++;
            return *this;
        }

        ArrayIterator operator ++ (int) //postfix operator
        {
            ArrayIterator tmp = *this;
            ++(this->i_ptr); //++(*this);

            return tmp;
        }


        ArrayIterator& operator -- ()   //prefix operator
        {
            i_ptr--;
            return *this;
        }

        ArrayIterator operator -- (int) //postfix operator
        {
            ArrayIterator tmp = *this;
            --(this->i_ptr);

            return tmp;
        }


        // Type operator [](uint32_t index)
        // {
        //     return i_ptr[index];
        // }


        uint32_t* operator &()
        {
            return (uint32_t*)this;
        }

        Type& operator *()
        {
            return *i_ptr;
        }

        bool operator == (const ArrayIterator x)
        {
            return i_ptr == x.i_ptr;
        }

        bool operator != (const ArrayIterator x)
        {
            return i_ptr != x.i_ptr;
        }

    private:
        Type* i_ptr;


};


template <class T, int SIZE>
class array
{

    private:
    T arr[SIZE];

    public:

    using Type = T;
    using iterator = ArrayIterator< array<T, SIZE> >;

    array()
    {
        std::cout << "Array" << std::endl;
    }

    array(const array& arr)
    {
        for(int i = 0; i < SIZE; i++)
            this->arr[i] = arr[i];  
    }

    array(std::initializer_list<T> a)
    {
        auto it = a.begin();

        for(int i = 0; i < SIZE;i++) 
            arr[i] = it[i];
    }

    T* begin()
    {
        return &arr[0];
    }

    T* end()
    {
        return &arr[SIZE];
    }

    T& operator[](int32_t index)
    {
        if(index < 0)
            return arr[SIZE + index];
        return arr[index];
    }

    int find(T key)
    {
        for(int i = 0; i < SIZE; i++)
        {
            if(arr[i] == key) 
                return i;
        }
        return -1;
    }

    int find_other_than(T key)
    {
        for(int i = 0; i < SIZE; i++)
        {
            if(arr[i] != key) 
                return i;
        }
        return -1;
    }

};


}
