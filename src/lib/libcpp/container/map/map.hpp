#pragma once

#include <lib/libc/stdlibx.h>
#include <lib/libcpp/algorithm.h>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/ostream.h>
#include "../list.hpp"

#include "./iterator.hpp"

/*DONT USE STD::STRING DOESNT WORK*/

namespace std
{

    template <class T>
    using ForwardUnorderedMapIterator = ForwardListIterator<T>;

    template <class T>
    using ConstForwardUnorderedMapIterator = ConstForwardListIterator<T>;

    template <class T>
    using ReversedUnorderedMapIterator = ReversedListIterator<T>;

    template <class T>
    using ConstReversedUnorderedMapIterator = ConstReversedListIterator<T>;

    template <class K, class V>
    class UnorderedMap
    {

    private:
        List<std::pair<K, V>> elements;
        size_t size;

    public:
        using this_type = UnorderedMap<K, V>;

        using value_type = std::pair<K, V>;
        using iterable_type = ListElement<value_type> *;
        using backend_type = decltype(elements);

        using lreference = std::pair<K, V> &;
        using rreference = std::pair<K, V> &&;

        using const_lreference = const std::pair<K, V> &;
        using const_rreference = const std::pair<K, V> &&;

        using forward_iterator = ForwardUnorderedMapIterator<backend_type>;
        using const_forward_iterator = ConstForwardUnorderedMapIterator<backend_type>;

        using reversed_iterator = ReversedUnorderedMapIterator<backend_type>;
        using const_reversed_iterator = ConstReversedUnorderedMapIterator<backend_type>;

        UnorderedMap() = default;
        UnorderedMap(const this_type &other) = default;
        UnorderedMap(initializer_list<pair<K, V>> ListOfElements);
        ~UnorderedMap();

        this_type &operator=(const this_type &other) = default;
        this_type &operator=(this_type &&other);

        V &operator[](K key);

        inline iterable_type begin_ptr()
        {
            return this->elements.Head;
        }
        inline iterable_type end_ptr()
        {
            return this->elements.ListUpperBoundary;
        }
        inline iterable_type rbegin_ptr()
        {
            return this->elements.goto_last_element();
        }
        inline iterable_type rend_ptr()
        {
            return this->elements.ListLowerBoundary;
        }

        inline const iterable_type cbegin_ptr()
        {
            return this->elements.Head;
        }
        inline const iterable_type cend_ptr()
        {
            return this->elements.ListUpperBoundary;
        }
        inline const iterable_type crbegin_ptr()
        {
            return this->elements.goto_last_element();
        }
        inline const iterable_type crend_ptr()
        {
            return this->elements.ListLowerBoundary;
        }

        forward_iterator begin(void);
        forward_iterator end(void);
        const_forward_iterator cbegin(void);
        const_forward_iterator cend(void);

        reversed_iterator rbegin(void);
        reversed_iterator rend(void);
        const_reversed_iterator crbegin(void);
        const_reversed_iterator crend(void);

        template <class OutIt = ForwardListIterator<decltype(elements)>>
        OutIt find(K key)
        {
            static_assert(OutIt::type == Types::ForwardListIterator, "Use UnorderedMap iterators");

            auto Tail = this->elements.goto_last_element();

            for (auto it = this->elements.Head; it != Tail->next; it = it->next)
            {
                if (it->value.first == key)
                {
                    return OutIt(it, this->elements);
                }
            }

            return OutIt(NULL, this->elements);
        }

        void remove(K key);

        template <typename InputIt>
        void erase(InputIt it)
        {
            this->elements.erase(it);
        }

        template <typename InputIt>
        void erase(InputIt beg, InputIt end)
        {
            this->elements.erase(beg, end);
        }

        bool exists(K key);
        void insert(K key, V value);
        void insert_or_assign(K key, V value);

        void pop_back(void);
        void pop_front(void);

        // void push_front(K key, V value);
        void print(void);
    };

    template <class K, class V>
    UnorderedMap<K, V>::UnorderedMap(std::initializer_list<pair<K, V>> items)
    {
        for (auto a : items)
            this->elements.push_back(a);
    }

    template <class K, class V>
    UnorderedMap<K, V>::~UnorderedMap()
    {
        this->elements.clear();
    }

    template <class K, class V>
    typename UnorderedMap<K, V>::forward_iterator UnorderedMap<K, V>::begin(void)
    {
        return this->elements.begin();
    }

    template <class K, class V>
    typename UnorderedMap<K, V>::forward_iterator UnorderedMap<K, V>::end(void)
    {
        return this->elements.end();
    }

    template <class K, class V>
    typename UnorderedMap<K, V>::const_forward_iterator UnorderedMap<K, V>::cbegin(void)
    {
        return this->elements.cbegin();
    }

    template <class K, class V>
    typename UnorderedMap<K, V>::const_forward_iterator UnorderedMap<K, V>::cend(void)
    {
        return this->elements.cend();
    }

    template <class K, class V>
    typename UnorderedMap<K, V>::reversed_iterator UnorderedMap<K, V>::rbegin(void)
    {
        return this->elements.rbegin();
    }

    template <class K, class V>
    typename UnorderedMap<K, V>::reversed_iterator UnorderedMap<K, V>::rend(void)
    {
        return this->elements.rend();
    }

    template <class K, class V>
    typename UnorderedMap<K, V>::const_reversed_iterator UnorderedMap<K, V>::crbegin(void)
    {
        return this->elements.crbegin();
    }

    template <class K, class V>
    typename UnorderedMap<K, V>::const_reversed_iterator UnorderedMap<K, V>::crend(void)
    {
        return this->elements.crend();
    }

    template <class K, class V>
    V &UnorderedMap<K, V>::operator[](K key)
    {
        if (this->exists(key))
        {
            return (*this->find<UnorderedMap<K, V>::forward_iterator>(key)).second;
        }
        else
        {
            this->elements.push_back(std::pair<K, V>(key, this->elements.Head->value.second));
            return this->elements.goto_last_element()->value.second;
        }
    }

    template <class K, class V>
    void UnorderedMap<K, V>::remove(K key)
    {
        for (auto tmp = this->elements.Head; tmp != NULL; tmp = tmp->next)
        {
            if (tmp->value.first == key)
                this->elements.erase(ForwardListIterator<decltype(this->elements)>(tmp), ForwardListIterator<decltype(this->elements)>(tmp->next));
        }
    }

    template <class K, class V>
    bool UnorderedMap<K, V>::exists(K key)
    {
        for (auto it = this->elements.begin(); it != this->elements.end(); it++)
        {
            if ((*it).first == key)
                return true;
        }
        return false;
    }

    template <class K, class V>
    void UnorderedMap<K, V>::insert(K key, V value)
    {
        if (!this->exists(key))
            this->elements.push_back(std::pair(key, value));
    }

    template <class K, class V>
    void UnorderedMap<K, V>::insert_or_assign(K key, V value)
    {
        if (!this->exists(key))
            this->elements.push_back(std::pair(key, value));
        else
        {
            *this->find(key) = std::pair(key, value);
        }
    }

    template <class K, class V>
    void UnorderedMap<K, V>::pop_back(void)
    {
        this->elements.pop_back();
    }

    template <class K, class V>
    void UnorderedMap<K, V>::pop_front(void)
    {
        this->elements.pop_front();
    }

}