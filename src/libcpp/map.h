
#pragma once

#include <libcpp/algorithm.h>

namespace std
{

template<class K, class V>
class UnorderedMap
{

    public:
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
        this->size = 0;
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

        if(!size)
        {
            this->Head->item.first = key;
            this->Head->item.second = value;
            this->size++;
            return;
        }

        ListElement* LastItem = goto_last_element();

        LastItem->next = (ListElement*)(malloc(sizeof(ListElement)));

        LastItem = LastItem->next;
        LastItem->item.first = key;
        LastItem->item.second = value;

        LastItem->next = nullptr;
        this->size++;

        
    }

    V pop_end(void)
    {
        ListElement* LastItem = goto_last_element();
        auto tmp = LastItem->value;
        free(LastItem);
        return tmp;
    }

    V pop_front(void)
    {
        V ret = this->Head->item.second;
        auto tmp = this->Head;
        this->Head = this->Head->next;
        free(tmp);

        return ret;
    }

    void push_front(K key, V value)
    {
        if(!size)
        {
            this->Head->item.first = key;
            this->Head->item.second = value;
            this->size++;
            return;
        }

        this->size++;
        ListElement* NewItem = (ListElement*)malloc(sizeof(ListElement));
        NewItem->next = this->Head;
        NewItem->previous = nullptr;
        NewItem->item.first = key;
        NewItem->item.second = value;
        Head = NewItem;
    }

    void print(void)
    {
        ListElement* HeadTmp = this->Head;

        std::cout << '[';

        while(HeadTmp->next != nullptr)
        {
            std::cout << '[' << HeadTmp->item.first << ',' << HeadTmp->item.second << "], ";
            HeadTmp = HeadTmp->next;
        }

        std::cout << '[' << HeadTmp->item.first << ',' << HeadTmp->item.second << "]]" << std::endl;

    }

    V operator[](K key)
    {
        return this->find(key);
    }



};

}
