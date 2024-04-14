
#pragma once

#include <limits.h>
#include <lib/libcpp/type_traits.h>

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

    Iterator() : container(*(Cont*)NULL) {}
    Iterator(iterable_type i_ptr, Cont& container) : i_ptr(i_ptr), container(container) {}

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

    ForwardIterator() : Iterator<Cont>()
    {
        this->begin = (iterable_type)INT_MAX;
        this->end = (iterable_type)INT_MAX;
    }

    ForwardIterator(iterable_type i_ptr, Cont& container) : Iterator<Cont>(i_ptr, container),
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

    ReversedIterator()
    {
        this->i_ptr = NULL;
        this->container = NULL;
        this->rbegin = (iterable_type)INT_MAX;
        this->rend = (iterable_type)INT_MAX;
    }

    ReversedIterator(iterable_type i_ptr, Cont& container) : Iterator<Cont>(i_ptr, container),
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

    ConstIterator(iterable_type i_ptr, Cont& container) : i_ptr(i_ptr), container(container) {}

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

    ConstForwardIterator()
    {
        this->i_ptr = NULL;
        this->container = NULL;
        this->begin = (iterable_type)INT_MAX;
        this->end = (iterable_type)INT_MAX;
    }

    ConstForwardIterator(iterable_type i_ptr, Cont& container) : ConstIterator<Cont>(i_ptr, container),
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

    ConstReversedIterator()
    {
        this->i_ptr = NULL;
        this->container = NULL;
        this->rbegin = (iterable_type)INT_MAX;
        this->rend = (iterable_type)INT_MAX;
    }

    ConstReversedIterator(iterable_type i_ptr, Cont& container) : ConstIterator<Cont>(i_ptr, container),
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

#define RANDOM_ACCESS_ITERATORS_DECLARE(ItType, StoredType) \
class ItType;\
class Const##ItType;\
class Reversed##ItType;\
class ConstReversed##ItType;\
\
class ItType { \
public: \
    ItType(StoredType* ptr) : m_ptr(ptr) {}; \
    ItType(ItType const& other) = default; \
    ItType(Const##ItType const& other); \
    ItType(Reversed##ItType const& other); \
    ItType(ConstReversed##ItType const& other); \
    \
    StoredType* data(void) { return m_ptr; }\
    \
    ItType& operator++(void); \
    ItType operator++(int); \
    ItType& operator--(void); \
    ItType operator--(int); \
    ItType operator+(int offset); \
    ItType operator-(int offset); \
    StoredType& operator*(); \
    int operator<=>(const ItType& other); \
    bool operator==(const ItType& other); \
    bool operator!=(const ItType& other); \
private: \
    StoredType* m_ptr; \
    friend class Const##ItType; \
    friend class Reversed##ItType; \
    friend class ConstReversed##ItType; \
}; \
\
class Const##ItType{ \
public: \
    Const##ItType(const StoredType * ptr) : m_ptr(ptr) {}; \
    Const##ItType(ItType const& other); \
    Const##ItType(Const##ItType const& other) = default; \
    Const##ItType(Reversed##ItType const& other); \
    Const##ItType(ConstReversed##ItType const& other); \
    \
    const StoredType* data(void){ return m_ptr; }\
    \
    Const##ItType& operator++(void); \
    Const##ItType operator++(int); \
    Const##ItType& operator--(void); \
    Const##ItType operator--(int); \
    Const##ItType operator+(int offset); \
    Const##ItType operator-(int offset); \
    const StoredType& operator*(); \
    int operator<=>(const Const##ItType& other); \
    bool operator==(const Const##ItType& other); \
    bool operator!=(const Const##ItType& other); \
private: \
    const StoredType* m_ptr; \
        friend class ItType; \
        friend class Reversed##ItType;\
        friend class ConstReversed##ItType;\
}; \
class Reversed##ItType{ \
public: \
        Reversed##ItType(StoredType * ptr) : m_ptr(ptr) {}; \
        Reversed##ItType(ItType const& other); \
        Reversed##ItType(Const##ItType const& other); \
        Reversed##ItType(Reversed##ItType const& other) = default; \
        Reversed##ItType(ConstReversed##ItType const& other); \
        \
        StoredType* data(void) { return m_ptr; }\
        \
        Reversed##ItType& operator++(void); \
        Reversed##ItType operator++(int); \
        Reversed##ItType& operator--(void); \
        Reversed##ItType operator--(int); \
        Reversed##ItType operator+(int offset); \
        Reversed##ItType operator-(int offset); \
        StoredType& operator*(); \
        int operator<=>(const Reversed##ItType& other); \
        bool operator==(const Reversed##ItType& other); \
        bool operator!=(const Reversed##ItType& other); \
private: \
        StoredType* m_ptr; \
        friend class ItType; \
        friend class Const##ItType;\
        friend class ConstReversed##ItType;\
}; \
class ConstReversed##ItType{ \
public: \
        ConstReversed##ItType(const StoredType * ptr) : m_ptr(ptr) {}; \
        ConstReversed##ItType(ItType const& other); \
        ConstReversed##ItType(Const##ItType const& other); \
        ConstReversed##ItType(Reversed##ItType const& other); \
        ConstReversed##ItType(ConstReversed##ItType const& other) = default; \
        \
        const StoredType* data(void){ return m_ptr; }\
        \
        ConstReversed##ItType& operator++(void); \
        ConstReversed##ItType operator++(int); \
        ConstReversed##ItType& operator--(void); \
        ConstReversed##ItType operator--(int); \
        ConstReversed##ItType operator+(int offset); \
        ConstReversed##ItType operator-(int offset); \
        const StoredType& operator*(); \
        int operator<=>(const ConstReversed##ItType& other); \
        bool operator==(const ConstReversed##ItType& other); \
        bool operator!=(const ConstReversed##ItType& other); \
private: \
        const StoredType* m_ptr; \
        friend class ItType; \
        friend class Const##ItType;\
        friend class Reversed##ItType;\
};

/////////////////////////////////////////////////////

#define BASIC_ITERATOR_DECLARE(ItType, StoredType) \
class ItType{ \
public: \
    ItType(StoredType* ptr) : m_ptr(ptr) {}; \
    ItType(ItType const& other) = default; \
    ItType& operator++(void); \
    ItType operator++(int); \
    ItType& operator--(void); \
    ItType operator--(int); \
    StoredType& operator*(); \
    bool operator==(const ItType& other); \
    bool operator!=(const ItType& other); \
private: \
    StoredType* m_ptr; 
// };

#define BASIC_CONSTANT_ITERATOR_DECLARE(ItType, StoredType) \
class ItType{ \
public: \
    ItType(StoredType* ptr) : m_ptr(ptr) {}; \
    ItType(ItType const& other) = default; \
    ItType& operator++(void); \
    ItType operator++(int); \
    ItType& operator--(void); \
    ItType operator--(int); \
    const StoredType& operator*(); \
    bool operator==(const ItType& other); \
    bool operator!=(const ItType& other); \
private: \
    const StoredType* m_ptr; 
// };

#define DEFINE_CLASS_RANGE_OPERATIONS(ItType) \
ItType begin(void); \
ItType end(void); \
Const##ItType cbegin(void) const; \
Const##ItType cend(void) const; \
\
Reversed##ItType rbegin(void); \
Reversed##ItType rend(void); \
ConstReversed##ItType crbegin(void) const; \
ConstReversed##ItType crend(void) const; 

#define DEFINE_ITERATORS_CONVERTION_CONSTRUCTORS(ItType)\
ItType::ItType(Const##ItType const& other) : m_ptr(const_cast<char*>(other.m_ptr)) {} \
ItType::ItType(Reversed##ItType const& other) : m_ptr(other.m_ptr) {} \
ItType::ItType(ConstReversed##ItType const& other) : m_ptr(const_cast<char*>(other.m_ptr)) {} \
\
Const##ItType::Const##ItType(ItType const& other) : m_ptr(other.m_ptr) {} \
Const##ItType::Const##ItType(Reversed##ItType const& other) : m_ptr(other.m_ptr) {} \
Const##ItType::Const##ItType(ConstReversed##ItType const& other) : m_ptr(other.m_ptr) {} \
\
Reversed##ItType::Reversed##ItType(ItType const& other) : m_ptr(other.m_ptr) {} \
Reversed##ItType::Reversed##ItType(Const##ItType const& other) : m_ptr(const_cast<char*>(other.m_ptr)) {} \
Reversed##ItType::Reversed##ItType(ConstReversed##ItType const& other) : m_ptr(const_cast<char*>(other.m_ptr)) {} \
\
ConstReversed##ItType::ConstReversed##ItType(ItType const& other) : m_ptr(other.m_ptr) {} \
ConstReversed##ItType::ConstReversed##ItType(Const##ItType const& other) : m_ptr(other.m_ptr) {} \
ConstReversed##ItType::ConstReversed##ItType(Reversed##ItType const& other) : m_ptr(other.m_ptr) {} 


////////////////////////////////

#define DEFINE_ITERATOR_FUNCTIONALITY(ItType, Operation, ReturnType, Arg, Functionality) \
ReturnType ItType::operator Operation (Arg) Functionality \
Const##ReturnType Const##ItType::operator Operation (Arg) Functionality

////////////////////////////////

#define DEFINE_ITERATOR_PLUSPLUS_PREFIX_OPERATOR(ItType, Functionality) \
ItType& ItType::operator ++ (void) Functionality \
Const##ItType& Const##ItType::operator ++ (void) Functionality 

#define DEFINE_ITERATOR_PLUSPLUS_POSTFIX_OPERATOR(ItType, Functionality) \
ItType ItType::operator ++ (int) Functionality \
Const##ItType Const##ItType::operator ++ (int) Functionality 

////////////////////////////////

#define DEFINE_ITERATOR_MINUSMINUS_PREFIX_OPERATOR(ItType, Functionality) \
ItType& ItType::operator -- (void) Functionality \
Const##ItType& Const##ItType::operator -- (void) Functionality \

#define DEFINE_ITERATOR_MINUSMINUS_POSTFIX_OPERATOR(ItType, Functionality) \
ItType ItType::operator -- (int) Functionality \
Const##ItType Const##ItType::operator -- (int) Functionality \

////////////////////////////////

#define DEFINE_ITERATOR_PLUS_OPERATOR(ItType, Arg, Functionality) \
ItType ItType::operator + (Arg) Functionality \
Const##ItType Const##ItType::operator + (Arg) Functionality 

#define DEFINE_ITERATOR_MINUS_OPERATOR(ItType, Arg, Functionality) \
ItType ItType::operator - (Arg) Functionality \
Const##ItType Const##ItType::operator - (Arg) Functionality 

////////////////////////////////

#define DEFINE_ITERATOR_ASTERISK_OPERATOR(ItType, ReturnType, Functionality) \
ReturnType ItType::operator * (void) Functionality \
const ReturnType Const##ItType::operator * (void) Functionality 

////////////////////////////////

#define DEFINE_ITERATOR_SPACESHIP_OPERATOR(ItType,  Functionality) \
int ItType::operator <=> (const ItType& other) Functionality \
int Const##ItType::operator <=> (const Const##ItType& other) Functionality 

#define DEFINE_ITERATOR_EQUALITY_OPERATOR(ItType,  Functionality) \
bool ItType::operator == (const ItType& other) Functionality \
bool Const##ItType::operator == (const Const##ItType& other) Functionality 

#define DEFINE_ITERATOR_INEQUALITY_OPERATOR(ItType,  Functionality) \
bool ItType::operator != (const ItType& other) Functionality \
bool Const##ItType::operator != (const Const##ItType& other) Functionality 

} //namspace

