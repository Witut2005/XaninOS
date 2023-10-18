
#pragma once

#include <lib/libcpp/container/iterator.hpp>

namespace std
{

template<class Li>
class ForwardListIterator : public ForwardIterator<Li>
{

    public: 
    using this_type = ForwardListIterator;

    using value_type = typename Li::value_type;
    using iterable_type = typename Li::iterable_type;
    using lreference = typename Li::lreference;

    ForwardListIterator<Li>(iterable_type* ptr) {this->i_ptr = ptr;}

    ForwardListIterator<Li>(const this_type& other) = default;
    ForwardListIterator<Li>(this_type&& other) {
        this->i_ptr = other.i_ptr;
        this->other.i_ptr = NULL;
    }

    this_type& operator ++ () override
    {
        this->i_ptr = this->i_ptr->next;
        return *this;
    }

    this_type&& operator ++ (int) override
    {
        this_type tmp = *this;
        this->i_ptr = this->i_ptr->next; 

        return std::move(tmp);
    }

    this_type& operator -- () override 
    {
        this->i_ptr = this->i_ptr->previous;
        return *this;
    }

    this_type&& operator -- (int) override
    {
        this_type tmp = *this;
        this->i_ptr = this->i_ptr->previous; 

        return std::move(tmp);
    }

    this_type&& operator + (int offset) override
    {            
        this_type tmp = *this;

        if(std::is_positive(offset)) 
        {
            for(int i = 0; i < offset; i++)
            {
                tmp.i_ptr = tmp.i_ptr->next;

                if(!tmp.i_ptr->next)
                    break;
            }
        }

        else 
        {
            for(int i = 0; i < offset; i++)
            {
                tmp.i_ptr = tmp.i_ptr->previous;

                if(!tmp.i_ptr->previous)
                    break;
            }
        }

        return tmp;
    }

    this_type&& operator - (int offset) override
    {
        this_type tmp = *this;

        if(std::is_negative(offset)) 
        {
            for(int i = 0; i < offset; i++)
            {
                tmp.i_ptr = tmp.i_ptr->next;

                if(!tmp.i_ptr->next)
                    break;
            }
        }

        else 
        {
            for(int i = 0; i < offset; i++)
            {
                tmp.i_ptr = tmp.i_ptr->previous;

                if(!tmp.i_ptr->previous)
                    break;
            }
        }

        return tmp;
    }

    this_type& operator = (const this_type& other) override 
    {
        *this = other;
        return *this;
    }

    this_type& operator = (this_type&& other)  override 
    {
        *this = other;
        other.i_ptr = NULL;
        return *this;
    }

    value_type& operator * () override
    {
        return this->i_ptr->value;
    }

    operator bool(void) const override
    {
        return this->valid();
    }

    bool valid(void) const override
    {
        return this->i_ptr != NULL;
    }

};

template<class Li>
class ReversedListIterator : public ReversedIterator<Li>
{

    public: 
    using this_type = ReversedListIterator;

    using value_type = typename Li::value_type;
    using iterable_type = typename Li::iterable_type;
    using lreference = typename Li::lreference;

    ReversedListIterator<Li>(iterable_type* ptr) {this->i_ptr = ptr;}

    ReversedListIterator<Li>(const this_type& other) = default;
    ReversedListIterator<Li>(this_type&& other) {
        this->i_ptr = other.i_ptr;
        other.i_ptr = NULL;
    }

    this_type& operator ++ () override 
    {
        this->i_ptr = this->i_ptr->previous;
        return *this;
    }

    this_type&& operator ++ (int) override
    {
        this_type tmp = *this;
        this->i_ptr = this->i_ptr->previous; 

        return std::move(tmp);
    }

    this_type& operator -- () override
    {
        this->i_ptr = this->i_ptr->next;
        return *this;
    }

    this_type&& operator -- (int)
    {
        this_type tmp = *this;
        this->i_ptr = this->i_ptr->next; 

        return tmp;
    }

    this_type operator + (int offset) override
    {
        this_type tmp = *this;

        if(std::is_negative(offset)) 
        {
            for(int i = 0; i < offset; i++)
            {
                tmp.i_ptr = tmp.i_ptr->next;

                if(!tmp.i_ptr->next)
                    break;
            }
        }

        else 
        {
            for(int i = 0; i < offset; i++)
            {
                tmp.i_ptr = tmp.i_ptr->previous;

                if(!tmp.i_ptr->previous)
                    break;
            }
        }

        return tmp;
    }

    this_type operator - (int offset) override
    {
        this_type tmp = *this;

        if(std::is_positive(offset)) 
        {
            for(int i = 0; i < offset; i++)
            {
                tmp.i_ptr = tmp.i_ptr->next;

                if(!tmp.i_ptr->next)
                    break;
            }
        }

        else 
        {
            for(int i = 0; i < offset; i++)
            {
                tmp.i_ptr = tmp.i_ptr->previous;

                if(!tmp.i_ptr->previous)
                    break;
            }
        }

        return tmp;
    }

    this_type& operator = (const this_type& other)  override 
    {
        *this = other;
        return *this;
    }

    this_type& operator = (this_type&& other)  override 
    {
        *this = other;
        other.i_ptr = NULL;
        return *this;
    }

    value_type& operator * () const override
    {
        return this->i_ptr->value;
    }

    operator bool (void) const override
    {
        this->valid();
    }

    bool valid(void) const override
    {
        return this->i_ptr != NULL;
    }

};

}