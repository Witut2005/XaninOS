
#pragma once

#include <libc/stdlibx.h>
#include <libcpp/iostream.h>
#include <libcpp/initializer_list.hpp>
#include <libcpp/type_traits.h>

namespace std
{

template< typename T, template <typename> class List>
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
        // Type* i_ptr;


};

template<class T>
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
    ListElement* FirstElement;
    uint32_t size;

    public:

    // using Iterator = ListIterator<List<T>>;
    using Type = T;

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
        decltype(this->FirstElement) NewFirstElement;
        NewFirstElement = (ListElement*)malloc(sizeof(ListElement));
        NewFirstElement->value = value;
        this->FirstElement = NewFirstElement;
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

        // if constexpr((std::is_pointer<T>::value) && (sizeof(std::remove_pointer<T>) == sizeof(char)))
        if constexpr(is_char_ptr(T))
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

    ListIterator<T, List> begin()
    {
        return ListIterator<T, List>(this->FirstElement);
    }

    ListIterator<T, List> end()
    {
        return ListIterator<T, List>(nullptr);
    }



};

}
