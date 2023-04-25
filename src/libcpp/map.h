
#pragma once

#include <libc/stdlibx.h>
#include <libcpp/algorithm.h>
#include <libcpp/utility.h>
#include <libcpp/ostream.h>

/*DONT USE STD::STRING DOESNT WORK*/

namespace std
{

    template<typename K, typename V, template <typename, typename> class UnorderedMap>
    class UnorderedMapIterator 
    {
        using Type = typename UnorderedMap<K, V>::ListElement;
        
        public:

        UnorderedMapIterator(Type* ptr) : i_ptr(ptr) {}

        UnorderedMapIterator& operator ++ () 
        {
            this->i_ptr = this->i_ptr->next;
            return *this;
        }

        UnorderedMapIterator operator ++ (int) 
        {
            UnorderedMapIterator tmp = this->i_ptr;
            this->i_ptr = this->i_ptr->next;
            return tmp;
        }

        UnorderedMapIterator& operator -- () 
        {
            this->i_ptr = this->i_ptr->previous;
            return this->i_ptr;
        }

        UnorderedMapIterator operator -- (int) 
        {
            UnorderedMapIterator tmp = this->i_ptr;
            this->i_ptr = this->i_ptr->previous;
            return tmp;
        }

        V& operator * ()
        {
            return this->i_ptr->item.second;
        }

        bool operator == (const UnorderedMapIterator x)
        {
            return this->i_ptr == x.i_ptr; 
        }

        bool operator != (const UnorderedMapIterator x)
        {
            return this->i_ptr != x.i_ptr; 
        }

        private:
        Type* i_ptr;

    };


    template <class K, class V>
    class UnorderedMap
    {

        bool initialized;

    public:
        struct ListElement
        {
            pair<K, V> item;
            ListElement *next;
            ListElement *previous;
        };

        ListElement *Head;
        ListElement *Tail;
        size_t size;

    public:
        UnorderedMap(initializer_list<pair<K,V>> ListOfElements)
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

            this->Head->next->previous = this->Head;
            this->Head = this->Head->next;
            free(this->Head->previous);
            this->Head->previous = nullptr;
        }

        bool is_initialized()
        {
            return this->initialized;
        }

        UnorderedMap()
        {
            this->Head = (ListElement *)malloc(sizeof(ListElement));
            this->Tail = (ListElement *)malloc(sizeof(ListElement));
            this->Head->next = nullptr;
            this->Head->previous = nullptr;
            this->size = 0;
            this->Head->next = this->Tail;
            this->initialized = true;
        }

        void init()
        {

            // if(this->initialized)
            //     return;
            this->Head = (ListElement *)malloc(sizeof(ListElement));
            this->Tail = (ListElement *)malloc(sizeof(ListElement));
            this->Head->next = nullptr;
            this->Head->previous = nullptr;
            this->size = 0;
            this->Head->next = this->Tail;
            this->initialized = true;
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

        UnorderedMapIterator<K, V, UnorderedMap> find(K key)
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

        bool exists(K key)
        {
            if(this->find(key) == this->Tail)
                return false;

            return true;
        }

        void insert(K key, V value)
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

            ListElement *LastItem = goto_last_element();

            LastItem->next = (ListElement *)(malloc(sizeof(ListElement)));

            LastItem = LastItem->next;
            LastItem->item.first = key;
            LastItem->item.second = value;

            LastItem->next = this->Tail;
            this->size++;
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
                return (V&)this->Tail;
            else
                return *ret;
        }        
        
        UnorderedMapIterator<K, V, UnorderedMap> begin(void)
        {
            return UnorderedMapIterator<K, V, UnorderedMap>(this->Head);
        }

        UnorderedMapIterator<K, V, UnorderedMap> end(void)
        {
            return UnorderedMapIterator<K, V, UnorderedMap>(this->Tail);
        }

    };

}



