
#pragma once

#include <libcpp/algorithm.h>
#include <libcpp/utility.h>

namespace std
{

    template <class K, class V>
    class UnorderedMap
    {

    public:
        struct ListElement
        {
            std::pair<K, V> item;
            ListElement *next;
            ListElement *previous;
        };

        ListElement *Head;
        ListElement *Tail;
        size_t size;

    public:
        UnorderedMap(std::initializer_list<std::pair<K,V>> ListOfElements)
        {
            this->Head = (ListElement *)malloc(sizeof(ListElement));

            this->Head->item.first = (*ListOfElements.begin()).first;
            this->Head->item.second = (*ListOfElements.begin()).second;

            ListElement* Tmp = this->Head;

            for(auto a : ListOfElements)
            {
                Tmp->next = (ListElement *)malloc(sizeof(ListElement));
                Tmp = Tmp->next;
                Tmp->item.first = a.first;
                Tmp->item.second = a.second;
            }
            Tmp->next = nullptr;
            this->Tail = Tmp;
            this->Head = this->Head->next;
        }

        UnorderedMap()
        {
            this->Head = (ListElement *)malloc(sizeof(ListElement));
            this->Tail = (ListElement *)malloc(sizeof(ListElement));
            this->Head->next = nullptr;
            this->Head->previous = nullptr;
            this->size = 0;
            this->Head->next = this->Tail;
        }

        ListElement *goto_last_element()
        {
            ListElement *Tmp = Head;
            while (Tmp->next != this->Tail)
            {
                Tmp = Tmp->next;
            }
            return Tmp;
        }

        ListElement *find(K key)
        {
            ListElement *Tmp = Head;
            while (Tmp != Tail)
            {
                if (Tmp->item.first == key)
                    return Tmp;
                Tmp = Tmp->next;
            }
            return Tail;
        }

        void insert(K key, V value)
        {

            if (!size)
            {
                this->Head->item.first = key;
                this->Head->item.second = value;
                this->size++;
                return;
            }

            ListElement *LastItem = goto_last_element();

            LastItem->next = (ListElement *)(malloc(sizeof(ListElement)));

            LastItem = LastItem->next;
            LastItem->item.first = key;
            LastItem->item.second = value;

            LastItem->next = this->Tail;
            this->size++;
        }

        V begin(void)
        {
            return this->Head;
        }

        V end(void)
        {
            return this->Tail;
        }

        ListElement *operator++(int)
        {
            ListElement *Tmp = this->Head;
            this->Head = this->Head->next;

            return Tmp;
        }

        ListElement *operator++(void)
        {
            Head = Head->next;
            return Head;
        }

        V pop_end(void)
        {
            ListElement *LastItem = goto_last_element();
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
            if (!size)
            {
                this->Head->item.first = key;
                this->Head->item.second = value;
                this->size++;
                return;
            }

            this->size++;
            ListElement *NewItem = (ListElement *)malloc(sizeof(ListElement));
            NewItem->next = this->Head;
            NewItem->previous = nullptr;
            NewItem->item.first = key;
            NewItem->item.second = value;
            Head = NewItem;
        }

        void print(void)
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

        V &operator[](K key)
        {
            auto ret = this->find(key);
            if (ret == this->Tail)
                return (V &)this->Tail;
            else
                return ret->item.second;
        }
    };

}
