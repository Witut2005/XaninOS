
#pragma once

#include <lib/libc/stdlibx.h>
#include <lib/libcpp/algorithm.h>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/ostream.h>

#include "./iterator.hpp"

/*DONT USE STD::STRING DOESNT WORK*/

namespace std
{


template <class K, class V>
class UnorderedMap
{


    public:
    struct ListElement
    {
        pair<K, V> item;
        ListElement *next;
        ListElement *previous;
    };

    private:
    ListElement *Head;
    ListElement *Tail;
    size_t size;

    public:
    UnorderedMap();
    UnorderedMap(const UnorderedMap<K, V> &other) = default;
    UnorderedMap(initializer_list<pair<K,V>> ListOfElements);
    ListElement* operator++(int);
    ListElement* operator++(void);
    V& operator[](K key);
    UnorderedMapIterator<K, V, UnorderedMap> begin(void);
    UnorderedMapIterator<K, V, UnorderedMap> end(void);
    ListElement* goto_last_element();
    UnorderedMapIterator<K, V, UnorderedMap> find(K key);
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
UnorderedMap<K, V>::UnorderedMap()
{
    this->Head = (ListElement *)malloc(SIZE_OF(ListElement));
    this->Tail = (ListElement *)malloc(SIZE_OF(ListElement));
    this->Head->next = nullptr;
    this->Head->previous = nullptr;
    this->size = 0;
    this->Head->next = this->Tail;
}

template <class K, class V>
UnorderedMap<K, V>::UnorderedMap(initializer_list<pair<K,V>> ListOfElements)
{
    this->Head = (ListElement *)malloc(SIZE_OF(ListElement));

    this->Head->item.first = (*ListOfElements.begin()).first;
    this->Head->item.second = (*ListOfElements.begin()).second;

    ListElement* Tmp = this->Head;

    for(auto a : ListOfElements)
    {
        Tmp->next = (ListElement *)malloc(SIZE_OF(ListElement));
        Tmp = Tmp->next;
        Tmp->item.first = a.first;
        Tmp->item.second = a.second;
    }
    Tmp->next = nullptr;
    this->Tail = Tmp;

    this->Head->next->previous = this->Head;
    this->Head = this->Head->next;
    free(this->Head->previous);
    this->Head->previous = nullptr;
}

template<typename K, typename V>
typename UnorderedMap<K, V>::ListElement* UnorderedMap<K, V>::goto_last_element()
{
    ListElement *Tmp = Head;
    while (Tmp->next != this->Tail)
    {
        Tmp = Tmp->next;
    }
    return Tmp;
}

template<typename K, typename V>
UnorderedMapIterator<K, V, UnorderedMap> UnorderedMap<K, V>::find(K key)
{
    ListElement *Tmp = Head;
    while (Tmp != Tail)
    {
        if (Tmp->item.first == key)
            return UnorderedMapIterator<K, V, UnorderedMap>(Tmp);
        Tmp = Tmp->next;
    }
    return UnorderedMapIterator<K, V, UnorderedMap>(Tail);
}

template<typename K, typename V>
bool UnorderedMap<K, V>::erase(K key)
{
    auto IteratorToErase = find(key);

    if(IteratorToErase == Tail)
        return false;
    
    auto Previous = IteratorToErase->previous();
    auto Next = IteratorToErase->next();

    Previous.i_ptr->next = Previous.i_ptr;

    return true;
}

template<typename K, typename V>
bool UnorderedMap<K, V>::exists(K key)
{
    if(this->find(key) == this->Tail)
        return false;

    return true;
}

template<typename K, typename V>
void UnorderedMap<K, V>::insert(K key, V value)
{

    if(this->exists(key))
        return;

    if (!size)
    {
        this->Head->item.first = key;
        this->Head->item.second = value;
        this->size++;
        return;
    }

    ListElement *ItemInserted = goto_last_element();

    ItemInserted->next = (ListElement *)(malloc(SIZE_OF(ListElement)));

    ItemInserted->next->previous = ItemInserted;
    ItemInserted = ItemInserted->next;
    ItemInserted->item.first = key;
    ItemInserted->item.second = value;
    ItemInserted->next = this->Tail;

    this->size++;
}

template<typename K, typename V>
void UnorderedMap<K, V>::insert_or_assign(K key, V value)
{
    
    if(this->exists(key))
    {
        auto Iterator = this->find(key);
        *Iterator = value;
        return;
    }

    if (!size)
    {
        this->Head->item.first = key;
        this->Head->item.second = value;
        this->size++;
        return;
    }

    ListElement *ItemInserted = goto_last_element();

    ItemInserted->next = (ListElement *)(malloc(SIZE_OF(ListElement)));

    ItemInserted->next->previous = ItemInserted;
    ItemInserted = ItemInserted->next;
    ItemInserted->item.first = key;
    ItemInserted->item.second = value;
    ItemInserted->next = this->Tail;

    this->size++;

}

template<typename K, typename V>
typename UnorderedMap<K, V>::ListElement* UnorderedMap<K, V>::operator++(int)
{
    ListElement *Tmp = this->Head;
    this->Head = this->Head->next;

    return Tmp;
}

template<typename K, typename V>
typename UnorderedMap<K, V>::ListElement* UnorderedMap<K, V>::operator++(void)
{
    Head = Head->next;
    return Head;
}

template<typename K, typename V>
V UnorderedMap<K, V>::pop_end(void)
{
    ListElement *LastItem = goto_last_element();
    auto tmp = LastItem->value;
    free(LastItem);
    return tmp;
}

template<typename K, typename V>
V UnorderedMap<K, V>::pop_front(void)
{
    V ret = this->Head->item.second;
    auto tmp = this->Head;
    this->Head = this->Head->next;
    free(tmp);

    return ret;
}

template<typename K, typename V>
void UnorderedMap<K, V>::push_front(K key, V value)
{
    if (!size)
    {
        this->Head->item.first = key;
        this->Head->item.second = value;
        this->size++;
        return;
    }

    this->size++;
    ListElement *NewItem = (ListElement *)malloc(SIZE_OF(ListElement));
    NewItem->next = this->Head;
    NewItem->previous = nullptr;
    NewItem->item.first = key;
    NewItem->item.second = value;
    Head = NewItem;
}

template<typename K, typename V>
void UnorderedMap<K, V>::print(void)
{
    ListElement *HeadTmp = this->Head;

    std::cout << '[';

    while (HeadTmp->next != nullptr)
    {
        std::cout << '[' << HeadTmp->item.first << ',' << HeadTmp->item.second << "], ";
        HeadTmp = HeadTmp->next;
    }

    std::cout << '[' << HeadTmp->item.first << ',' << HeadTmp->item.second << "]]" << std::endl;
}

template<typename K, typename V>
V& UnorderedMap<K, V>::operator[](K key)
{
    auto ret = this->find(key);

    if (ret == this->Tail)
    {
        this->insert(key, *this->end());
        return *this->find(key);
    }

    else
        return *ret;
}        

template<typename K, typename V>
UnorderedMapIterator<K, V, UnorderedMap> UnorderedMap<K, V>::begin(void) {
    return UnorderedMapIterator<K, V, UnorderedMap>(this->Head);
}

template<typename K, typename V>
UnorderedMapIterator<K, V, UnorderedMap> UnorderedMap<K, V>::end(void) {
    return UnorderedMapIterator<K, V, UnorderedMap>(this->Tail);
}

}



