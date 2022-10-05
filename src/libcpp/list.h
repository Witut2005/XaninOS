
#pragma once

#include <libcpp/cstdlib.h>

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

    List(T first)
    {
        size = 0;
        FirstElement = (ListElement*)malloc(sizeof(FirstElement));
        FirstElement->next = nullptr;
        FirstElement->previous = nullptr;
        FirstElement->value = first;
    }

    ListElement* goto_last_element(void)
    {
        ListElement* Tmp = FirstElement;
        while(Tmp->next != nullptr)
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
        LastItem->next = nullptr;
    }

    void print(void)
    {
        ListElement* tmp = FirstElement;
        std::cout << '[';
        while(tmp->next != nullptr)
        {
            std::cout << tmp->value << ',';
            tmp = tmp->next;
        }
        std::cout << tmp->value << ']';
        std::cout << std::endl;
    }

    T pop(void);
    T operator[](uint32_t index);

    ListElement* operator++(int);
    ListElement* operator++(void);

    ListElement* operator--(int);
    ListElement* operator--(void);
    


};

}
