
#pragma once

#include <stdarg.h>
#include <lib/libcpp/ostream.h>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/container.hpp>
#include <lib/libcpp/type_traits.h>
#include "./iterator.hpp"

namespace std
{

template<class Arr>
class ForwardArrayIterator : public std::ForwardIterator<Arr>
{
    public: 

    using this_type = ForwardArrayIterator<Arr>;

    using value_type = typename ConditionalConst<typename Arr::value_type, false>::type;
    using iterable_type = typename ConditionalConst<typename Arr::iterable_type, false>::type;
    using lreference = typename Arr::lreference;

    static constexpr Types type = Types::ForwardArrayIterator;

    private:
    this_type& perform_operation_with_bounds_check(auto operation, ForwardArrayIterator<Arr>* IteratorToBeChecked) {
        operation();

        if(!((IteratorToBeChecked->i_ptr >= IteratorToBeChecked->begin) && (IteratorToBeChecked->i_ptr <= IteratorToBeChecked->end)))
            IteratorToBeChecked->i_ptr = NULL;
        return *IteratorToBeChecked;
    }

    public:
    ForwardArrayIterator<Arr>(iterable_type ptr, Arr& arr) {
        this->i_ptr = ptr; 
        this->begin = arr.ptr;
        this->end = arr.ptr + arr.size();
    }

    ForwardArrayIterator<Arr>(const ForwardArrayIterator<Arr>& other) = default;

    this_type& operator ++ (void) override //prefix operator
    {
        return this->perform_operation_with_bounds_check([this](){this->i_ptr++;}, this);
    }

    this_type operator ++ (int) override //postfix operator
    {
        ForwardArrayIterator tmp = *this;
        this->perform_operation_with_bounds_check([this](){this->i_ptr++;}, this);

        return tmp;
    }

    this_type& operator -- (void) override //prefix operator
    {
        return this->perform_operation_with_bounds_check([this](){this->i_ptr--;}, this);
    }

    this_type operator -- (int) override //postfix operator
    {
        ForwardArrayIterator tmp = *this;
        this->perform_operation_with_bounds_check([this](){this->i_ptr--;}, this);

        return tmp;
    }

    this_type operator + (int offset) override 
    {
        ForwardArrayIterator tmp = *this;
        return this->perform_operation_with_bounds_check([&tmp, offset](){tmp.i_ptr = tmp.i_ptr + offset;}, &tmp);
    }

    this_type operator - (int offset) override 
    {
        ForwardArrayIterator tmp = *this;
        return this->perform_operation_with_bounds_check([&tmp, offset](){tmp.i_ptr = tmp.i_ptr - offset;}, &tmp);
    }

    lreference operator* (void) const override
    {
        return *this->i_ptr;
    }

    this_type& operator = (const this_type& other) override 
    {
        *this = other;
        return *this;
    }

    this_type& operator = (this_type&& other) override 
    {
        *this = other;
        other.i_ptr = NULL;
        return *this;
    }

    explicit operator bool(void) const override
    {
        return this->valid();
    }

    bool valid(void) const override
    {
        return (this->i_ptr != NULL) & (this->i_ptr >= this->begin) & (this->i_ptr < this->end);
    }

};

template<class Arr>
class ConstForwardArrayIterator : public ConstForwardIterator<Arr>
{
    public: 
    using this_type = ConstForwardArrayIterator<Arr>;

    using value_type = typename ConditionalConst<typename Arr::value_type, false>::type;
    using iterable_type = typename ConditionalConst<typename Arr::iterable_type, false>::type;
    using const_lreference = typename Arr::const_lreference;

    static constexpr Types type = Types::ForwardArrayIterator;

    private:
    this_type& perform_operation_with_bounds_check(auto operation, this_type* IteratorToBeChecked) {
        operation();

        if(!((IteratorToBeChecked->i_ptr >= IteratorToBeChecked->begin) && (IteratorToBeChecked->i_ptr <= IteratorToBeChecked->end)))
            IteratorToBeChecked->i_ptr = NULL;
        return *IteratorToBeChecked;
    }

    public:
    ConstForwardArrayIterator<Arr>(iterable_type ptr, Arr& arr) {
        this->i_ptr = ptr; 
        this->begin = arr.ptr;
        this->end = arr.ptr + arr.size();
    }

    ConstForwardArrayIterator<Arr>(const ConstForwardArrayIterator<Arr>& other) = default;


    this_type& operator ++ (void) override //prefix operator
    {
        return this->perform_operation_with_bounds_check([this](){this->i_ptr++;}, this);
    }

    this_type operator ++ (int) override //postfix operator
    {
        this_type tmp = *this;
        this->perform_operation_with_bounds_check([this](){this->i_ptr++;}, this);

        return tmp;
    }

    this_type& operator -- (void) override //prefix operator
    {
        return this->perform_operation_with_bounds_check([this](){this->i_ptr--;}, this);
    }

    this_type operator -- (int) override //postfix operator
    {
        this_type tmp = *this;
        this->perform_operation_with_bounds_check([this](){this->i_ptr--;}, this);

        return tmp;
    }

    this_type operator + (int offset) override 
    {
        this_type tmp = *this;
        return this->perform_operation_with_bounds_check([&tmp, offset](){tmp.i_ptr = tmp.i_ptr + offset;}, &tmp);
    }

    this_type operator - (int offset) override 
    {
        this_type tmp = *this;
        return this->perform_operation_with_bounds_check([&tmp, offset](){tmp.i_ptr = tmp.i_ptr - offset;}, &tmp);
    }

    const_lreference operator* (void) override
    {
        return *this->i_ptr;
    }

    this_type& operator = (const this_type& other) override 
    {
        *this = other;
        return *this;
    }

    this_type& operator = (this_type&& other) override 
    {
        *this = other;
        other.i_ptr = NULL;
        return *this;
    }

    explicit operator bool(void) const override
    {
        return this->valid();
    }

    bool valid(void) const override
    {
        return (this->i_ptr != NULL) & (this->i_ptr >= this->begin) & (this->i_ptr < this->end);
    }

};

template<class Arr>
class ReversedArrayIterator : public std::ReversedIterator<Arr>
{
    public: 
    using this_type = typename Arr::reversed_iterator;

    using value_type = typename Arr::value_type;
    using iterable_type = typename Arr::iterable_type;
    using lreference = typename Arr::lreference;

    static constexpr Types type = Types::ForwardArrayIterator;

    private:
    ReversedArrayIterator<Arr>& perform_operation_with_bounds_check(auto operation, ReversedArrayIterator<Arr>* IteratorToBeChecked) {
        operation();

        if(!((IteratorToBeChecked->i_ptr <= IteratorToBeChecked->rbegin) && (IteratorToBeChecked->i_ptr >= IteratorToBeChecked->rend)))
            IteratorToBeChecked->i_ptr = NULL;

        return *IteratorToBeChecked;
    }

    public:
    ReversedArrayIterator<Arr>(iterable_type ptr, Arr& arr) {
        this->i_ptr = ptr; 
        this->rbegin = arr.ptr + arr.size() - 1;
        this->rend = arr.ptr - 1;
    }

    ReversedArrayIterator<Arr>(const ReversedArrayIterator<Arr>& other) = default;

    this_type& operator ++ (void) override //prefix operator
    {
        return this->perform_operation_with_bounds_check([this](){this->i_ptr--;}, this);
    }

    this_type operator ++ (int) override //postfix operator
    {
        ReversedArrayIterator tmp = *this;
        this->perform_operation_with_bounds_check([this](){this->i_ptr--;}, this);

        return tmp;
    }

    this_type& operator -- (void) override //prefix operator
    {
        return this->perform_operation_with_bounds_check([this](){this->i_ptr++;}, this);
    }

    this_type operator -- (int) override //postfix operator
    {
        ReversedArrayIterator<Arr> tmp = *this;
        this->perform_operation_with_bounds_check([this](){this->i_ptr++;}, this);

        return tmp;
    }

    this_type operator + (int offset) override 
    {
        ReversedArrayIterator tmp = *this;
        return this->perform_operation_with_bounds_check([&tmp, offset](){tmp = tmp - offset;}, &tmp);
    }

    this_type operator - (int offset) override 
    {
        ReversedArrayIterator tmp = *this;
        return this->perform_operation_with_bounds_check([&tmp, offset](){tmp = tmp + offset;}, &tmp);
    }

    lreference operator* (void) const override
    {
        return *this->i_ptr;
    }

    this_type& operator = (const this_type& other) override 
    {
        *this = other;
        return *this;
    }

    this_type& operator = (this_type&& other) override 
    {
        *this = other;
        other.i_ptr = NULL;
        return *this;
    }

    explicit operator bool(void) const override
    {
        return this->valid();
    }

    bool valid() const
    {
        return (this->i_ptr != NULL) & (this->i_ptr <= this->rbegin) & (this->i_ptr > this->rend); 
    }

};


template <class T, int SIZE>
class array : Container<T>
{

    private:
    T ptr[SIZE];

    public:

    using this_type = array<T, SIZE>;

    using value_type = T;
    using iterable_type = T*;

    using lreference = T&;
    using rreference = T&&;

    using const_lreference = const T&;
    using const_rreference = const T&&;

    using forward_iterator = ForwardArrayIterator<this_type>;
    using reversed_iterator = ReversedArrayIterator<this_type>;
    using const_forward_iterator = ConstForwardArrayIterator<this_type>;
    // using const_reversed_iterator = ConstReversedArrayIterator<this_type>;

    array() = default;
    array(const array& arr) = default;
    array(std::initializer_list<T> a);

    template<typename InputIt>
    array(InputIt beg, InputIt end, T default_value) {
        int i = 0;
        
        for(; (beg != end) & (i < SIZE); beg++, i++)
            this->ptr[i] = *beg;

        for(; i < SIZE; i++)
            this->ptr[i] = default_value;
        
    }

    constexpr forward_iterator begin();
    constexpr forward_iterator end();
    constexpr reversed_iterator rbegin();
    constexpr reversed_iterator rend();

    constexpr const_forward_iterator cbegin();

    T* pointer(void);  //override;

    std::array<T, SIZE>& operator = (const std::array<T, SIZE>& other) = default;
    constexpr lreference operator[](const int index);

    constexpr bool valid_element(T& element) const;

    T get_copy(int32_t index) const;
    int find(T key);
    int find_other_than(T key);

    T& front(void); //override;
    T& back(void);  //override;

    constexpr int size(void)// override
    {
        return SIZE;
    }

    template<int OTHER_SIZE>
    auto concat(const std::array<T, OTHER_SIZE>& other) 
    {
        std::array<T, SIZE + OTHER_SIZE> tmp;

        for(int i = 0; i < SIZE; i++)
            tmp[i] = other.get_copy(i);

        for(int i = SIZE; i < SIZE + OTHER_SIZE; i++)
            tmp[i] = other.get_copy(i - SIZE);
        
        return tmp;
    }

    template<int TO_SIZE, typename InputIt>
    std::array<T, TO_SIZE> slice(InputIt&& begin)
    {

        static_assert((begin.type == Types::ReversedArrayIterator) || (begin.type == Types::ForwardArrayIterator), 
            "You need to use ArrayIterator object");

        std::array<T, TO_SIZE> tmp;

        for(int i = 0; i < TO_SIZE; i++, begin++) {
            if(!begin.valid())
                break;
            tmp[i] = *begin;
        }

        return tmp;
    }

    template<typename Arr>
    friend class ForwardArrayIterator;

    template<typename Arr>
    friend class ConstForwardArrayIterator;

    template<typename Arr>
    friend class ReversedArrayIterator;

    // void print(void);
};

template <class T, int SIZE>
array<T, SIZE>::array(std::initializer_list<T> a)
{
    auto it = a.begin();

    for(int i = 0; i < SIZE;i++) 
        this->ptr[i] = it[i];
}

template <class T, int SIZE>
constexpr ForwardArrayIterator<array<T, SIZE>> array<T, SIZE>::begin()
{
    return ForwardArrayIterator<array<T, SIZE>>(&this->ptr[0], *this);
}

template <class T, int SIZE>
constexpr ForwardArrayIterator<array<T, SIZE>> array<T, SIZE>::end() 
{
    return ForwardArrayIterator<array<T, SIZE>>(&this->ptr[SIZE], *this);
}

template <class T, int SIZE>
constexpr ReversedArrayIterator<array<T, SIZE>> array<T, SIZE>::rbegin() 
{
    return ReversedArrayIterator<array<T, SIZE>>(&ptr[SIZE - 1], *this);
}

template <class T, int SIZE>
constexpr ReversedArrayIterator<array<T, SIZE>> array<T, SIZE>::rend() 
{
    return ReversedArrayIterator<array<T, SIZE>>(ptr - 1, *this);
}

template <class T, int SIZE>
constexpr ConstForwardArrayIterator<array<T, SIZE>> array<T, SIZE>::cbegin()
{
    return ConstForwardArrayIterator<array<T, SIZE>>(&this->ptr[0], *this);
}

template <class T, int SIZE>
constexpr T& array<T, SIZE>::operator[](const int index) 
{

    if (index < 0)
    {
        if(index < SIZE * (-1))
            return *(T*)NULL;

        return this->ptr[SIZE + index];
    }

    else if(index > SIZE)
        return *(T*)NULL;

    return this->ptr[index];
}

template <class T, int SIZE>
constexpr bool array<T, SIZE>::valid_element(T& element) const
{
    return ((uint32_t)&element >= (uint32_t)this->ptr) & ((uint32_t)&element < (uint32_t)&this->ptr[SIZE]);
}

template <class T, int SIZE>
T array<T, SIZE>::get_copy(int32_t index) const
{
    return this->ptr[index];
}

template <class T, int SIZE>
int array<T, SIZE>::find(T key)
{
    for(int i = 0; i < SIZE; i++)
    {
        if(ptr[i] == key) 
            return i;
    }
    return -1;
}

template <class T, int SIZE>
int array<T, SIZE>::find_other_than(T key) 
{
    for(int i = 0; i < SIZE; i++)
    {
        if(ptr[i] != key) 
            return i;
    }
    return -1;
}

template<typename T, int SIZE>
T* array<T, SIZE>::pointer(void) 
{
    return this->ptr;
}

template<typename T, int SIZE>
T& array<T, SIZE>::front(void) 
{
    return this->ptr[0];
}

template<typename T, int SIZE>
T& array<T, SIZE>::back(void) 
{
    return this->ptr[SIZE - 1];
}

// template<typename T, int SIZE>
// void array<T, SIZE>::print(void)
// {

//     if(!this->size())
//     {
//         std::cout << "[]" << std::endl;
//         return;
//     }

//     std::cout << "[";
//     auto it = this->begin();

//     for(; it != this->end()-2; it++)
//         std::cout << *it << ", ";
    
//     std::cout << *(it + 1);
//     std::cout << "]";
// }

//global scope functions
template<typename T, int SIZE>
std::array<T, SIZE> to_array(T* ptr)
{
    std::array<T, SIZE> arr;

    for(int i = 0; i < SIZE; i++)
        arr[i] = ptr[i];
}

}
