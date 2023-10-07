
#pragma once

#include <lib/libc/stdlibx.h>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/type_traits.h>
#include <lib/libcpp/iterator.hpp>
#include <lib/libcpp/function.hpp>

namespace std
{

template<class Li>
class ForwardListIterator : public ForwardIterator<Li>
{

    public: 
    
        using Type = typename Li::value_type;

        ForwardListIterator(Type* ptr) {this->i_ptr = ptr;}
        ForwardListIterator(const ForwardIterator<Li>& other) {this->i_ptr = other.i_ptr;} 
        ForwardListIterator(const ForwardListIterator<Li>& other) {this->i_ptr = other.i_ptr;}

        ForwardIterator<Li>& operator ++ () override
        {
            this->i_ptr = this->i_ptr->next;
            return *this;
        }

        ForwardIterator<Li>&& operator ++ (int) override
        {
            ForwardListIterator<Li> tmp = *this;
            this->i_ptr = this->i_ptr->next; 

            return std::move(tmp);
        }

        ForwardIterator<Li>& operator -- () override 
        {
            this->i_ptr = this->i_ptr->previous;
            return *this;
        }

        ForwardIterator<Li>&& operator -- (int) override
        {
            ForwardListIterator tmp = *this;
            this->i_ptr = this->i_ptr->previous; 

            return std::move(tmp);
        }

        ForwardIterator<Li>&& operator + (int) override
        {
            ForwardListIterator<Li>tmp(nullptr);
            return std::move(tmp);
        }
        ForwardIterator<Li>&& operator - (int) override
        {
            ForwardListIterator<Li>tmp(nullptr);
            return std::move(tmp);
        }

        ForwardIterator<Li>& operator = (const ForwardIterator<Li>& other)  override 
        {
            *this = other;
            return *this;
        }

        ForwardIterator<Li>& operator = (ForwardIterator<Li>&& other)  override 
        {
            *this = other;
            other.i_ptr = NULL;
            return *this;
        }

        bool operator == (const ForwardIterator<Li>& other) override
        {
            return this->i_ptr == other.i_ptr;
        }

        bool operator != (const ForwardIterator<Li>& other) override
        {
            return this->i_ptr != other.i_ptr;
        }

        operator bool(void) override
        {
            return this->i_ptr != NULL;
        }

        Type& operator * () override
        {
            return *this->i_ptr;
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

    ListNode*  Head;
    uint32_t li_size;
    ListNode* goto_last_element(void);

    public:
    using value_type = ListElement<T>;
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
    this->li_size = 0;
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
    return this->Head;
}

template<typename T>
ForwardListIterator<ListC<T>> ListC<T>::end()
{
    return nullptr;
}


template<typename T>
uint32_t ListC<T>::size(void)
{
    return this->li_size;
}


template<typename T>
void ListC<T>::push_back(T value)
{

    ListNode* Tail;
    ListNode* ElementCreated;;

    this->li_size ? [&](){
        Tail = NULL;
        Tail = this->goto_last_element();
        Tail->next = (ListNode*)calloc(SIZE_OF(ListNode));

        ElementCreated = Tail->next;

    }() : [&](){
        Tail = NULL;
        ElementCreated = Tail = this->Head;
    }();

    ElementCreated->value = value;
    ElementCreated->previous = Tail;

    this->li_size++;
}

template<typename T>
void ListC<T>::push_front(T value)
{

    ListNode* NewHead;

    this->li_size ? [&](){

        NewHead = (ListNode*)calloc(SIZE_OF(ListNode));
        //NewHead->previous already set to NULL
        NewHead->next = this->Head;

    }() : [&](){
        NewHead = Head;
    }();

    NewHead->value = value;

    this->Head = NewHead;
    this->li_size++;
}


template<typename T>
void ListC<T>::pop_back(void)
{
    ListNode* tmp = goto_last_element();
    tmp->previous->next = NULL;

    this->li_size--;
    free(tmp); //realase resources from destroyed node
}


template<typename T>
void ListC<T>::pop_front(void)
{
    ListNode* tmp = this->Head;
    tmp->next->previous = NULL;

    this->Head = this->Head->next;

    this->li_size--;
    free(tmp); //realase resources from destroyed node
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
}

}
