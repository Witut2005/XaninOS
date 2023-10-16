
#pragma once

namespace std 
{

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

}