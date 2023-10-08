
#pragma once

namespace std
{

template<class Cont>
class ForwardIterator
{
    protected:
    typename Cont::iterable_type* i_ptr;

    public:
    
    using iterable_type = typename Cont::iterable_type;
    using value_type = typename Cont::value_type;

    using lreference = typename Cont::lreference;
    using rreference = typename Cont::rreference;

    using const_lreference = typename Cont::const_lreference;
    using const_rreference = typename Cont::const_rreference;

    virtual ForwardIterator<Cont>& operator ++ (void) = 0;
    virtual ForwardIterator<Cont>&& operator ++ (int) = 0;
    virtual ForwardIterator<Cont>& operator -- (void) = 0;
    virtual ForwardIterator<Cont>&& operator -- (int) = 0;

    virtual ForwardIterator<Cont>&& operator + (int) = 0;
    virtual ForwardIterator<Cont>&& operator - (int) = 0;

    virtual value_type& operator * (void) = 0;

    virtual ForwardIterator<Cont>& operator = (const ForwardIterator<Cont>& other) = 0;
    virtual ForwardIterator<Cont>& operator = (ForwardIterator<Cont>&&) = 0;

    virtual bool operator == (const ForwardIterator<Cont>&) = 0;
    virtual bool operator != (const ForwardIterator<Cont>&) = 0;
    virtual operator bool(void) = 0;

    template <class Arr>
    friend class ForwardArrayIterator;

    template <class Vec>
    friend class ForwardVectorIterator;

    template <class Li>
    friend class ForwardListIterator;

};

template<class Cont>
class ReversedIterator
{
    protected:
    typename Cont::iterable_type* i_ptr;

    public:

    using iterable_type = typename Cont::iterable_type;
    using value_type = typename Cont::value_type;

    virtual ReversedIterator<Cont>& operator ++ (void) = 0;
    virtual ReversedIterator<Cont>&& operator ++ (int) = 0;
    virtual ReversedIterator<Cont>& operator -- (void) = 0;
    virtual ReversedIterator<Cont>&& operator -- (int) = 0;

    virtual ReversedIterator<Cont>&& operator + (int) = 0;
    virtual ReversedIterator<Cont>&& operator - (int) = 0;

    virtual value_type& operator * (void) = 0;

    virtual ReversedIterator<Cont>& operator = (const ReversedIterator<Cont>& other) = 0;
    virtual ReversedIterator<Cont>& operator = (ReversedIterator<Cont>&&) = 0;

    virtual bool operator == (const ReversedIterator<Cont>&) = 0;
    virtual bool operator != (const ReversedIterator<Cont>&) = 0;
    virtual operator bool(void) = 0;

    template <class Arr>
    friend class ReversedArrayIterator;

    template <class Vec>
    friend class ReversedVectorIterator;

    template <class Li>
    friend class ReversedListIterator;

};


}
