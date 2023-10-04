
#include <stdint.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/math.h>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/algorithm.h>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/ostream.h>
#include <lib/libcpp/container.hpp>

namespace std {

template<class Vec>
class ForwardVectorIterator : public std::ForwardIterator<Vec>
{

    public: 
    using value_type = typename Vec::value_type;

    using lreference = typename Vec::lreference;
    using rreference = typename Vec::rreference;

    using const_lreference = typename Vec::const_lreference;
    using const_rreference = typename Vec::const_rreference;

    ForwardVectorIterator<Vec>(value_type* ptr){this->i_ptr = ptr;}
    ForwardVectorIterator<Vec>(const ForwardIterator<Vec>& other) {this->i_ptr = other.i_ptr;}
    ForwardVectorIterator<Vec>(const ForwardArrayIterator<Vec>& other) {this->i_ptr = other.i_ptr;}
    // ForwardArrayIterator<Arr>(ForwardIterator<Arr>&& other) 

    constexpr const char* type_info(void)
    {
        return "ForwardVectorIterator";
    }

    ForwardIterator<Vec>& operator ++ (void) override //prefix operator
    {
        this->i_ptr++;
        return *this;
    }

    ForwardIterator<Vec>&& operator ++ (int) override //postfix operator
    {
        ++(this->i_ptr); 
        ForwardArrayIterator tmp = *this;

        return std::move(tmp);
    }

    ForwardIterator<Vec>& operator -- (void) override //prefix operator
    {
        this->i_ptr--;
        return *this;
    }

    ForwardIterator<Vec>&& operator -- (int) override //postfix operator
    {
        --(this->i_ptr);
        ForwardArrayIterator tmp = *this;

        return std::move(tmp);
    }

    ForwardIterator<Vec>&& operator + (int offset) override 
    {
        ForwardArrayIterator tmp = *this;
        tmp.i_ptr = tmp.i_ptr + offset;

        return std::move(tmp);
    }

    ForwardIterator<Vec>&& operator - (int offset) override 
    {
        ForwardArrayIterator tmp = *this;
        tmp.i_ptr = tmp.i_ptr - offset;

        return std::move(tmp);
    }

    lreference operator* (void) override
    {
        return *this->i_ptr;
    }

    ForwardIterator<Vec>& operator = (ForwardIterator<Vec>&& other) override 
    {
        this->i_ptr = other.i_ptr;
    }

    bool operator == (const ForwardIterator<Vec>& x) override
    {
        return this->i_ptr == x.i_ptr;
    }

    bool operator != (const ForwardIterator<Vec>& x) override
    {
        return this->i_ptr != x.i_ptr;
    }

    operator bool(void) override
    {
        return this->i_ptr != NULL;
    }

};

template<class Vec>
class ReversedVectorIterator : public std::ReversedIterator<Vec>
{

    public: 
    using value_type = typename Vec::value_type;

    using lreference = typename Vec::lreference;
    using rreference = typename Vec::rreference;

    using const_lreference = typename Vec::const_lreference;
    using const_rreference = typename Vec::const_rreference;

    constexpr const char* type_info(void)
    {
        return "ReversedVectorIterator";
    }

    ReversedVectorIterator(value_type* ptr) : ReversedIterator<Vec>(ptr){}
    ReversedVectorIterator(const ReversedArrayIterator<Vec>& other) : ReversedIterator<Vec>(other){}

    ReversedIterator<Vec>& operator ++ (void) override //prefix operator
    {
        this->i_ptr--;
        return *this;
    }

    ReversedIterator<Vec>&& operator ++ (int) override //postfix operator
    {
        --(this->i_ptr); 
        ReversedVectorIterator tmp = *this;

        return std::move(tmp);
    }

    ReversedIterator<Vec>& operator -- (void) override //prefix operator
    {
        this->i_ptr++;
        return *this;
    }

    ReversedIterator<Vec>&& operator -- (int) override //postfix operator
    {
        ++(this->i_ptr);
        ReversedVectorIterator<Vec> tmp = *this;

        return std::move(tmp);
    }

    ReversedIterator<Vec>&& operator + (int offset) override 
    {
        ReversedVectorIterator tmp = *this;
        tmp.i_ptr = tmp.i_ptr + offset;

        return std::move(tmp);
    }

    ReversedIterator<Vec>&& operator - (int offset) override 
    {
        ReversedVectorIterator tmp = *this;
        tmp.i_ptr = tmp.i_ptr - offset;
        
        return std::move(tmp);
    }

    lreference operator* (void) override
    {
        return *this->i_ptr;
    }

    bool operator == (const ReversedIterator<Vec>& x) override
    {
        return this->i_ptr == x.i_ptr;
    }

    bool operator != (const ReversedIterator<Vec>& x) override
    {
        return this->i_ptr != x.i_ptr;
    }

    operator bool(void) override
    {
        return this->i_ptr != NULL;
    }

};



    
template <typename T>
class vector : Container<T>
{
    
private:
    T* ptr;
    uint32_t v_size = 0;
    
public:
    using value_type = T;

    using lreference = T&;
    using rreference = T&&;

    using const_lreference = const T&;
    using const_rreference = const T&&;

    using forward_iterator = ForwardVectorIterator<vector<T>>;
    using reversed_iterator = ReversedVectorIterator<vector<T>>;

    vector();
    vector(const vector<T>& other) =  default; // copy constructor
    vector(vector<T>&& other); // move constructor
    vector (std::initializer_list<T> items); 
    ~vector(); 

    std::vector<T>& operator = (const vector<T>& other) = default; // copy assigment operator
    std::vector<T>& operator = (vector<T>&& other); // move assigment operator

    T* pointer_get(void) override;

    forward_iterator begin(void);
    forward_iterator end(void);
    reversed_iterator rbegin(void);
    reversed_iterator rend(void);

    void push_back(T item);
    void pop_back(void);

    T& front(void) override;
    T& back(void) override;
    T& operator [](int index) override;

    int size(void) override;
    // void print(void) override;
};

template<typename T>
vector<T>::vector()
{
    this->ptr = (T*)calloc(SIZE_OF(T));
}

template<typename T>
vector<T>::vector(vector<T>&& other) 
{
    *this = (const vector<T>&)other;

    other.ptr = NULL;
    other.v_size = 0;
}

template<typename T>
vector<T>::vector (std::initializer_list<T> items)
{
    this->ptr = (T*)calloc(SIZE_OF(T) * items.v_size());
    int index = 0;
    for(auto it = items.begin(); it != items.end(); it++, index++)
        this->ptr[index] = *it;

    this->v_size = items.v_size();
}

template<typename T>
vector<T>::~vector()
{
    free(this->ptr);
}

template<typename T>
vector<T>& vector<T>::operator = (std::vector<T>&& other)
{
    if(this == &other)
        return *this;

    *this = (const vector<T>&)other;

    other.ptr = NULL;
    other.v_size = 0;
}

template<typename T>
T* vector<T>::pointer_get(void)
{
    return this->ptr;
}

template<typename T>
typename vector<T>::forward_iterator vector<T>::begin(void)
{
    return vector<T>::forward_iterator(this->ptr);
}

template<typename T>
typename vector<T>::forward_iterator vector<T>::end(void)
{
    return vector<T>::forward_iterator(this->ptr + this->v_size);
}

template<typename T>
typename vector<T>::reversed_iterator vector<T>::rbegin(void)
{
    return vector<T>::reversed_iterator(this->ptr + this->v_size - 1);
}

template<typename T>
typename vector<T>::reversed_iterator vector<T>::rend(void)
{
    return vector<T>::reversed_iterator(this->ptr - 1);
}

template<typename T>
void vector<T>::push_back(T item)
{
    this->ptr = (T*)realloc(this->ptr, SIZE_OF(T) * this->v_size);
    ptr[this->v_size++] = item;
}

template<typename T>
void vector<T>::pop_back(void)
{
    if(!this->v_size)
        return;
    
    T tmp = *(this->ptr + this->v_size - 1);
    *(this->ptr + this->v_size - 1) = (T)NULL;
    this->ptr = (T*)realloc(this->ptr, SIZE_OF(T) * (--this->v_size));
}

template<typename T>
T& vector<T>::front(void)
{
    return this->ptr[0];
}

template<typename T>
T& vector<T>::back(void)
{
    return this->ptr[this->v_size - 1];
}

template<typename T>
T& vector<T>::operator [](int index)
{

    if(index < 0)
    {
        if(abs(index) > this->v_size) 
            return *ptr;

        return *(this->end() + index);
    }

    else if(index >= this->v_size)
        return *this->end();
    
    return *(this->ptr+index);
}

template<typename T>
int vector<T>::size(void)
{
    return this->v_size;
}

// template<typename T>
// void vector<T>::print(void)
// {
//     if(!this->v_size)
//     {
//         std::cout << "[]" << std::endl;
//         return;
//     }

//     std::cout << "[";

//     auto it = this->begin();
//     for(; it != this->end()-1; it++)
//         std::cout << *it << ", ";
    
//     std::cout << *it;
//     std::cout << "]";
// }

}
