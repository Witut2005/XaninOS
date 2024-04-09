
#pragma once

#include <limits.h>

namespace std
{

template <class Cont>
class Iterator
{
protected:
    using iterable_type = typename Cont::iterable_type;
    iterable_type i_ptr{ nullptr };

public:
    Cont& container;

    Iterator<Cont>() : container(*(Cont*)NULL) {}
    Iterator<Cont>(iterable_type i_ptr, Cont& container) : i_ptr(i_ptr), container(container) {}

    iterable_type pointer()
    {
        return this->i_ptr;
    }

    bool operator == (Iterator<Cont> rhs) const;
    bool operator != (Iterator<Cont> rhs) const;

};

template <typename Cont>
bool Iterator<Cont>::operator==(Iterator <Cont> rhs) const
{
    return i_ptr == rhs.i_ptr;
}

template <typename Cont>
bool Iterator<Cont>::operator!=(Iterator <Cont> rhs) const
{
    return !(i_ptr == rhs.i_ptr);
}

template <class Cont>
class ForwardIterator : public Iterator<Cont>
{

protected:
    typename Cont::iterable_type begin;
    typename Cont::iterable_type end;

public:
    using iterator_type = typename Cont::forward_iterator;

    using iterable_type = typename Cont::iterable_type;
    using value_type = typename Cont::value_type;
    using lreference = typename Cont::lreference;

    ForwardIterator<Cont>() : Iterator<Cont>()
    {
        this->begin = (iterable_type)INT_MAX;
        this->end = (iterable_type)INT_MAX;
    }

    ForwardIterator<Cont>(iterable_type i_ptr, Cont& container) : Iterator<Cont>(i_ptr, container),
        begin(container.begin_ptr()), end(container.end_ptr()) {}

    // virtual constexpr const char* type_info(void);

    virtual iterator_type& operator++(void) = 0;
    virtual iterator_type operator++(int) = 0;
    virtual iterator_type& operator--(void) = 0;
    virtual iterator_type operator--(int) = 0;

    virtual iterator_type operator+(int) = 0;
    virtual iterator_type operator-(int) = 0;

    virtual lreference operator*(void) const = 0;
    // virtual iterator_type operator-> (void) const = 0;

    virtual iterator_type& operator=(const iterator_type& other) = 0;
    virtual iterator_type& operator=(iterator_type&&) = 0;

    virtual explicit operator bool(void) const = 0;
    virtual bool valid(void) const = 0;

    template <class Arr>
    friend class ForwardArrayIterator;

    template <class Vec>
    friend class ForwardVectorIterator;

    template <class Li>
    friend class ForwardListIterator;
};

template <class Cont>
class ReversedIterator : public Iterator<Cont>
{
protected:
    typename Cont::iterable_type rbegin;
    typename Cont::iterable_type rend;

public:
    using iterator_type = typename Cont::reversed_iterator;

    using iterable_type = typename Cont::iterable_type;
    using value_type = typename Cont::value_type;
    using lreference = typename Cont::lreference;

    ReversedIterator<Cont>()
    {
        this->i_ptr = NULL;
        this->container = NULL;
        this->rbegin = (iterable_type)INT_MAX;
        this->rend = (iterable_type)INT_MAX;
    }

    ReversedIterator<Cont>(iterable_type i_ptr, Cont& container) : Iterator<Cont>(i_ptr, container),
        rbegin(container.rbegin_ptr()), rend(container.rend_ptr()) {}
    virtual iterator_type& operator++(void) = 0;
    virtual iterator_type operator++(int) = 0;
    virtual iterator_type& operator--(void) = 0;
    virtual iterator_type operator--(int) = 0;

    virtual iterator_type operator+(int) = 0;
    virtual iterator_type operator-(int) = 0;

    virtual lreference operator*(void) const = 0;

    virtual iterator_type& operator=(const iterator_type& other) = 0;
    virtual iterator_type& operator=(iterator_type&&) = 0;

    virtual explicit operator bool(void) const = 0;
    virtual bool valid(void) const = 0;

    template <class Arr>
    friend class ReversedArrayIterator;

    template <class Vec>
    friend class ReversedVectorIterator;

    template <class Li>
    friend class ReversedListIterator;
};

template <class Cont>
class ConstIterator
{
protected:
    using iterable_type = typename Cont::iterable_type;
    iterable_type i_ptr;

public:
    Cont& container;

    ConstIterator<Cont>(iterable_type i_ptr, Cont& container) : i_ptr(i_ptr), container(container) {}

    template <typename InputIt>
    friend bool operator==(InputIt lhs, InputIt rhs);

    template <typename InputIt>
    friend bool operator!=(InputIt lhs, InputIt rhs);
};

template <class Cont>
class ConstForwardIterator : public ConstIterator<Cont>
{
protected:
    typename Cont::iterable_type begin;
    typename Cont::iterable_type end;

public:
    using iterator_type = typename Cont::const_forward_iterator;

    using iterable_type = typename Cont::iterable_type;
    using value_type = typename Cont::value_type;
    using const_lreference = typename Cont::const_lreference;

    ConstForwardIterator<Cont>()
    {
        this->i_ptr = NULL;
        this->container = NULL;
        this->begin = (iterable_type)INT_MAX;
        this->end = (iterable_type)INT_MAX;
    }

    ConstForwardIterator<Cont>(iterable_type i_ptr, Cont& container) : ConstIterator<Cont>(i_ptr, container),
        begin(container.begin_ptr()),
        end(container.end_ptr()) {}

    virtual iterator_type& operator++(void) = 0;
    virtual iterator_type operator++(int) = 0;
    virtual iterator_type& operator--(void) = 0;
    virtual iterator_type operator--(int) = 0;

    virtual iterator_type operator+(int) = 0;
    virtual iterator_type operator-(int) = 0;

    virtual const_lreference operator*(void) const = 0;

    virtual iterator_type& operator=(const iterator_type& other) = 0;
    virtual iterator_type& operator=(iterator_type&&) = 0;

    virtual explicit operator bool(void) const = 0;
    virtual bool valid(void) const = 0;

    // template <class Arr>
    // friend clanstForwardArrayIterator;

    template <class Vec>
    friend class ForwardVectorIterator;

    template <class Li>
    friend class ForwardListIterator;
};

template <class Cont>
class ConstReversedIterator : public ConstIterator<Cont>
{
protected:
    typename Cont::iterable_type rbegin;
    typename Cont::iterable_type rend;

public:
    using iterator_type = typename Cont::const_reversed_iterator;

    using iterable_type = typename Cont::iterable_type;
    using value_type = typename Cont::value_type;
    using const_lreference = typename Cont::const_lreference;

    ConstReversedIterator<Cont>()
    {
        this->i_ptr = NULL;
        this->container = NULL;
        this->rbegin = (iterable_type)INT_MAX;
        this->rend = (iterable_type)INT_MAX;
    }

    ConstReversedIterator<Cont>(iterable_type i_ptr, Cont& container) : ConstIterator<Cont>(i_ptr, container),
        rbegin(container.rbegin_ptr()),
        rend(container.rend_ptr()) {}
    virtual iterator_type& operator++(void) = 0;
    virtual iterator_type operator++(int) = 0;
    virtual iterator_type& operator--(void) = 0;
    virtual iterator_type operator--(int) = 0;

    virtual iterator_type operator+(int) = 0;
    virtual iterator_type operator-(int) = 0;

    virtual const_lreference operator*(void) const = 0;

    virtual iterator_type& operator=(const iterator_type& other) = 0;
    virtual iterator_type& operator=(iterator_type&&) = 0;

    virtual explicit operator bool(void) const = 0;
    virtual bool valid(void) const = 0;
};

}
