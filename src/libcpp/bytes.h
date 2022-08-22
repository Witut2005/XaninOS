

#include <stdint.h>

template <class T>
class Bytes
{
    T copy;
    uint8_t* begin_of_data; 
    uint8_t* end_of_data;
    
    public:
    uint8_t* begin(void)
    {
        return this->begin_of_data;
    }

    uint8_t* end(void)
    {
        return this->end_of_data;
    }

    // void little_endian(void)
    // {
    //     T tmp = 0;

    //     for(int i = 0; i < sizeof(this->copy); i++)
    //     {
    //         tmp = tmp << 8;
    //         tmp = tmp | (bytes & 0xFF);
    //         bytes = bytes >> 8;
    //     }

    //     this->copy = tmp;
    // }

    Bytes(T bytes)
    {
        this->copy = bytes;

        this->begin_of_data = (uint8_t*)&this->copy;
        this->end_of_data = (uint8_t*)(&(this->copy)) + sizeof(this->copy);

        T tmp = 0;

        for(int i = 0; i < sizeof(this->copy); i++)
        {
            tmp = tmp << 8;
            tmp = tmp | (bytes & 0xFF);
            bytes = bytes >> 8;
        }

        this->copy = tmp;
    }
};