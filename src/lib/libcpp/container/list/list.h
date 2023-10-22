
#pragma once

#include <lib/libc/stdlibx.h>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/type_traits.h>
#include <lib/libcpp/function.hpp>
#include <sys/macros.h>
#include "./iterator.hpp"

namespace std
{


template<typename T>
struct ListElement
{
    using value_type = T;
    T value;
    ListElement* next;
    ListElement* previous;
};

template<typename T>
class List
{

    private:
    ListElement<T>*  Head;
    uint32_t li_size;
    ListElement<T>* goto_last_element(void);

    public:
    using ListNode = ListElement<T>;

    using value_type = T;
    using iterable_type = ListElement<T>*;

    using this_type = List<T>;

    //BRUH
    using lreference = T&;
    using rreference = T&&;

    using const_lreference = const T&;
    using const_rreference = const T&&;
    //BRUH

    using forward_iterator = ForwardListIterator<this_type>;
    using reversed_iterator = ReversedListIterator<this_type>;

    using const_forward_iterator = ConstForwardListIterator<this_type>;
    using const_reversed_iterator = ConstReversedListIterator<this_type>;

    List();
    List(const List& other) = default;
    // List(List&& other);
    List(std::initializer_list<T> items);
    
    forward_iterator begin();
    forward_iterator end();
    reversed_iterator rbegin();
    reversed_iterator rend();

    const_forward_iterator cbegin();
    const_forward_iterator cend();
    const_reversed_iterator crbegin();
    const_reversed_iterator crend();

    uint32_t size(void);

    void push_back(T value);
    void push_front(T value);

    void pop_back();
    void pop_front();

    ListNode* front(T index); // can return NULL
    ListNode* back(T index); // can return NULL

    void print(void);

    template<class Li>
    friend class ForwardListIterator;

    template<class Li>
    friend class ReversedListIterator;

    template<class Li>
    friend class ConstForwardListIterator;

    template<class Li>
    friend class ConstReversedListIterator;

};

template<typename T>
List<T>::List()
{
    this->li_size = 0;
    this->Head = (ListNode*)calloc(SIZE_OF(ListNode));
}

template<typename T>
List<T>::List(std::initializer_list<T> items)
{
    this->li_size = 0;
    this->Head = (ListNode*)calloc(SIZE_OF(ListNode));

    for(auto a : items)
        this->push_back(a);
}

template<typename T>
typename List<T>::ListNode* List<T>::goto_last_element(void)
{
    ListNode* tmp = this->Head;

    while(tmp->next != NULL)
        tmp = tmp->next;

    return tmp;
}

template<typename T>
typename List<T>::forward_iterator List<T>::begin()
{
    return List<T>::forward_iterator(this->Head);
}

template<typename T>
typename List<T>::forward_iterator List<T>::end()
{
    return List<T>::forward_iterator(NULL);
}


template<typename T>
typename List<T>::reversed_iterator List<T>::rbegin()
{
    return List<T>::reversed_iterator(this->goto_last_element());
}

template<typename T>
typename List<T>::reversed_iterator List<T>::rend()
{
    return List<T>::reversed_iterator(NULL);
}

template<typename T>
typename List<T>::const_forward_iterator List<T>::cbegin()
{
    return List<T>::const_forward_iterator(this->Head);
}

template<typename T>
typename List<T>::const_forward_iterator List<T>::cend()
{
    return List<T>::const_forward_iterator(NULL);
}


template<typename T>
typename List<T>::const_reversed_iterator List<T>::crbegin()
{
    return List<T>::const_reversed_iterator(this->goto_last_element());
}

template<typename T>
typename List<T>::const_reversed_iterator List<T>::crend()
{
    return List<T>::const_reversed_iterator(NULL);
}

template<typename T>
uint32_t List<T>::size(void)
{
    return this->li_size;
}


template<typename T>
void List<T>::push_back(T value)
{

    ListNode* Tail;
    ListNode* ElementCreated;;

    this->li_size ? [&](){
        Tail = NULL;
        Tail = this->goto_last_element();
        Tail->next = (ListNode*)calloc(SIZE_OF(ListNode));

        ElementCreated = Tail->next;
        //ElementCreated->next already NULL
        ElementCreated->previous = Tail;

    }() : [&](){
        ElementCreated = this->Head;
    }();

    ElementCreated->value = value;

    this->li_size++;
}

template<typename T>
void List<T>::push_front(T value)
{

    ListNode* NewHead;

    this->li_size ? [&](){

        NewHead = (ListNode*)calloc(SIZE_OF(ListNode));
        //NewHead->previous already set to NULL
        NewHead->next = this->Head;
        this->Head->previous = NewHead; // set previous pointer to node
    }() : [&](){
        NewHead = this->Head;
    }();

    NewHead->value = value;

    this->Head = NewHead;
    this->li_size++;
}


template<typename T>
void List<T>::pop_back(void)
{
    ListNode* tmp = goto_last_element();
    tmp->previous->next = NULL;

    this->li_size--;
    free(tmp); //realase resources from destroyed node
}


template<typename T>
void List<T>::pop_front(void)
{
    ListNode* tmp = this->Head;
    tmp->next->previous = NULL;

    this->Head = this->Head->next;

    this->li_size--;
    free(tmp); //realase resources from destroyed node
}

template<typename T>
void List<T>::print(void)
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
}

}
