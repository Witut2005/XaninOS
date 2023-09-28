
#pragma once

namespace std
{

template<class Cont>
class ForwardIterator
{
    protected:
    typename Cont::Type* i_ptr;

    public:
    using Type = typename Cont::Type;

    ForwardIterator(Type* ptr) {this->i_ptr = ptr;};
    ForwardIterator(const ForwardIterator& other) = default;
    virtual ForwardIterator<Cont>& operator ++ (void){asm("int 0");};
    virtual ForwardIterator<Cont> operator ++ (int){asm("int 0");};
    virtual ForwardIterator<Cont>& operator -- (void){};
    virtual ForwardIterator<Cont> operator -- (int){};
    virtual Type& operator * (void){};
    virtual bool operator == (const ForwardIterator<Cont>&){};
    virtual bool operator != (const ForwardIterator<Cont>&){};
    virtual operator bool(void){};

};

template<class Cont>
class ReversedIterator
{
    private:
    
    using Type = typename Cont::Type;

    Type* i_ptr;

    public:

    ReversedIterator(Type* ptr){this->i_ptr = ptr;};
    ReversedIterator(const ReversedIterator& other) = default;
    virtual ReversedIterator<Cont>& operator ++ (void){};
    virtual ReversedIterator<Cont> operator ++ (int){};
    virtual ReversedIterator<Cont>& operator -- (void){};
    virtual ReversedIterator<Cont> operator -- (int){};
};

}
