


#pragma once

#include <limits.h>
#include <lib/libcpp/functional.hpp>
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
class RandomAccessIterator;
template <class Cont>
class ConstRandomAccessIterator;
template <class Cont>
class RandomAccessReversedIterator;
template <class Cont>
class ConstRandomAccessReversedIterator;

template <class Cont>
class RandomAccessIterator
{
public:
    using value_type = typename Cont::value_type;
    RandomAccessIterator(const RandomAccessReversedIterator<Cont>& other) : m_container(other.m_container), m_index(other.m_index) {}

    constexpr bool is_valid(void) { return m_index >= 0 && m_index < m_container.get().size(); }
    constexpr bool is_end(void) { return m_index == m_container.get().size(); }
    constexpr bool is_rend(void) { return m_index == -1; }
    constexpr value_type* data(void) { return &m_container.get().data()[m_index]; }

    constexpr RandomAccessIterator& operator=(const RandomAccessIterator& other) = default;

    constexpr RandomAccessIterator& operator++(void) { m_index++; return *this; }
    constexpr RandomAccessIterator operator++(int) { m_index++; return RandomAccessIterator(m_container, m_index - 1); }

    constexpr RandomAccessIterator& operator--(void) { m_index--; return *this; }
    constexpr RandomAccessIterator operator--(int) { m_index--; return RandomAccessIterator(m_container, m_index + 1); }

    constexpr RandomAccessIterator operator+(int offset) { return RandomAccessIterator(m_container, m_index + offset); }
    constexpr RandomAccessIterator operator-(int offset) { return RandomAccessIterator(m_container, m_index - offset); }

    constexpr value_type& operator*(void) { return m_container.get().data()[m_index]; }
    constexpr value_type* operator->(void) { return &m_container.get().data()[m_index]; }

    constexpr bool operator==(const RandomAccessIterator& other) { return m_index == other.m_index; }
    constexpr bool operator!=(const RandomAccessIterator& other) { return m_index != other.m_index; }
    constexpr int operator<=>(const RandomAccessIterator& other) { return m_index > other.m_index ? 1 : m_index < other.m_index ? -1 : 0; }

private:
    friend Cont;
    //Andreas Kling is a genius
    RandomAccessIterator(Cont& container, int index) : m_container(container), m_index(index) {}
    RandomAccessIterator(const reference_wrapper<Cont>& container, int index) : m_container(container), m_index(index) {}

    reference_wrapper<Cont> m_container;
    int m_index;
};

template <class Cont>
class ConstRandomAccessIterator
{
public:
    using value_type = typename Cont::value_type;

    ConstRandomAccessIterator(const RandomAccessIterator<Cont>& other) : m_container(other.m_container), m_index(other.m_index) {}
    ConstRandomAccessIterator(const RandomAccessReversedIterator<Cont>& other) : m_container(other.m_container), m_index(other.m_index) {}
    ConstRandomAccessIterator(const ConstRandomAccessReversedIterator<Cont>& other) : m_container(other.m_container), m_index(other.m_index) {}

    constexpr bool is_valid(void) { return m_index >= 0 && m_index < m_container.get().size(); }
    constexpr bool is_end(void) { return m_index == m_container.get().size(); }
    constexpr bool is_rend(void) { return m_index == -1; }
    constexpr const value_type* data(void) { return &m_container.get().data()[m_index]; }

    constexpr ConstRandomAccessIterator& operator=(const ConstRandomAccessIterator& other) = default;

    constexpr ConstRandomAccessIterator& operator++(void) { m_index++; return *this; }
    constexpr ConstRandomAccessIterator operator++(int) { m_index++; return ConstRandomAccessIterator(m_container, m_index - 1); }

    constexpr ConstRandomAccessIterator& operator--(void) { m_index--; return *this; }
    constexpr ConstRandomAccessIterator operator--(int) { m_index--; return ConstRandomAccessIterator(m_container, m_index + 1); }

    constexpr ConstRandomAccessIterator operator+(int offset) { return ConstRandomAccessIterator(m_container, m_index + offset); }
    constexpr ConstRandomAccessIterator operator-(int offset) { return ConstRandomAccessIterator(m_container, m_index - offset); }

    constexpr const value_type& operator*(void) { return m_container[m_index]; }
    constexpr const value_type* operator->(void) { return &m_container[m_index]; }

    constexpr bool operator==(const ConstRandomAccessIterator& other) { return m_index == other.m_index; }
    constexpr bool operator!=(const ConstRandomAccessIterator& other) { return m_index != other.m_index; }
    constexpr int operator<=>(const ConstRandomAccessIterator& other) { return m_index > other.m_index ? 1 : m_index < other.m_index ? -1 : 0; }

private:
    friend Cont;
    //Andreas Kling is a genius
    ConstRandomAccessIterator(const Cont& container, int index) : m_container(container), m_index(index) {}
    ConstRandomAccessIterator(const reference_wrapper<Cont>& container, int index) : m_container(container), m_index(index) {}

    const reference_wrapper<Cont> m_container;
    int m_index;
};

template <class Cont>
class RandomAccessReversedIterator
{
public:
    using value_type = typename Cont::value_type;
    RandomAccessReversedIterator(const RandomAccessIterator<Cont>& other) : m_container(other.m_container), m_index(other.m_index) {}

    constexpr bool is_valid(void) { return m_index >= 0 && m_index < m_container.get().size(); }
    constexpr bool is_end(void) { return m_index == m_container.get().size(); }
    constexpr bool is_rend(void) { return m_index == -1; }
    constexpr value_type* data(void) { return &m_container.get().data()[m_index]; }

    constexpr RandomAccessReversedIterator& operator=(const RandomAccessReversedIterator& other) = default;

    constexpr RandomAccessReversedIterator& operator++(void) { m_index--; return *this; }
    constexpr RandomAccessReversedIterator operator++(int) { m_index--; return RandomAccessReversedIterator(m_container, m_index + 1); }

    constexpr RandomAccessReversedIterator& operator--(void) { m_index++; return *this; }
    constexpr RandomAccessReversedIterator operator--(int) { m_index++; return RandomAccessReversedIterator(m_container, m_index - 1); }

    constexpr RandomAccessReversedIterator operator+(int offset) { return RandomAccessReversedIterator(m_container, m_index - offset); }
    constexpr RandomAccessReversedIterator operator-(int offset) { return RandomAccessReversedIterator(m_container, m_index + offset); }

    constexpr value_type& operator*(void) { return m_container.get().data()[m_index]; }
    constexpr value_type* operator->(void) { return &m_container.get().data()[m_index]; }

    constexpr bool operator==(const RandomAccessReversedIterator& other) { return m_index == other.m_index; }
    constexpr bool operator!=(const RandomAccessReversedIterator& other) { return m_index != other.m_index; }
    constexpr int operator<=>(const RandomAccessReversedIterator& other) { return m_index > other.m_index ? -1 : m_index < other.m_index ? 1 : 0; }

private:
    friend Cont;
    //Andreas Kling is a genius
    RandomAccessReversedIterator(Cont& container, int index) : m_container(container), m_index(index) {}
    RandomAccessReversedIterator(const reference_wrapper<Cont>& container, int index) : m_container(container), m_index(index) {}

    reference_wrapper<Cont> m_container;
    int m_index;
};

template <class Cont>
class ConstRandomAccessReversedIterator
{
public:
    using value_type = typename Cont::value_type;
    ConstRandomAccessReversedIterator(const RandomAccessIterator<Cont>& other) : m_container(other.m_container), m_index(other.m_index) {}
    ConstRandomAccessReversedIterator(const ConstRandomAccessIterator<Cont>& other) : m_container(other.m_container), m_index(other.m_index) {}
    ConstRandomAccessReversedIterator(const RandomAccessReversedIterator<Cont>& other) : m_container(other.m_container), m_index(other.m_index) {}

    constexpr bool is_valid(void) { return m_index >= 0 && m_index < m_container.get().size(); }
    constexpr bool is_end(void) { return m_index == m_container.get().size(); }
    constexpr bool is_rend(void) { return m_index == -1; }
    constexpr const value_type* data(void) { return &m_container.get().data()[m_index]; }

    constexpr ConstRandomAccessReversedIterator& operator=(const ConstRandomAccessReversedIterator& other) = default;

    constexpr ConstRandomAccessReversedIterator& operator++(void) { m_index--; return *this; }
    constexpr ConstRandomAccessReversedIterator operator++(int) { m_index--; return ConstRandomAccessReversedIterator(m_container, m_index + 1); }

    constexpr ConstRandomAccessReversedIterator& operator--(void) { m_index++; return *this; }
    constexpr ConstRandomAccessReversedIterator operator--(int) { m_index++; return ConstRandomAccessReversedIterator(m_container, m_index - 1); }

    constexpr ConstRandomAccessReversedIterator operator+(int offset) { return ConstRandomAccessReversedIterator(m_container, m_index - offset); }
    constexpr ConstRandomAccessReversedIterator operator-(int offset) { return ConstRandomAccessReversedIterator(m_container, m_index + offset); }

    constexpr const value_type& operator*(void) { return m_container.get().data()[m_index]; }
    constexpr const value_type* operator->(void) { return &m_container.get().data()[m_index]; }

    constexpr bool operator==(const ConstRandomAccessReversedIterator& other) { return m_index == other.m_index; }
    constexpr bool operator!=(const ConstRandomAccessReversedIterator& other) { return m_index != other.m_index; }
    constexpr int operator<=>(const ConstRandomAccessReversedIterator& other) { return m_index > other.m_index ? -1 : m_index < other.m_index ? 1 : 0; }

private:
    friend Cont;
    //Andreas Kling is a genius
    ConstRandomAccessReversedIterator(const Cont& container, int index) : m_container(container), m_index(index) {}
    ConstRandomAccessReversedIterator(const reference_wrapper<Cont>& container, int index) : m_container(container), m_index(index) {}

    const reference_wrapper<Cont> m_container;
    int m_index;
};



} //namspace
