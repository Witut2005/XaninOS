
#include <stdint.h>
#include <lib/libc/stdlibx.h>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/algorithm.h>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/ostream.h>

namespace std {

template<class Vec>
class ForwardVectorIterator : public std::ForwardIterator<Vec>
{

    public: 
    using value_type = typename Vec::value_type;

    using lreference_type = typename Vec::lreference_type;
    using rreference_type = typename Vec::rreference_type;

    using const_lreference_type = typename Vec::const_lreference_type;
    using const_rreference_type = typename Vec::const_rreference_type;

    ForwardVectorIterator<Vec>(value_type* ptr){this->i_ptr = ptr;}
    ForwardVectorIterator<Vec>(const ForwardIterator<Vec>& other) {this->i_ptr = other.i_ptr;}
    ForwardVectorIterator<Vec>(const ForwardArrayIterator<Vec>& other) {this->i_ptr = other.i_ptr;}
    // ForwardArrayIterator<Arr>(ForwardIterator<Arr>&& other) 

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

        for(int i = 0; i < offset; i++)
            tmp.i_ptr++;

        return std::move(tmp);
    }

    ForwardIterator<Vec>&& operator - (int offset) override 
    {
        ForwardArrayIterator tmp = *this;
        
        for(int i = 0; i < offset; i++)
            tmp.i_ptr--;

        return std::move(tmp);
    }

    lreference_type operator* (void) override
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

    using lreference_type = typename Vec::lreference_type;
    using rreference_type = typename Vec::rreference_type;

    using const_lreference_type = typename Vec::const_lreference_type;
    using const_rreference_type = typename Vec::const_rreference_type;

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
        ReversedArrayIterator tmp = *this;

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

        for(int i = 0; i < offset; i++)
            tmp.i_ptr++;

        return std::move(tmp);
    }

    ReversedIterator<Vec>&& operator - (int offset) override 
    {
        ReversedVectorIterator tmp = *this;
        
        for(int i = 0; i < offset; i++)
            tmp.i_ptr--;

        return std::move(tmp);
    }

    lreference_type operator* (void) override
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
class vector
{
    
private:
    T* ptr;
    uint32_t size;
    
public:
    
    using value_type = T;

    using lreference_type = T&;
    using rreference_type = T&&;

    using const_lreference_type = const T&;
    using const_rreference_type = const T&&;

    using forward_iterator = ForwardArrayIterator<vector<T>>;
    using reversed_iterator = ReversedArrayIterator<vector<T>>;

    vector()
    {
        this->ptr = (T*)calloc(SIZE_OF(T));
        this->size = 0;
    }

    vector(const vector<T>& other) =  default;

    vector(vector<T>&& other) 
    {
        this->ptr = other.ptr;
        other.ptr = NULL;
        other.size = 0;
    }

    vector (std::initializer_list<T> items)
    {
        this->ptr = (T*)calloc(SIZE_OF(T) * items.size());
        int index = 0;
        for(auto it = items.begin(); it != items.end(); it++, index++)
            this->ptr[index] = *it;

        this->size = items.size();
    }
    
    ~vector()
    {
        free(this->ptr);
    }

    std::vector<T>& operator = (const std::vector<T>& other) = default;
    std::vector<T>& operator = (std::vector<T>&& other)
    {
        this->ptr = other.ptr;
        other.ptr = NULL;
        other.size = 0;
    }

    T* pointer_get(void)
    {
        return this->ptr;
    }
    
    T* begin(void)
    {
        return ptr;
    }
    
    T* end(void)
    {
        return ptr + this->size;
    }
    
    void push(T item)
    {
        ptr = (T*)realloc(ptr, SIZE_OF(T) * this->size);
        ptr[size++] = item;
    }

    void pop(void)
    {
        if(!this->size)
            return;
        
        T tmp = *(ptr + this->size - 1);
        *(ptr + this->size - 1) = (T)NULL;
        ptr = (T*)realloc(ptr, SIZE_OF(T) * (--this->size));
    }

    T& front(void)
    {
        return this->ptr[0];
    }

    T& back(void)
    {
        return this->ptr[this->size - 1];
    }
    
    T& operator [](uint32_t index)
    {
        if(index > (this->size - 1))
            return *this->end();
        
        return *(ptr+index);
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
