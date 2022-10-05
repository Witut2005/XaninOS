
#pragma once

#include <libcpp/algorithm.h>

namespace std
{

template<class K, class V>
class UnorderedMap
{

    struct ListElement
    {
        std::pair<K, V> item;
        ListElement* next;
        ListElement* previous;
    };

    ListElement* Head;
    ListElement* Tail;
    size_t size;

    public:

    UnorderedMap()
    {
        Head = (ListElement*)malloc(sizeof(ListElement));
        Head->next = nullptr;
        Head->previous = nullptr;
        Tail = Head;
    }

    ListElement* goto_last_element()
    {
        ListElement* Tmp = Head;
        while(Tmp->next != nullptr)
        {
            Tmp = Tmp->next;
        }
        return Tmp;
    }

    ListElement* find(K key)
    {
        ListElement* Tmp = Head;
        while(Tmp != nullptr)
        {
            if(Tmp->item.first == key)
                return Tmp;
            Tmp = Tmp->next;
            
        }
        return nullptr;
    }

    void insert(K key, V value)
    {

        ListElement* LastItem = goto_last_element();
        LastItem->next = (ListElement*)(malloc(sizeof(ListElement)));

        LastItem = LastItem->next;
        LastItem->item.first = key;
        LastItem->item.second = value;

        LastItem->next = nullptr;

        
    }

    V operator[](K key)
    {
        return this->find(key);
    }



};

}