
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

    void print(void)
    {
        ListElement* Tmp = FirstElement;
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
        //FirstElement->value = first;

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

    T& operator[](uint32_t index)
    {
        ListElement* Tmp = this->FirstElement;

        for(int i = 0; i < index; i++)
        {
            if(Tmp->next != NULL)
                Tmp = Tmp->next;
            else
                return this->goto_last_element()->value;
        }
        return Tmp->value;
    }

    // ListElement* operator++(int);
    // ListElement* operator++(void);

    // ListElement* operator--(int);
    // ListElement* operator--(void);
    


};

}
