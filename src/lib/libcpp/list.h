
#pragma once

#include <lib/libc/stdlibx.h>
#include <lib/libcpp/iostream.h>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/type_traits.h>

namespace std
{

template< typename T, template <typename> typename List>
class ListIterator 
{

    public: 
    
        using Type = typename List<T>::ListElement;

        ListIterator(Type* ptr) : i_ptr(ptr){}

        ListIterator& operator ++ ()   //prefix operator
        {
            this->i_ptr = this->i_ptr->next;
            return *this;
        }

        ListIterator operator ++ (int) //postfix operator
        {
            ListIterator tmp = *this;
            this->i_ptr = this->i_ptr->next; //++(*this);

            return tmp;
        }

        ListIterator& operator -- ()   //prefix operator
        {
            this->i_ptr = this->i_ptr->previous;
            return *this;
        }

        ListIterator operator -- (int) //postfix operator
        {
            ListIterator tmp = *this;
            this->i_ptr = this->i_ptr->previous; //++(*this);

            return tmp;
        }

        bool operator != (const ListIterator& other)
        {
            if((uint32_t)this->i_ptr != (uint32_t)other.i_ptr)
                return true;
            else
                return false;
        }

        T& operator *()
        {
            return this->i_ptr->value;
        }

    private:
        Type* i_ptr;
};

template<typename T>
class List
{

    public:
    struct ListElement
    {
        T value;
        ListElement* next;
        ListElement* previous;
    };

    private:
    static uint8_t NoSuchElement;
    bool initialized;
    ListElement* FirstElement;
    uint32_t size;

    public:

    using Type = T;

    List() = default;
    List(const List& other) = default;
    List(std::initializer_list<T> items);
    List(T first);

    uint32_t size_get(void);
    ListElement* goto_last_element(void);
    void push(T value);
    void push_front(T value);
    void print(void);
    T pop(void);
    ListElement* operator[](T index);
    ListIterator<T, List> begin();
    ListIterator<T, List> end();

};

template<typename T>
List<T>::List(T first)
{
    this->initialized = true;
    size = 1;
    FirstElement = (ListElement*)malloc(SIZE_OF(FirstElement));
    FirstElement->next = NULL;
    FirstElement->previous = NULL;
    FirstElement->value = first;
}

template<typename T>
uint32_t List<T>::size_get(void)
{
    return this->size;
}

template<typename T>
typename List<T>::ListElement* List<T>::goto_last_element(void)
{
    ListElement* Tmp = FirstElement;
    while(Tmp->next != NULL)
        Tmp = Tmp->next;

    return Tmp;
}

template<typename T>
void List<T>::push(T value)
{

    if(!this->initialized)
    {
        this->size = 1;
        this->FirstElement = (ListElement*)malloc(SIZE_OF(FirstElement));
        this->FirstElement->next = NULL;
        this->FirstElement->previous = NULL;
        this->FirstElement->value = value;
    }

    ListElement* LastItem = this->goto_last_element();
    auto LastItemTmp = LastItem;

    LastItem->next = (ListElement*)malloc(SIZE_OF(ListElement));
    LastItem = LastItem->next;

    LastItem->value = value;
    LastItem->previous = LastItemTmp;
    LastItem->next = NULL;
    this->size++;
}

template<typename T>
void List<T>::push_front(T value)
{
    if(!this->initialized)
    {
        this->size = 1;
        this->FirstElement = (ListElement*)malloc(SIZE_OF(FirstElement));
        this->FirstElement->next = NULL;
        this->FirstElement->previous = NULL;
        this->FirstElement->value = value;
    }

    decltype(this->FirstElement) NewFirstElement;
    NewFirstElement = (ListElement*)malloc(SIZE_OF(ListElement));
    NewFirstElement->value = value;
    this->FirstElement = NewFirstElement;

    this->size++;
}

template<typename T>
void List<T>::print(void)
{
    ListElement* Tmp = this->FirstElement;
    std::cout << '[';
    while(Tmp->next != NULL)
    {
        std::cout << Tmp->value << ',';
        Tmp = Tmp->next;
    }
    std::cout << Tmp->value << ']';
    std::cout << std::endl;
}

template<typename T>
List<T>::List(std::initializer_list<T> items)
{
    size = 0;
    FirstElement = (ListElement*)malloc(SIZE_OF(FirstElement));
    FirstElement->next = NULL;
    FirstElement->previous = NULL;

    for(auto a : items)
        this->push(a);
}

template<typename T>
T List<T>::pop(void)
{
    ListElement* Tmp = goto_last_element();

    T ret = Tmp->value;
    Tmp->previous->next = NULL;

    free(Tmp);
    return ret;
}

template<typename T>
typename List<T>::ListElement* List<T>::operator[](T index)
{
    ListElement* Tmp = this->FirstElement;

    // if constexpr((std::is_pointer<T>::value) && (SIZE_OF(std::remove_pointer<T>) == SIZE_OF(char)))
    if constexpr(is_char_ptr(T))
    {
        while(!bstrcmp((char*)Tmp->value, (char*)index))
        {
            if(Tmp->next != NULL)
                Tmp = Tmp->next;
            else
                return nullptr;
        }
    }

    else
    {
        while(Tmp->value != index)
        {
            if(Tmp->next != NULL)
                Tmp = Tmp->next;
            else
                return nullptr;
        }
    }
    return Tmp;
}

template<typename T>
ListIterator<T, List> List<T>::begin()
{
    return ListIterator<T, List>(this->FirstElement);
}

template<typename T>
ListIterator<T, List> List<T>::end()
{
    return ListIterator<T, List>(nullptr);
}

}
