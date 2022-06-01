
#include <stdarg.h>
#include <libcpp/initializer_list.hpp>

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


template <class T, int X>
class array
{

    private:
    T arr[X];

    public:

    using Type = T;
    using iterator = ArrayIterator< array<Type, X> >;

    array(array& arr)
    {
        for(int i = 0; i < X; i++)
            this->arr[i] = arr[i];  
    }

    array(std::initializer_list<T> a)
    {
        
        auto it = a.begin();

        for(int i = 0; i < X;i++) 
        {
            arr[i] = it[i];
        }
    }

    T* begin()
    {
        return &arr[0];
    }

    T* end()
    {
        return &arr[X];
    }

    T& operator[](uint32_t index)
    {
        return arr[index];
    }




};


}
