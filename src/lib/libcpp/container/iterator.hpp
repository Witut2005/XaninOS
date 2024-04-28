


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

#define NRANDOM_ACCESS_ITERATORS_DECLARE(ItType) \
template<class Cont>\
class ItType;\
template<class Cont>\
class Const##ItType;\
template<class Cont>\
class Reversed##ItType;\
template<class Cont>\
class ConstReversed##ItType;\
\
template<class Cont>\
class ItType { \
public: \
using value_type = Cont::value_type; \
ItType(Cont::value_type* ptr) : m_ptr(ptr) {}; \
ItType(ItType const& other) = default; \
ItType(Const##ItType<Cont> const& other); \
ItType(Reversed##ItType<Cont> const& other); \
ItType(ConstReversed##ItType<Cont> const& other); \
\
Cont::value_type* data(void) { return m_ptr; }\
\
ItType& operator++(void); \
ItType operator++(int); \
ItType& operator--(void); \
ItType operator--(int); \
ItType operator+(int offset); \
ItType operator-(int offset); \
Cont::value_type& operator*(); \
int operator<=>(const ItType& other); \
bool operator==(const ItType& other); \
bool operator!=(const ItType& other); \
private: \
    Cont::value_type* m_ptr; \
    friend class Const##ItType<Cont>; \
    friend class Reversed##ItType<Cont>; \
    friend class ConstReversed##ItType<Cont>; \
}; \
\
template<class Cont>\
class Const##ItType{ \
public: \
    using value_type = typename Cont::value_type;\
    Const##ItType(const value_type* ptr) : m_ptr(ptr) {}; \
    Const##ItType(ItType<Cont> const& other); \
    Const##ItType(Const##ItType const& other) = default; \
    Const##ItType(Reversed##ItType<Cont> const& other); \
    Const##ItType(ConstReversed##ItType<Cont> const& other); \
    \
    const value_type* data(void) { return m_ptr; }\
    \
    Const##ItType& operator++(void); \
    Const##ItType operator++(int); \
    Const##ItType& operator--(void); \
    Const##ItType operator--(int); \
    Const##ItType operator+(int offset); \
    Const##ItType operator-(int offset); \
    const value_type& operator*(); \
    int operator<=>(const Const##ItType& other); \
    bool operator==(const Const##ItType& other); \
    bool operator!=(const Const##ItType& other); \
private: \
    const value_type* m_ptr; \
    friend class ItType<Cont>; \
    friend class Reversed##ItType<Cont>;\
    friend class ConstReversed##ItType<Cont>;\
}; \
template<class Cont>\
class Reversed##ItType{ \
public: \
        using value_type = typename Cont::value_type;\
        Reversed##ItType(value_type* ptr) : m_ptr(ptr) {}; \
        Reversed##ItType(ItType<Cont> const& other); \
        Reversed##ItType(Const##ItType<Cont> const& other); \
        Reversed##ItType(Reversed##ItType const& other) = default; \
        Reversed##ItType(ConstReversed##ItType<Cont> const& other); \
        \
        value_type* data(void) { return m_ptr; }\
        \
        Reversed##ItType& operator++(void); \
        Reversed##ItType operator++(int); \
        Reversed##ItType& operator--(void); \
        Reversed##ItType operator--(int); \
        Reversed##ItType operator+(int offset); \
        Reversed##ItType operator-(int offset); \
        value_type& operator*(); \
        int operator<=>(const Reversed##ItType& other); \
        bool operator==(const Reversed##ItType& other); \
        bool operator!=(const Reversed##ItType& other); \
private: \
        value_type* m_ptr; \
        friend class ItType<Cont>; \
        friend class Const##ItType<Cont>;\
        friend class ConstReversed##ItType<Cont>;\
}; \
template<class Cont>\
class ConstReversed##ItType{ \
public: \
        using value_type = typename Cont::value_type;\
        ConstReversed##ItType(const value_type* ptr) : m_ptr(ptr) {}; \
        ConstReversed##ItType(ItType<Cont> const& other); \
        ConstReversed##ItType(Const##ItType<Cont> const& other); \
        ConstReversed##ItType(Reversed##ItType<Cont> const& other); \
        ConstReversed##ItType(ConstReversed##ItType const& other) = default; \
        \
        const value_type* data(void) { return m_ptr; }\
        \
        ConstReversed##ItType& operator++(void); \
        ConstReversed##ItType operator++(int); \
        ConstReversed##ItType& operator--(void); \
        ConstReversed##ItType operator--(int); \
        ConstReversed##ItType operator+(int offset); \
        ConstReversed##ItType operator-(int offset); \
        const value_type& operator*(); \
        int operator<=>(const ConstReversed##ItType& other); \
        bool operator==(const ConstReversed##ItType& other); \
        bool operator!=(const ConstReversed##ItType& other); \
private: \
        const value_type* m_ptr; \
        friend class ItType<Cont>; \
        friend class Const##ItType<Cont>;\
        friend class Reversed##ItType<Cont>;\
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

#define NDEFINE_CLASS_RANGE_OPERATIONS(Class) \
Class::Iterator begin(void); \
Class::Iterator end(void); \
Class::ConstIterator cbegin(void) const; \
Class::ConstIterator cend(void) const; \
\
Class::ReversedIterator rbegin(void); \
Class::ReversedIterator rend(void); \
Class::ConstReversedIterator crbegin(void) const; \
Class::ConstReversedIterator crend(void) const; 

#define DEFINE_ITERATORS_CONVERTION_CONSTRUCTORS(Class, ItType)\
template<class Cont>\
Class::ItType<Cont>::ItType(Const##ItType<Cont> const& other) : m_ptr(const_cast<value_type*>(other.m_ptr)) {}  \
template<class Cont>\
Class::ItType<Cont>::ItType(Reversed##ItType<Cont> const& other) : m_ptr(other.m_ptr) {} \
template<class Cont>\
Class::ItType<Cont>::ItType(ConstReversed##ItType<Cont> const& other) : m_ptr(const_cast<value_type*>(other.m_ptr)) {} \
\
template<class Cont>\
Class::Const##ItType<Cont>::Const##ItType(ItType<Cont> const& other) : m_ptr(other.m_ptr) {} \
template<class Cont>\
Class::Const##ItType<Cont>::Const##ItType(Reversed##ItType<Cont> const& other) : m_ptr(other.m_ptr) {} \
template<class Cont>\
Class::Const##ItType<Cont>::Const##ItType(ConstReversed##ItType<Cont> const& other) : m_ptr(other.m_ptr) {} \
\
template<class Cont>\
Class::Reversed##ItType<Cont>::Reversed##ItType(ItType<Cont> const& other) : m_ptr(other.m_ptr) {} \
template<class Cont>\
Class::Reversed##ItType<Cont>::Reversed##ItType(Const##ItType<Cont> const& other) : m_ptr(const_cast<value_type*>(other.m_ptr)) {} \
template<class Cont>\
Class::Reversed##ItType<Cont>::Reversed##ItType(ConstReversed##ItType<Cont> const& other) : m_ptr(const_cast<value_type*>(other.m_ptr)) {} \
\
template<class Cont>\
Class::ConstReversed##ItType<Cont>::ConstReversed##ItType(ItType<Cont> const& other) : m_ptr(other.m_ptr) {} \
template<class Cont>\
Class::ConstReversed##ItType<Cont>::ConstReversed##ItType(Const##ItType<Cont> const& other) : m_ptr(other.m_ptr) {} \
template<class Cont>\
Class::ConstReversed##ItType<Cont>::ConstReversed##ItType(Reversed##ItType<Cont> const& other) : m_ptr(other.m_ptr) {}


////////////////////////////////

#define DEFINE_ITERATOR_FUNCTIONALITY(Class, ItType, Operation, ReturnType, Arg, Functionality) \
template<class Cont>\
ReturnType Class::ItType<Cont>::operator Operation (Arg) Functionality \
template<class Cont>\
Const##ReturnType Class::Const##ItType<Cont>::operator Operation (Arg) Functionality

////////////////////////////////

#define DEFINE_ITERATOR_PLUSPLUS_PREFIX_OPERATOR(Class, ItType, Functionality) \
template<class Cont>\
Class::ItType<Cont>& Class::ItType<Cont>::operator ++ (void) Functionality \
template<class Cont>\
Class::Const##ItType<Cont>& Class::Const##ItType<Cont>::operator ++ (void) Functionality 

#define DEFINE_ITERATOR_PLUSPLUS_POSTFIX_OPERATOR(Class, ItType, Functionality) \
template<class Cont>\
Class::ItType<Cont> Class::ItType<Cont>::operator ++ (int) Functionality \
template<class Cont>\
Class::Const##ItType<Cont> Class::Const##ItType<Cont>::operator ++ (int) Functionality 

////////////////////////////////

#define DEFINE_ITERATOR_MINUSMINUS_PREFIX_OPERATOR(Class, ItType, Functionality) \
template<class Cont>\
Class::ItType<Cont>& Class::ItType<Cont>::operator -- (void) Functionality \
template<class Cont>\
Class::Const##ItType<Cont>& Class::Const##ItType<Cont>::operator -- (void) Functionality \

#define DEFINE_ITERATOR_MINUSMINUS_POSTFIX_OPERATOR(Class, ItType, Functionality) \
template<class Cont>\
Class::ItType<Cont> Class::ItType<Cont>::operator -- (int) Functionality \
template<class Cont>\
Class::Const##ItType<Cont> Class::Const##ItType<Cont>::operator -- (int) Functionality \

////////////////////////////////

#define DEFINE_ITERATOR_PLUS_OPERATOR(Class, ItType, Arg, Functionality) \
template<class Cont>\
Class::ItType<Cont> Class::ItType<Cont>::operator + (Arg) Functionality \
template<class Cont>\
Class::Const##ItType<Cont> Class::Const##ItType<Cont>::operator + (Arg) Functionality 

#define DEFINE_ITERATOR_MINUS_OPERATOR(Class, ItType, Arg, Functionality) \
template<class Cont>\
Class::ItType<Cont> Class::ItType<Cont>::operator - (Arg) Functionality \
template<class Cont>\
Class::Const##ItType<Cont> Class::Const##ItType<Cont>::operator - (Arg) Functionality 

////////////////////////////////

#define DEFINE_ITERATOR_ASTERISK_OPERATOR(Class, ItType, Functionality) \
template<class Cont>\
Cont::value_type& Class::ItType<Cont>::operator * (void) Functionality \
template<class Cont>\
const Cont::value_type& Class::Const##ItType<Cont>::operator * (void) Functionality 

////////////////////////////////

#define DEFINE_ITERATOR_SPACESHIP_OPERATOR(Class, ItType,  Functionality) \
template<class Cont>\
int Class::ItType<Cont>::operator <=> (const ItType<Cont>& other) Functionality \
template<class Cont>\
int Class::Const##ItType<Cont>::operator <=> (const Const##ItType<Cont>& other) Functionality 

#define DEFINE_ITERATOR_EQUALITY_OPERATOR(Class, ItType,  Functionality) \
template<class Cont>\
bool Class::ItType<Cont>::operator == (const ItType<Cont>& other) Functionality \
template<class Cont>\
bool Class::Const##ItType<Cont>::operator == (const Const##ItType<Cont>& other) Functionality 

#define DEFINE_ITERATOR_INEQUALITY_OPERATOR(Class, ItType,  Functionality) \
template<class Cont>\
bool Class::ItType<Cont>::operator != (const ItType<Cont>& other) Functionality \
template<class Cont>\
bool Class::Const##ItType<Cont>::operator != (const Const##ItType<Cont>& other) Functionality 

template <typename Cont>
class back_inserter
{
public:
    back_inserter(Cont& container) : m_container(container) {}

    back_inserter& operator*(void)
    {
        return *this;
    }

    back_inserter& operator = (typename Cont::value_type value)
    {
        m_container.push_back(value);
        return *this;
    }

    back_inserter& operator ++ (void) { return *this; }
    back_inserter& operator ++ (int) { return *this; }

    void operator()(typename Cont::value_type value)
    {
        m_container.push_back(value);
    }

    const Cont& container(void)
    {
        return m_container;
    }

private:
    Cont& m_container;
};

//inspired by SerenityOS -> AK/Iterator.h

template <class Cont>
class RandomAccessIterator
{
public:
    using value_type = typename Cont::value_type;

    constexpr bool is_valid(void) { return m_index >= 0 && m_index < m_container.size(); }
    constexpr bool is_end(void) { return m_index == m_container.size(); }
    constexpr bool is_rend(void) { return m_index == -1; }
    constexpr value_type* data(void) { return &m_container[m_index]; }

    constexpr bool operator==(const RandomAccessIterator& other) { return m_index == other.m_index; }
    constexpr bool operator!=(const RandomAccessIterator& other) { return m_index != other.m_index; }
    constexpr int operator<=>(const RandomAccessIterator& other) { return m_index > other.m_index ? 1 : m_index < other.m_index ? -1 : 0; }

    constexpr RandomAccessIterator& operator++(void) { m_index++; return *this; }
    constexpr RandomAccessIterator operator++(int) { m_index++; return RandomAccessIterator(m_container, m_index - 1); }

    constexpr RandomAccessIterator& operator--(void) { m_index--; return *this; }
    constexpr RandomAccessIterator operator--(int) { m_index--; return RandomAccessIterator(m_container, m_index + 1); }

    constexpr RandomAccessIterator operator+(int offset) { return RandomAccessIterator(m_container, m_index + offset); }
    constexpr RandomAccessIterator operator-(int offset) { return RandomAccessIterator(m_container, m_index - offset); }

    constexpr value_type& operator*(void) { return m_container[m_index]; }
    constexpr value_type* operator->(void) { return &m_container[m_index]; }


private:
    friend Cont;
    //Andreas Kling is a genius
    RandomAccessIterator(Cont& container, int index) : m_container(container), m_index(index) {}

    Cont& m_container;
    int m_index;
};

template <class Cont>
class ConstRandomAccessIterator
{
public:
    using value_type = typename Cont::value_type;

    constexpr bool is_valid(void) { return m_index >= 0 && m_index < m_container.size(); }
    constexpr bool is_end(void) { return m_index == m_container.size(); }
    constexpr bool is_rend(void) { return m_index == -1; }
    constexpr const value_type* data(void) { return &m_container[m_index]; }

    constexpr bool operator==(const ConstRandomAccessIterator& other) { return m_index == other.m_index; }
    constexpr bool operator!=(const ConstRandomAccessIterator& other) { return m_index != other.m_index; }
    constexpr int operator<=>(const ConstRandomAccessIterator& other) { return m_index > other.m_index ? 1 : m_index < other.m_index ? -1 : 0; }

    constexpr ConstRandomAccessIterator& operator++(void) { m_index++; return *this; }
    constexpr ConstRandomAccessIterator operator++(int) { m_index++; return ConstRandomAccessIterator(m_container, m_index - 1); }

    constexpr ConstRandomAccessIterator& operator--(void) { m_index--; return *this; }
    constexpr ConstRandomAccessIterator operator--(int) { m_index--; return ConstRandomAccessIterator(m_container, m_index + 1); }

    constexpr ConstRandomAccessIterator operator+(int offset) { return ConstRandomAccessIterator(m_container, m_index + offset); }
    constexpr ConstRandomAccessIterator operator-(int offset) { return ConstRandomAccessIterator(m_container, m_index - offset); }

    constexpr const value_type& operator*(void) { return m_container[m_index]; }
    constexpr const value_type* operator->(void) { return &m_container[m_index]; }


private:
    friend Cont;
    //Andreas Kling is a genius
    ConstRandomAccessIterator(const Cont& container, int index) : m_container(container), m_index(index) {}

    const Cont& m_container;
    int m_index;
};

template <class Cont>
class RandomAccessReversedIterator
{
public:
    using value_type = typename Cont::value_type;

    constexpr bool is_valid(void) { return m_index >= 0 && m_index < m_container.size(); }
    constexpr bool is_end(void) { return m_index == m_container.size(); }
    constexpr bool is_rend(void) { return m_index == -1; }
    constexpr value_type* data(void) { return &m_container[m_index]; }

    constexpr bool operator==(const RandomAccessReversedIterator& other) { return m_index == other.m_index; }
    constexpr bool operator!=(const RandomAccessReversedIterator& other) { return m_index != other.m_index; }
    constexpr int operator<=>(const RandomAccessReversedIterator& other) { return m_index > other.m_index ? -1 : m_index < other.m_index ? 1 : 0; }

    constexpr RandomAccessReversedIterator& operator++(void) { m_index--; return *this; }
    constexpr RandomAccessReversedIterator operator++(int) { m_index--; return RandomAccessReversedIterator(m_container, m_index + 1); }

    constexpr RandomAccessReversedIterator& operator--(void) { m_index++; return *this; }
    constexpr RandomAccessReversedIterator operator--(int) { m_index++; return RandomAccessReversedIterator(m_container, m_index - 1); }

    constexpr RandomAccessReversedIterator operator+(int offset) { return RandomAccessReversedIterator(m_container, m_index - offset); }
    constexpr RandomAccessReversedIterator operator-(int offset) { return RandomAccessReversedIterator(m_container, m_index + offset); }

    constexpr value_type& operator*(void) { return m_container[m_index]; }
    constexpr value_type* operator->(void) { return &m_container[m_index]; }


private:
    friend Cont;
    //Andreas Kling is a genius
    RandomAccessReversedIterator(Cont& container, int index) : m_container(container), m_index(index) {}

    Cont& m_container;
    int m_index;
};

template <class Cont>
class ConstRandomAccessReversedIterator
{
public:
    using value_type = typename Cont::value_type;

    constexpr bool is_valid(void) { return m_index >= 0 && m_index < m_container.size(); }
    constexpr bool is_end(void) { return m_index == m_container.size(); }
    constexpr bool is_rend(void) { return m_index == -1; }
    constexpr const value_type* data(void) { return &m_container[m_index]; }

    constexpr bool operator==(const ConstRandomAccessReversedIterator& other) { return m_index == other.m_index; }
    constexpr bool operator!=(const ConstRandomAccessReversedIterator& other) { return m_index != other.m_index; }
    constexpr int operator<=>(const ConstRandomAccessReversedIterator& other) { return m_index > other.m_index ? -1 : m_index < other.m_index ? 1 : 0; }

    constexpr ConstRandomAccessReversedIterator& operator++(void) { m_index--; return *this; }
    constexpr ConstRandomAccessReversedIterator operator++(int) { m_index--; return ConstRandomAccessReversedIterator(m_container, m_index + 1); }

    constexpr ConstRandomAccessReversedIterator& operator--(void) { m_index++; return *this; }
    constexpr ConstRandomAccessReversedIterator operator--(int) { m_index++; return ConstRandomAccessReversedIterator(m_container, m_index - 1); }

    constexpr ConstRandomAccessReversedIterator operator+(int offset) { return ConstRandomAccessReversedIterator(m_container, m_index - offset); }
    constexpr ConstRandomAccessReversedIterator operator-(int offset) { return ConstRandomAccessReversedIterator(m_container, m_index + offset); }

    constexpr const value_type& operator*(void) { return m_container[m_index]; }
    constexpr const value_type* operator->(void) { return &m_container[m_index]; }


private:
    friend Cont;
    //Andreas Kling is a genius
    ConstRandomAccessReversedIterator(const Cont& container, int index) : m_container(container), m_index(index) {}

    const Cont& m_container;
    int m_index;
};



} //namspace
