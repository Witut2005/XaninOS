
#include <stdint.h>
#include <lib/libc/stdlibx.h>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/algorithm.h>
#include <lib/libcpp/utility.h>


namespace std{
    
template <class T>
class vector
{
    
private:
    T* begin_ptr;
    uint32_t size;
    
public:
    
    #define null 0
    
    T* begin(void)
    {
        return begin_ptr;
    }
    
    T* end(void)
    {
        return begin_ptr + this->size;
    }
    
    void push(T item)
    {
        begin_ptr = (T*)realloc(begin_ptr, SIZE_OF(T) * this->size);
        begin_ptr[size++] = item;
    }

    T pop(void)
    {
        if(!size)
            return (T)null;
        
        T tmp = *(begin_ptr + this->size - 1);
        *(begin_ptr + this->size - 1) = (T)null;
        begin_ptr = (T*)realloc(begin_ptr, SIZE_OF(T) * (--this->size));
        return tmp;
    }
    
    T& operator [](uint32_t index)
    {
        if(index > (this->size - 1))
            return *this->end();
        
        return *(begin_ptr+index);
    }
    
    vector (std::initializer_list<T> items)
    {
        this->begin_ptr = (T*)calloc(SIZE_OF(T) * items.size());
        int index = 0;
        for(auto it = items.begin(); it != items.end(); it++, index++)
            this->begin_ptr[index] = *it;

        this->size = items.size();
    }
    
    vector()
    {
        this->begin_ptr = (T*)calloc(SIZE_OF(T));
        this->size = 0;
    }

    ~vector()
    {
        free(this->begin_ptr);
    }
    
    void print(void)
    {
        std::cout << "[";
        T* it;
        for(it = this->begin(); it != this->end()-1; it++)
        {
            std::cout << *it << ", ";
        }
        
        std::cout << *it;
        std::cout << "]";
    }
    
    
};

}
