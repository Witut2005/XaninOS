

#include <stdint.h>

namespace std
{

template <typename T>
class Bytes
{
private:
    T copy;
    uint8_t* begin_of_data;
    uint8_t* end_of_data;

public:

    Bytes(const Bytes& other) = default;
    Bytes(T bytes);
    uint8_t* begin(void);
    uint8_t* end(void);
    void switch_endiannes(void);

};

template <typename T>
Bytes<T>::Bytes(T bytes)
{
    this->copy = bytes;

    this->begin_of_data = (uint8_t*)&this->copy;
    this->end_of_data = (uint8_t*)(&(this->copy)) + sizeof(this->copy);

    T tmp = 0;

    for (int i = 0; i < sizeof(this->copy); i++)
    {
        tmp = tmp << 8;
        tmp = tmp | (bytes & 0xFF);
        bytes = bytes >> 8;
    }

    this->copy = tmp;
}

template <typename T>
uint8_t* Bytes<T>::begin(void)
{
    return this->begin_of_data;
}

template <typename T>
uint8_t* Bytes<T>::end(void)
{
    return this->end_of_data;
}

template <typename T>
void Bytes<T>::switch_endiannes(void)
{
    T tmp = 0;
    T result = 0;

    for (int i = 0; i < sizeof(this->copy); i++)
    {
        tmp = tmp << 8;
        tmp = tmp | (this->copy & 0xFF);
        this->copy = this->copy >> 8;
    }

    this->copy = tmp;
}


}