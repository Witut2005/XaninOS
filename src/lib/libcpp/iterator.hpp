
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

    ForwardIterator(value_type* ptr) {this->i_ptr = ptr;};
    ForwardIterator(const ForwardIterator& other) = default;
    
    virtual ForwardIterator<Cont>& operator ++ (void){};
    virtual ForwardIterator<Cont>&& operator ++ (int){};
    virtual ForwardIterator<Cont>& operator -- (void){};
    virtual ForwardIterator<Cont>&& operator -- (int){};

    virtual ForwardIterator<Cont>&& operator + (int){};
    virtual ForwardIterator<Cont>&& operator - (int){};

    virtual value_type& operator * (void){};
    virtual bool operator == (const ForwardIterator<Cont>&){};
    virtual bool operator != (const ForwardIterator<Cont>&){};
    virtual operator bool(void){};

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
    virtual ReversedIterator<Cont>& operator ++ (void){};
    virtual ReversedIterator<Cont>&& operator ++ (int){};
    virtual ReversedIterator<Cont>& operator -- (void){};
    virtual ReversedIterator<Cont>&& operator -- (int){};
    virtual value_type& operator * (void){};
    virtual bool operator == (const ReversedIterator<Cont>&){};
    virtual bool operator != (const ReversedIterator<Cont>&){};
    virtual operator bool(void){};

};


}
