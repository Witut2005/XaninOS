
#pragma once

namespace std
{

template<class Cont>
class ForwardIterator
{
    protected:
    typename Cont::value_type* i_ptr;

    public:
    using value_type = typename Cont::value_type;

    using lreference_type = typename Cont::lreference_type;
    using rreference_type = typename Cont::rreference_type;

    using const_lreference_type = typename Cont::const_lreference_type;
    using const_rreference_type = typename Cont::const_rreference_type;

    // ForwardIterator(value_type* ptr) {this->i_ptr = ptr;};
    // ForwardIterator(const ForwardIterator& other) = default;
    
    virtual ForwardIterator<Cont>& operator ++ (void) = 0;
    virtual ForwardIterator<Cont>&& operator ++ (int) = 0;
    virtual ForwardIterator<Cont>& operator -- (void) = 0;
    virtual ForwardIterator<Cont>&& operator -- (int) = 0;

    virtual ForwardIterator<Cont>&& operator + (int) = 0;
    virtual ForwardIterator<Cont>&& operator - (int) = 0;

    virtual value_type& operator * (void) = 0;

    virtual ForwardIterator<Cont>& operator = (ForwardIterator<Cont>&&) = 0;

    virtual bool operator == (const ForwardIterator<Cont>&) = 0;
    virtual bool operator != (const ForwardIterator<Cont>&) = 0;
    virtual operator bool(void) = 0;

    template <class Arr>
    friend class ForwardArrayIterator;

};

template<class Cont>
class ReversedIterator
{
    protected:
    typename Cont::value_type* i_ptr;

    public:
    using value_type = typename Cont::value_type;

    ReversedIterator(value_type* ptr) {this->i_ptr = ptr;};
    ReversedIterator(const ReversedIterator& other) = default;

    virtual ReversedIterator<Cont>& operator ++ (void) = 0;
    virtual ReversedIterator<Cont>&& operator ++ (int) = 0;
    virtual ReversedIterator<Cont>& operator -- (void) = 0;
    virtual ReversedIterator<Cont>&& operator -- (int) = 0;

    virtual ReversedIterator<Cont>&& operator + (int) = 0;
    virtual ReversedIterator<Cont>&& operator - (int) = 0;

    virtual value_type& operator * (void) = 0;
    virtual bool operator == (const ReversedIterator<Cont>&) = 0;
    virtual bool operator != (const ReversedIterator<Cont>&) = 0;
    virtual operator bool(void) = 0;

    template <class Arr>
    friend class ReversedArrayIterator;

};


}
