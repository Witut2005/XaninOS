
#pragma once

#include <stdint.h>

struct PointerAddressCounter
{
    uint8_t* address;
    uint32_t counter;
};

template <class T>
class shared_ptr
{

    private:
        T* ptr;
        static PointerAddressCounter ShareCounter;

    public:

        shared_ptr(T* given_ptr)
        {
            this->ptr = given_ptr; 
            this->ShareCounter.counter++;            
        }

        T* get(void)
        {
            return this->ptr;
        }
    
        shared_ptr& operator = (T* given_ptr)
        {
            this->ptr = given_ptr;
            return *this;
        }

        T operator * (void)
        {
            return *ptr;
        }

};

template <class T>
PointerAddressCounter shared_ptr<T>::ShareCounter;


template <class T>
class unique_ptr 
{

    private:
        T* ptr;
        static PointerAddressCounter UniqueCounter;

    public:

        unique_ptr(T* given_ptr)
        {
            ptr = given_ptr; 
            this->UniqueCounter.counter++;            
        }

        T* get(void)
        {
            return this->ptr;
        }
    
        unique_ptr& operator = (T* given_ptr)
        {
            this->ptr = given_ptr;
            return *this;
        }

        T operator * (void)
        {
            return *ptr;
        }

};

template <class T>
PointerAddressCounter unique_ptr<T>::UniqueCounter;

template <class T>
class weak_ptr 
{

    private:
        T* ptr;
        static PointerAddressCounter WeakCounter;

    public:

        weak_ptr(T* given_ptr)
        {
            this->ptr = given_ptr; 
            this->WeakCounter.counter++;            
        }

        T* get(void)
        {
            return this->ptr;
        }
    
        weak_ptr& operator = (T* given_ptr)
        {
            this->ptr = given_ptr;
            return *this;
        }

        T operator * (void)
        {
            return *ptr;
        }

};

template <class T>
PointerAddressCounter weak_ptr<T>::WeakCounter;
