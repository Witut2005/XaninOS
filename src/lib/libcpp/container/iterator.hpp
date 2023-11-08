
#pragma once

namespace std
{

    template <class Cont>
    class Iterator
    {
    protected:
        using iterable_type = typename Cont::iterable_type;
        iterable_type i_ptr;

    public:
        Cont &container;
        iterable_type pointer()
        {
            return this->i_ptr;
        }

        Iterator<Cont>(iterable_type i_ptr, Cont &container) : i_ptr(i_ptr), container(container) {}

        template <typename InputIt>
        friend bool operator==(InputIt lhs, InputIt rhs);

        template <typename InputIt>
        friend bool operator!=(InputIt lhs, InputIt rhs);
    };

    template <typename InputIt>
    bool operator==(InputIt lhs, InputIt rhs)
    {
        return lhs.i_ptr == rhs.i_ptr;
    }

    template <typename InputIt>
    bool operator!=(InputIt lhs, InputIt rhs)
    {
        return lhs.i_ptr != rhs.i_ptr;
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

        ForwardIterator<Cont>(iterable_type i_ptr, Cont &container) : Iterator<Cont>(i_ptr, container),
                                                                      begin(container.begin_ptr()), end(container.end_ptr()) {}

        // virtual constexpr const char* type_info(void);

        virtual iterator_type &operator++(void) = 0;
        virtual iterator_type operator++(int) = 0;
        virtual iterator_type &operator--(void) = 0;
        virtual iterator_type operator--(int) = 0;

        virtual iterator_type operator+(int) = 0;
        virtual iterator_type operator-(int) = 0;

        virtual lreference operator*(void) const = 0;
        // virtual iterator_type operator-> (void) const = 0;

        virtual iterator_type &operator=(const iterator_type &other) = 0;
        virtual iterator_type &operator=(iterator_type &&) = 0;

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

        ReversedIterator<Cont>(iterable_type i_ptr, Cont &container) : Iterator<Cont>(i_ptr, container),
                                                                       rbegin(container.rbegin_ptr()), rend(container.rend_ptr()) {}
        virtual iterator_type &operator++(void) = 0;
        virtual iterator_type operator++(int) = 0;
        virtual iterator_type &operator--(void) = 0;
        virtual iterator_type operator--(int) = 0;

        virtual iterator_type operator+(int) = 0;
        virtual iterator_type operator-(int) = 0;

        virtual lreference operator*(void) const = 0;

        virtual iterator_type &operator=(const iterator_type &other) = 0;
        virtual iterator_type &operator=(iterator_type &&) = 0;

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
        Cont &container;

        ConstIterator<Cont>(iterable_type i_ptr, Cont &container) : i_ptr(i_ptr), container(container) {}

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

        ConstForwardIterator<Cont>(iterable_type i_ptr, Cont &container) : ConstIterator<Cont>(i_ptr, container),
                                                                           begin(container.begin_ptr()),
                                                                           end(container.end_ptr()) {}

        virtual iterator_type &operator++(void) = 0;
        virtual iterator_type operator++(int) = 0;
        virtual iterator_type &operator--(void) = 0;
        virtual iterator_type operator--(int) = 0;

        virtual iterator_type operator+(int) = 0;
        virtual iterator_type operator-(int) = 0;

        virtual const_lreference operator*(void) const = 0;

        virtual iterator_type &operator=(const iterator_type &other) = 0;
        virtual iterator_type &operator=(iterator_type &&) = 0;

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

        ConstReversedIterator<Cont>(iterable_type i_ptr, Cont &container) : ConstIterator<Cont>(i_ptr, container),
                                                                            rbegin(container.rbegin_ptr()),
                                                                            rend(container.rend_ptr()) {}
        virtual iterator_type &operator++(void) = 0;
        virtual iterator_type operator++(int) = 0;
        virtual iterator_type &operator--(void) = 0;
        virtual iterator_type operator--(int) = 0;

        virtual iterator_type operator+(int) = 0;
        virtual iterator_type operator-(int) = 0;

        virtual const_lreference operator*(void) const = 0;

        virtual iterator_type &operator=(const iterator_type &other) = 0;
        virtual iterator_type &operator=(iterator_type &&) = 0;

        virtual explicit operator bool(void) const = 0;
        virtual bool valid(void) const = 0;
    };

}
