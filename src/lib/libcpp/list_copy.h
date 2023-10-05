
#pragma once

#include <lib/libc/stdlibx.h>
#include <lib/libcpp/iostream.h>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/type_traits.h>
#include <lib/libcpp/iterator.hpp>

namespace std
{

template<class Li>
class ForwardListIterator : public ForwardIterator<Li>
{

    public: 
    
        using Type = typename Li::value_type;

        ForwardListIterator(Type* ptr) {this->i_ptr = ptr;}

        ForwardListIterator<Li>& operator ++ () override
        {
            this->i_ptr = this->i_ptr->next;
            return *this;
        }

        ForwardListIterator<Li>&& operator ++ (int) override
        {
            ListIterator tmp = *this;
            this->i_ptr = this->i_ptr->next; 

            return tmp;
        }

        ForwardListIterator<Li>& operator -- () override 
        {
            this->i_ptr = this->i_ptr->previous;
            return *this;
        }

        ForwardListIterator<Li>&& operator -- (int) override
        {
            ListIterator tmp = *this;
            this->i_ptr = this->i_ptr->previous; 

            return tmp;
        }

        bool operator == (const ForwardListIterator<Li>& other) override
        {
            return ((uint32_t)this->i_ptr == (uint32_t)other.i_ptr);
        }

        bool operator != (const ForwardListIterator<Li>& other) override
        {
            return ((uint32_t)this->i_ptr != (uint32_t)other.i_ptr);
        }

        Type& operator * () override
        {
            return this->i_ptr->value;
        }
};


template<class Li>
class ReversedListIterator : ReversedIterator<Li>
{

    public: 
    
        using Type = typename Li::value_type;

        ReversedListIterator(Type* ptr) {this->i_ptr = ptr;}

        ReversedListIterator<Li>& operator ++ () override 
        {
            this->i_ptr = this->i_ptr->next;
            return *this;
        }

        ReversedListIterator<Li> operator ++ (int) override
        {
            ReversedListIterator tmp = *this;
            this->i_ptr = this->i_ptr->next; 

            return tmp;
        }

        ReversedListIterator<Li>& operator -- () override
        {
            this->i_ptr = this->i_ptr->previous;
            return *this;
        }

        ReversedListIterator<Li>&& operator -- (int)
        {
            ReversedListIterator tmp = *this;
            this->i_ptr = this->i_ptr->previous; 

            return tmp;
        }

        bool operator == (const ReversedListIterator<Li>& other)
        {
            return this->i_ptr == other.i_ptr;
        }

        bool operator != (const ReversedListIterator<Li>& other)
        {
            return this->i_ptr != other.i_ptr;
        }

        Type& operator * ()
        {
            return this->i_ptr->value;
        }
};

template<typename T>
struct ListElement
{
    T value;
    ListElement* next;
    ListElement* previous;
};

template<typename T>
class ListC
{

    private:

    using ListNode = ListElement<T>;

    ListNode  Head;
    uint32_t li_size;
    ListNode* goto_last_element(void);

    public:
    using value_type = ListElement<T>*;
    using this_type = ListC<T>;

    using lreference = T&;
    using rreference = T&&;

    using const_lreference = const T&;
    using const_rreference = const T&&;

    using forward_iterator = ForwardListIterator<this_type>;
    using reversed_iterator = ReversedListIterator<this_type>;

    ListC();
    ListC(const ListC& other) = default;
    // List(List&& other);
    ListC(std::initializer_list<T> items);
    
    ForwardListIterator<ListC<T>> begin();
    ForwardListIterator<ListC<T>> end();

    ReversedListIterator<ListC<T>> rbegin();
    ReversedListIterator<ListC<T>> rend();

    uint32_t size(void);

    void push_back(T value);
    void push_front(T value);

    void pop_back();
    void pop_front();

    ListNode* front(T index); // can return NULL
    ListNode* back(T index); // can return NULL

    void print(void);

    // void print(void);

};

template<typename T>
ListC<T>::ListC()
{
    this->li_size = 0;
    this->Head = (ListNode*)calloc(SIZE_OF(ListNode));
    this->Head->next = NULL;
    this->Head->previous = NULL;
}

template<typename T>
ListC<T>::ListC(std::initializer_list<T> items)
{
    this->size = 0;
    this->Head = (ListNode*)malloc(SIZE_OF(ListNode));
    this->Head->next = NULL;
    this->Head->previous = NULL;

    for(auto a : items)
        this->push_back(a);
}

template<typename T>
typename ListC<T>::ListNode* ListC<T>::goto_last_element(void)
{
    ListNode* tmp = this->Head;

    while(tmp->next != NULL)
        tmp = tmp->next;

    return tmp;
}

template<typename T>
ForwardListIterator<ListC<T>> ListC<T>::begin()
{
    return ForwardListIterator<ListC<T>>(this->Head);
}

template<typename T>
ForwardListIterator<ListC<T>> ListC<T>::end()
{
    return ForwardListIterator<ListC<T>>(nullptr);
}


template<typename T>
uint32_t ListC<T>::size(void)
{
    return this->li_size;
}


template<typename T>
void ListC<T>::push_back(T value)
{

    ListNode* Tail = this->goto_last_element();

    this->Tail->next = (ListNode*)calloc(SIZE_OF(ListNode));
    auto ElementCreated = this->Tail->next;

    ElementCreated->value = value;
    ElementCreated->previous = Tail;
    ElementCreated->next = NULL;

    this->li_size++;
    this->Tail = this->Tail->next;
}

template<typename T>
void ListC<T>::push_front(T value)
{

    auto NewHead = (ListElement<T>*)calloc(SIZE_OF(ListElement<T>));

    //Newthis->Head already set to NULL
    NewHead->next = this->Head;
    NewHead->value = value;

    this->Head = NewHead;
    this->li_size++;
}


template<typename T>
void ListC<T>::pop_back(void)
{
    ListElement<T>* Tmp = goto_last_element();

    T ret = Tmp->value;
    Tmp->previous->next = NULL;

    free(Tmp);
}

template<typename T>
void ListC<T>::print(void)
{

    if(!this->li_size)
    {
        std::cout << "[]";
        return;
    }

    ListElement<T>* tmp = this->Head;
    std::cout << '[';
    while(tmp->next != NULL)
    {
        std::cout << tmp->value << ',';
        tmp = tmp->next;
    }
    std::cout << tmp->value << ']';
    std::cout << std::endl;
}

}
