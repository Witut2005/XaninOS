
#pragma once

#include <lib/libcpp/container/iterator.hpp>

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

    using iterable_type = typename Arr::iterable_type;
    using value_type = typename Arr::value_type;
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

    const_lreference operator* (void) const override
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

//====================REVERSED_ITERATORS=====================//

template<class Arr>
class ReversedArrayIterator : public std::ReversedIterator<Arr>
{
    public: 
    using this_type = ReversedArrayIterator<Arr>;

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

template<class Arr>
class ConstReversedArrayIterator : public std::ConstReversedIterator<Arr>
{
    public: 
    using this_type = ConstReversedArrayIterator<Arr>;

    using value_type = typename Arr::value_type;
    using iterable_type = typename Arr::iterable_type;
    using const_lreference = typename Arr::const_lreference;

    static constexpr Types type = Types::ReversedArrayIterator;

    private:
    ConstReversedArrayIterator<Arr>& perform_operation_with_bounds_check(auto operation, ConstReversedArrayIterator<Arr>* IteratorToBeChecked) {
        operation();

        if(!((IteratorToBeChecked->i_ptr <= IteratorToBeChecked->rbegin) && (IteratorToBeChecked->i_ptr >= IteratorToBeChecked->rend)))
            IteratorToBeChecked->i_ptr = NULL;

        return *IteratorToBeChecked;
    }

    public:
    ConstReversedArrayIterator<Arr>(iterable_type ptr, Arr& arr) {
        this->i_ptr = ptr; 
        this->rbegin = arr.ptr + arr.size() - 1;
        this->rend = arr.ptr - 1;
    }

    ConstReversedArrayIterator<Arr>(const this_type& other) = default;

    this_type& operator ++ (void) override //prefix operator
    {
        return this->perform_operation_with_bounds_check([this](){this->i_ptr--;}, this);
    }

    this_type operator ++ (int) override //postfix operator
    {
        this_type tmp = *this;
        this->perform_operation_with_bounds_check([this](){this->i_ptr--;}, this);

        return tmp;
    }

    this_type& operator -- (void) override //prefix operator
    {
        return this->perform_operation_with_bounds_check([this](){this->i_ptr++;}, this);
    }

    this_type operator -- (int) override //postfix operator
    {
        this_type tmp = *this;
        this->perform_operation_with_bounds_check([this](){this->i_ptr++;}, this);

        return tmp;
    }

    this_type operator + (int offset) override 
    {
        this_type tmp = *this;
        return this->perform_operation_with_bounds_check([&tmp, offset](){tmp = tmp - offset;}, &tmp);
    }

    this_type operator - (int offset) override 
    {
        this_type tmp = *this;
        return this->perform_operation_with_bounds_check([&tmp, offset](){tmp = tmp + offset;}, &tmp);
    }

    const_lreference operator* (void) const override
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


}