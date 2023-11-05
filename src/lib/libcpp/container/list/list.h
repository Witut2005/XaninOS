
#pragma once

#include <lib/libc/stdlibx.h>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/type_traits.h>
#include <lib/libcpp/function.hpp>
#include <sys/macros.h>
#include "./iterator.hpp"

namespace std
{

    template <typename T>
    struct ListElement
    {
        using value_type = T;
        T value;
        ListElement *next;
        ListElement *previous;
    };

    template <typename T>
    class List
    {

    private:
        uint32_t li_size;
        ListElement<T> *goto_last_element(void);
        bool empty_list_initializer(T value);

    public:
        ListElement<T> *Head;

        ListElement<T> *ListLowerBoundary;
        ListElement<T> *ListUpperBoundary;

        void new_head_push(ListElement<T> *head);
        void new_tail_push(ListElement<T> *tail);
        void head_replace(ListElement<T> *head);
        void tail_replace(ListElement<T> *tail);
        void head_replace(ListElement<T> *head, ListElement<T> *next);
        void tail_replace(ListElement<T> *, ListElement<T> *previous);

    public:
        using this_type = List<T>;
        using ListNode = ListElement<T>;

        using value_type = T;
        using iterable_type = ListElement<T> *;

        using lreference = T &;
        using rreference = T &&;

        using const_lreference = const T &;
        using const_rreference = const T &&;

        using forward_iterator = ForwardListIterator<this_type>;
        using reversed_iterator = ReversedListIterator<this_type>;

        using const_forward_iterator = ConstForwardListIterator<this_type>;
        using const_reversed_iterator = ConstReversedListIterator<this_type>;

        List();
        List(const List &other) = default;
        // List(List&& other);
        List(std::initializer_list<T> items);
        ~List();

        forward_iterator begin();
        forward_iterator end();
        reversed_iterator rbegin();
        reversed_iterator rend();

        const_forward_iterator cbegin();
        const_forward_iterator cend();
        const_reversed_iterator crbegin();
        const_reversed_iterator crend();

        uint32_t size(void);

        void clear(void);
        void remove(value_type value);

        template <typename InputIt>
        void erase(InputIt it)
        {

            static_assert((it.type == Types::ForwardListIterator) || (it.type == Types::ReversedListIterator));

            if (!(it.valid()))
                return;

            auto Tail = goto_last_element();

            auto erase_base_handler = [&]()
            {
                this->li_size--;
                free(it.i_ptr);
            };

            if (it.i_ptr == this->Head)
            {
                // std::cout << "head" << std::endl;
                this->head_replace(this->Head->next);
                erase_base_handler();
            }

            else if (it.i_ptr == Tail)
            {
                // std::cout << "LAST el" << std::endl;
                this->tail_replace(Tail->previous);
                erase_base_handler();
            }

            else
            {
                // std::cout << "nLAST el" << std::endl;
                if ((it.i_ptr != this->ListLowerBoundary) && (it.i_ptr != this->ListUpperBoundary))
                {
                    it.i_ptr->previous->next = it.i_ptr->next;
                    erase_base_handler();
                }
            }
        }

        template <typename InputIt>
        void erase(InputIt beg, InputIt end)
        {

            static_assert((beg.type == Types::ForwardListIterator) || (beg.type == Types::ReversedListIterator));
            static_assert((end.type == Types::ForwardListIterator) || (end.type == Types::ReversedListIterator));

            ListNode *Tail = goto_last_element();

            auto erase_base_handler = [&]()
            {
                this->li_size--;
                auto resource_to_free = beg++;
                free(resource_to_free.i_ptr);
            };

            if (!(beg.valid() & end.valid()))
                return;

            if (beg.i_ptr == this->Head)
            {
                while (beg.i_ptr != end.i_ptr)
                {
                    this->head_replace(this->Head->next);
                    erase_base_handler();
                }
            }

            else if (beg.i_ptr == Tail)
            {
                while (beg.i_ptr != end.i_ptr)
                {
                    this->tail_replace(goto_last_element()->previous);
                    erase_base_handler();
                }
            }

            else
            {
                while (beg != end)
                {
                    if ((beg.i_ptr != this->ListLowerBoundary) && (beg.i_ptr != this->ListUpperBoundary))
                    {
                        beg.i_ptr->previous->next = beg.i_ptr->next;
                        erase_base_handler();
                    }
                }
            }
        }

        void push_back(T value);
        void push_front(T value);

        void pop_back();
        void pop_front();

        ListNode *front(T index); // can return NULL
        ListNode *back(T index);  // can return NULL

        void print(void);

        template <class Li>
        friend class ForwardListIterator;

        template <class Li>
        friend class ReversedListIterator;

        template <class Li>
        friend class ConstForwardListIterator;

        template <class Li>
        friend class ConstReversedListIterator;

        template <typename K, typename V>
        friend class UnorderedMap;
    };

    template <typename T>
    List<T>::List()
    {
        this->li_size = 0;

        this->ListLowerBoundary = (ListNode *)calloc(SIZE_OF(ListNode));
        this->ListUpperBoundary = (ListNode *)calloc(SIZE_OF(ListNode));

        this->Head = this->ListUpperBoundary;
        this->ListLowerBoundary->previous = this->ListLowerBoundary;
        this->ListLowerBoundary->next = this->ListUpperBoundary;
    }

    template <typename T>
    bool List<T>::empty_list_initializer(T value)
    {

        if (this->li_size)
            return false;

        this->Head = (ListNode *)calloc(SIZE_OF(ListNode));

        this->Head->previous = this->ListLowerBoundary;
        this->Head->next = this->ListUpperBoundary;

        this->ListUpperBoundary->previous = this->Head;
        this->ListLowerBoundary->next = this->Head;

        this->Head->value = value;

        this->li_size = 1;
        return true;
    }

    template <typename T>
    List<T>::List(std::initializer_list<T> items) : List<T>()
    {
        for (auto a : items)
            this->push_back(a);
    }

    template <typename T>
    List<T>::~List()
    {
        this->clear();
    }

    template <typename T>
    typename List<T>::ListNode *List<T>::goto_last_element(void)
    {

        if (!this->li_size)
            return NULL;

        ListNode *tmp = this->Head;

        while (tmp->next != this->end().i_ptr)
            tmp = tmp->next;

        return tmp;
    }

    template <typename T>
    void List<T>::new_head_push(ListElement<T> *head)
    {

        // NewHead->previous already set to NULL
        head->next = this->Head;
        this->Head->previous = head; // set previous pointer to node

        head->previous = this->ListLowerBoundary;
        this->ListLowerBoundary->next = head;

        this->Head = head;
    }

    template <typename T>
    void List<T>::new_tail_push(ListElement<T> *tail)
    {
        ListNode *tmp = this->goto_last_element();

        tmp->next = tail;
        tail->previous = tmp;

        tail->next = this->ListUpperBoundary;
        this->ListUpperBoundary->previous = tail;
    }

    template <typename T>
    void List<T>::head_replace(ListElement<T> *head)
    {
        head->previous = this->ListLowerBoundary;
        this->Head = head;
        this->ListLowerBoundary->next = head;
    }

    template <typename T>
    void List<T>::tail_replace(ListElement<T> *tail)
    {

        if (tail == this->ListLowerBoundary)
        {
            this->Head = this->ListUpperBoundary;
            this->ListLowerBoundary->previous = this->ListLowerBoundary;
            this->ListLowerBoundary->next = this->ListUpperBoundary;
        }

        else
        {
            tail->next = this->ListUpperBoundary;
            this->ListUpperBoundary->previous = tail;
        }
    }

    template <typename T>
    void List<T>::head_replace(ListElement<T> *head, ListNode *next)
    {
        head->previous = this->ListLowerBoundary;
        this->Head = head;
        head->next = next;
        this->ListLowerBoundary->next = head;
    }

    template <typename T>
    void List<T>::tail_replace(ListElement<T> *tail, ListNode *previous)
    {
        ListNode *tmp = this->goto_last_element();
        tail->next = this->ListUpperBoundary;
        tail->previous = previous;
        this->ListUpperBoundary->previous = tmp;
    }

    template <typename T>
    typename List<T>::forward_iterator List<T>::begin()
    {
        return List<T>::forward_iterator(this->Head, *this);
    }

    template <typename T>
    typename List<T>::forward_iterator List<T>::end()
    {
        return List<T>::forward_iterator(this->ListUpperBoundary, *this);
    }

    template <typename T>
    typename List<T>::reversed_iterator List<T>::rbegin()
    {
        return List<T>::reversed_iterator(this->goto_last_element(), *this);
    }

    template <typename T>
    typename List<T>::reversed_iterator List<T>::rend()
    {
        return List<T>::reversed_iterator(this->ListLowerBoundary, *this);
    }

    template <typename T>
    typename List<T>::const_forward_iterator List<T>::cbegin()
    {
        return List<T>::const_forward_iterator(this->Head);
    }

    template <typename T>
    typename List<T>::const_forward_iterator List<T>::cend()
    {
        return List<T>::const_forward_iterator(this->ListUpperBoundary);
    }

    template <typename T>
    typename List<T>::const_reversed_iterator List<T>::crbegin()
    {
        return List<T>::const_reversed_iterator(this->goto_last_element());
    }

    template <typename T>
    typename List<T>::const_reversed_iterator List<T>::crend()
    {
        return List<T>::const_reversed_iterator(this->ListLowerBoundary);
    }

    template <typename T>
    uint32_t List<T>::size(void)
    {
        return this->li_size;
    }

    template <typename T>
    void List<T>::clear(void)
    {
        while (this->Head != this->end().i_ptr)
        {
            auto tmp = this->Head->next;
            free(this->Head);
            this->Head = tmp;
        }
        this->li_size = 0;
    }

    template <typename T>
    void List<T>::remove(List<T>::value_type value)
    {
        ListNode *next_tmp;
        for (auto tmp = this->Head; tmp != this->end().i_ptr; tmp = next_tmp)
        {
            next_tmp = tmp->next;
            if (tmp->value == value)
            {
                if (tmp == this->Head)
                {
                    this->head_replace(tmp->next);
                }

                else if (tmp == this->goto_last_element())
                {
                    this->tail_replace(tmp->previous);
                }

                else
                {
                    tmp->previous->next = tmp->next;
                    tmp->next->previous = tmp->previous;
                }

                this->li_size--;
                free(tmp);
            }
        }
    }

    template <typename T>
    void List<T>::push_back(T value)
    {

        if (this->empty_list_initializer(value))
            return;

        ListNode *ElementCreated = (ListNode *)calloc(SIZE_OF(ListNode));

        this->new_tail_push(ElementCreated);
        ElementCreated->value = value;

        this->li_size++;
    }

    template <typename T>
    void List<T>::push_front(T value)
    {
        if (this->empty_list_initializer(value))
            return;

        ListNode *NewHead = (ListNode *)calloc(SIZE_OF(ListNode));

        // NewHead->previous already set to NULL
        NewHead->next = this->Head;
        this->Head->previous = NewHead; // set previous pointer to node

        this->new_head_push(NewHead);
        NewHead->value = value;

        this->li_size++;
    }

    // CO JESLI LAST
    template <typename T>
    void List<T>::pop_back(void)
    {
        ListNode *tmp = goto_last_element();

        this->tail_replace(tmp->previous);
        this->li_size--;
        free(tmp); // realase resources from destroyed node
    }

    // CO JESLI LAST
    template <typename T>
    void List<T>::pop_front(void)
    {
        ListNode *tmp = this->Head;

        this->head_replace(this->Head->next);
        this->li_size--;
        free(tmp); // realase resources from destroyed node
    }

    template <typename T>
    void List<T>::print(void)
    {

        if (!this->li_size)
        {
            std::cout << "[]";
            return;
        }

        ListElement<T> *tmp = this->Head;
        ListElement<T> *end = this->end().i_ptr;
        std::cout << '[';
        while (tmp->next != end)
        {
            std::cout << tmp->value << ',';
            tmp = tmp->next;
        }
        std::cout << tmp->value << ']';
    }

}
