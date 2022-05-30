
#include <stdarg.h>

namespace std
{

template <class T, int X>
class array
{

    private:
    T arr[X];



    public:
    array(array& arr)
    {
        for(int i = 0; i < X; i++)
            this->arr[i] = arr[i];  
    }

    array(T start = 0, ...)
    {
        va_list arg;
        va_start(arg, start);

        arr[0] = start;

        for(int i = 1; i < X;i++) 
        {
            arr[i] = va_arg(arg, T);
        }
    }

    T* begin()
    {
        return &arr[0];
    }

    T* end()
    {
        return &arr[X - 1];
    }

    T& operator[](uint32_t index)
    {
        return arr[index];
    }




};


}
