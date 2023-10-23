

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



template <class K, class V>
class UnorderedMapC
{

    private:
    List<std::pair<K, V>> elements;
    size_t size;

    public:

    using this_type = UnorderedMapC<K, V>;

    using value_type = std::pair<K, V>;
    using iterable_type = ListElement<value_type>*;
    using backend_type = decltype(elements);

    using lreference = std::pair<K, V>&;
    using rreference = std::pair<K, V>&&;

    using const_lreference = const std::pair<K, V>&;
    using const_rreference = const std::pair<K, V>&&;

    using forward_iterator = ForwardUnorderedMapIterator<backend_type>;

    UnorderedMapC() = default;
    UnorderedMapC(const this_type& other) = default;
    UnorderedMapC(initializer_list<pair<K, V>> ListOfElements);
    ~UnorderedMapC();

    this_type& operator=(const this_type& other) = default;
    this_type& operator=(this_type&& other);

    V& operator[](K key);
    forward_iterator begin(void);
    forward_iterator end(void);
    this_type find(K key);
    bool erase(K key);
    bool exists(K key);
    void insert(K key, V value);
    void insert_or_assign(K key, V value);
    V pop_end(void);
    V pop_front(void);
    void push_front(K key, V value);
    void print(void);

};

template <class K, class V>
UnorderedMapC<K, V>::UnorderedMapC(std::initializer_list<pair<K, V>> items)
{
    for(auto a : items)
        this->elements.push_back(a);
}

template <class K, class V>
UnorderedMapC<K, V>::~UnorderedMapC() 
{
    // this->elements.clear();
}

template <class K, class V>
typename UnorderedMapC<K, V>::forward_iterator UnorderedMapC<K, V>::begin(void) {
    return this->elements.begin();
}

template <class K, class V>
typename UnorderedMapC<K, V>::forward_iterator UnorderedMapC<K, V>::end(void) {
    return this->elements.end();
}


}



