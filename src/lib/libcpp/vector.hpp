
#include <stdint.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/math.h>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/algorithm.h>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/ostream.h>
#include <lib/libcpp/container.hpp>
#include <lib/libcpp/type_traits.h>

namespace std {

template<class Vec>
class ForwardVectorIterator : public std::ForwardIterator<Vec>
{

    public: 
    using this_type = ForwardVectorIterator<Vec>;

    using value_type = typename Vec::value_type;
    using iterable_type = typename Vec::iterable_type;
    using lreference = typename Vec::lreference;

    static constexpr Types type = Types::ForwardVectorIterator;

    private:
    this_type& perform_operation_with_bounds_check(auto operation, this_type* IteratorToBeChecked) {
        operation();

        if(!((IteratorToBeChecked->i_ptr >= IteratorToBeChecked->begin) && (IteratorToBeChecked->i_ptr <= IteratorToBeChecked->end)))
            IteratorToBeChecked->i_ptr = NULL;
        return *IteratorToBeChecked;
    }

    public:

    ForwardVectorIterator<Vec>(iterable_type ptr, Vec& vec) {
        // std::cout << "DEFAULT" << std::endl;
        this->i_ptr = ptr; 
        
        this->begin = vec.ptr;
        this->end = vec.ptr + vec.v_size;
    }
    ForwardVectorIterator<Vec>(const this_type& other) = default;

    constexpr const char* type_info(void)
    {
        return "ForwardVectorIterator";
    }

    this_type& operator ++ (void) override
    {
        return this->perform_operation_with_bounds_check([this](){this->i_ptr++;}, this);
    }

    this_type operator ++ (int) override 
    {
        ForwardVectorIterator tmp = *this;
        this->perform_operation_with_bounds_check([this](){this->i_ptr++;}, this);

        return tmp;
    }

    this_type& operator -- (void) override 
    {
        return this->perform_operation_with_bounds_check([this](){this->i_ptr--;}, this);
    }

    this_type operator -- (int) override 
    {
        ForwardVectorIterator tmp = *this;
        this->perform_operation_with_bounds_check([this](){this->i_ptr--;}, this);

        return tmp;
    }

    this_type operator + (int offset) override 
    {
        ForwardVectorIterator tmp = *this;
        return this->perform_operation_with_bounds_check([&tmp, offset](){tmp.i_ptr = tmp.i_ptr + offset;}, &tmp);
    }

    this_type operator - (int offset) override 
    {
        ForwardVectorIterator tmp = *this;
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

template<class Vec>
class ReversedVectorIterator : public std::ReversedIterator<Vec>
{

    public: 
    using this_type = ReversedVectorIterator<Vec>;

    using value_type = typename Vec::value_type;
    using iterable_type = typename Vec::iterable_type;
    using lreference = typename Vec::lreference;

    static constexpr Types type = Types::ReversedVectorIterator;

    private:
    void perform_operation_with_bounds_check(auto operation, ReversedVectorIterator<Vec>* IteratorToBeChecked) {
        operation();

        if(!((IteratorToBeChecked->i_ptr <= IteratorToBeChecked->rbegin) && (IteratorToBeChecked->i_ptr >= IteratorToBeChecked->rend)))
            IteratorToBeChecked->i_ptr = NULL;
    }

    public:
    ReversedVectorIterator<Vec>(iterable_type ptr, Vec& vec) {
        this->i_ptr = ptr;
    
        this->rbegin = vec.ptr + vec.v_size - 1;
        this->rend = vec.ptr - 1;
    }

    ReversedVectorIterator<Vec>(const ReversedVectorIterator<Vec>& other) = default;
    ReversedVectorIterator<Vec>(ReversedVectorIterator<Vec>&& other) {
        *this = other;
        other.i_ptr = NULL;
    }

    this_type& operator ++ (void) override //prefix operator
    {
        this->perform_operation_with_bounds_check([this](){this->i_ptr--;}, this);
        return *this;
    }

    this_type operator ++ (int) override //postfix operator
    {
        ReversedVectorIterator tmp = *this;
        this->perform_operation_with_bounds_check([this](){this->i_ptr--;}, this);

        return tmp;
    }

    this_type& operator -- (void) override //prefix operator
    {
        this->perform_operation_with_bounds_check([this](){this->i_ptr++;}, this);
        return *this;
    }

    this_type operator -- (int) override //postfix operator
    {
        ReversedVectorIterator<Vec> tmp = *this;
        this->perform_operation_with_bounds_check([this](){this->i_ptr++;}, this);

        return tmp;
    }

    this_type operator + (int offset) override 
    {
        ReversedVectorIterator tmp = *this;
        this->perform_operation_with_bounds_check([&tmp, offset](){tmp.i_ptr = tmp.i_ptr - offset;}, &tmp);

        return tmp;
    }

    this_type operator - (int offset) override 
    {
        ReversedVectorIterator tmp = *this;
        this->perform_operation_with_bounds_check([&tmp, offset](){tmp.i_ptr = tmp.i_ptr + offset;}, &tmp);
        
        return tmp;
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

    operator bool(void) const override
    {
        return this->valid();
    }

    bool valid() const
    {
        return (this->i_ptr != NULL) & (this->i_ptr <= this->rbegin) & (this->i_ptr > this->rend); 
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
    using iterable_type = T*;

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

    template<typename InputIt>
    vector(InputIt beg, InputIt end) : vector()
    {
        for(; beg != end; beg++) 
            this->push_back(*beg);
    }

    ~vector(); 

    std::vector<T>& operator = (const vector<T>& other) = default; // copy assigment operator
    std::vector<T>& operator = (vector<T>&& other); // move assigment operator

    T* pointer(void); // override;

    forward_iterator begin(void);
    forward_iterator end(void);
    reversed_iterator rbegin(void);
    reversed_iterator rend(void);

    void push_back(T item);
    void pop_back(void);

    T& front(void); // override;
    T& back(void); // override;
    T& operator [](int index); // override;

    int size(void);// override;

    template<typename Cont>
    friend class ForwardVectorIterator;
    
    template<typename Cont>
    friend class ReversedVectorIterator;

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
vector<T>::vector (std::initializer_list<T> items) : vector()
{
    int index = 0;
    for(auto it = items.begin(); it != items.end(); it++, index++)
        this->ptr[index] = *it;

    this->v_size = items.size();
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
T* vector<T>::pointer(void)
{
    return this->ptr;
}

template<typename T>
typename vector<T>::forward_iterator vector<T>::begin(void)
{
    return vector<T>::forward_iterator(this->ptr, *this);
}

template<typename T>
typename vector<T>::forward_iterator vector<T>::end(void)
{
    return vector<T>::forward_iterator(this->ptr + this->v_size, *this);
}

template<typename T>
typename vector<T>::reversed_iterator vector<T>::rbegin(void)
{
    return vector<T>::reversed_iterator(this->ptr + this->v_size - 1, *this);
}

template<typename T>
typename vector<T>::reversed_iterator vector<T>::rend(void)
{
    return vector<T>::reversed_iterator(this->ptr - 1, *this);
}

template<typename T>
void vector<T>::push_back(T item)
{
    this->ptr = (T*)realloc(this->ptr, SIZE_OF(T) * (this->v_size + 1));
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
