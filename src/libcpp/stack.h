

#pragma once

#include <stddef.h>
#include <libcpp/cstdlib.h>

namespace std
{

    template<class T>
    class stack
    {
        private:

        T* stack_pointer;
        uint32_t size;

        public:

        stack()
        {
            stack_pointer = (T*)malloc(sizeof(T));       
            size = 0;
        }

        ~stack()
        {
            free(stack_pointer);
        }



        void push(T val)
        {
            stack_pointer = (T*)realloc(stack_pointer, size,++size);
            *(stack_pointer + size - 1) = val; 
        }

        void pop()
        {
            stack_pointer =(T*)realloc(stack_pointer, size, --size);
        }

        T top()
        {
            return *(stack_pointer + size - 1);
        }

        constexpr bool empty()
        {
            return size == 0;
        }
        

    };


}
