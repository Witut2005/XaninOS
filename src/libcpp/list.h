
#pragma once

#include <libc/stdlibx.h>
#include <libcpp/iostream.h>
#include <libcpp/initializer_list.hpp>

namespace std
{

template<class T>
class List
{
    struct ListElement
    {
        T value;
        ListElement* next;
        ListElement* previous;
    };

    private:

    static uint8_t NoSuchElement;
    ListElement* FirstElement;
    uint32_t size;

    public:

    uint32_t size_get(void)
    {
        return this->size;
    }

    List(T first)
    {
        size = 0;
        FirstElement = (ListElement*)malloc(sizeof(FirstElement));
        FirstElement->next = NULL;
        FirstElement->previous = NULL;
        FirstElement->value = first;
    }

    ListElement* goto_last_element(void)
    {
        ListElement* Tmp = FirstElement;
        while(Tmp->next != NULL)
        {
            Tmp = Tmp->next;
        }

        return Tmp;
    }

    void push(T value)
    {
        ListElement* LastItem = this->goto_last_element();
        auto LastItemTmp = LastItem;

        LastItem->next = (ListElement*)malloc(sizeof(ListElement));
        LastItem = LastItem->next;

        LastItem->value = value;
        LastItem->previous = LastItemTmp;
        LastItem->next = NULL;
    }

    void push_front(T value)
    {
        ListElement* LastItem = this->goto_last_element();
        auto LastItemTmp = LastItem;

        decltype(this->FirstElement) NewFirstElement;
        NewFirstElement = (ListElement*)malloc(sizeof(ListElement));

        NewFirstElement->value = value;
        ->previous = LastItemTmp;
        LastItem->next = NULL;
    }

    void print(void)
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
    
    List(std::initializer_list<T> items)
    {
        size = 0;
        FirstElement = (ListElement*)malloc(sizeof(FirstElement));
        FirstElement->next = NULL;
        FirstElement->previous = NULL;

        for(auto a : items)
            this->push(a);
    }

    T pop(void)
    {
        ListElement* Tmp = goto_last_element();

        T ret = Tmp->value;
        Tmp->previous->next = NULL;

        free(Tmp);
        return ret;
    }

    ListElement* operator[](T index)
    {
        ListElement* Tmp = this->FirstElement;

        if constexpr((std::is_pointer<T>::value) && (sizeof(std::remove_pointer<T>) == sizeof(char)))
        {
            while(!strcmp((char*)Tmp->value, (char*)index))
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


};

}
