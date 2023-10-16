
#pragma once

#include <lib/libcpp/container/iterator.hpp>

namespace std
{

template<class Li>
class ForwardListIterator : public ForwardIterator<Li>
{

    public: 
    
        using Type = typename Li::value_type;
        using iterable_type = typename Li::iterable_type;
        using this_type = ForwardListIterator;

        ForwardListIterator<Li>(iterable_type* ptr) {this->i_ptr = ptr;}
        ForwardListIterator<Li>(const ForwardListIterator<Li>& other) {this->i_ptr = other.i_ptr;}

        constexpr const char* type_info(void) 
        {
            return "ForwardListIterator";
        }

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

        ForwardIterator<Li>&& operator + (int offset) override
        {            
            ForwardListIterator<Li>tmp = *this;

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

                    if(!tmp.i_ptr->next)
                        break;
                }
            }

            return std::move(tmp);
        }

        ForwardIterator<Li>&& operator - (int offset) override
        {
            ForwardListIterator<Li>tmp = *this;

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

                    if(!tmp.i_ptr->next)
                        break;
                }
            }

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

        Type& operator * () override
        {
            return this->i_ptr->value;
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

};


template<class Li>
class ReversedListIterator : public ReversedIterator<Li>
{

    public: 
    
        using Type = typename Li::value_type;
        using iterable_type = typename Li::iterable_type;

        ReversedListIterator<Li>(iterable_type* ptr) {this->i_ptr = ptr;}
        ReversedListIterator<Li>(const ReversedListIterator<Li>& other) {this->i_ptr = other.i_ptr;}
        
        constexpr const char* type_info(void) 
        {
            return "ForwardListIterator";
        }

        ReversedIterator<Li>& operator ++ () override 
        {
            this->i_ptr = this->i_ptr->previous;
            return *this;
        }

        ReversedIterator<Li>&& operator ++ (int) override
        {
            ReversedListIterator tmp = *this;
            this->i_ptr = this->i_ptr->previous; 

            return std::move(tmp);
        }

        ReversedIterator<Li>& operator -- () override
        {
            this->i_ptr = this->i_ptr->next;
            return *this;
        }

        ReversedIterator<Li>&& operator -- (int)
        {
            ReversedListIterator tmp = *this;
            this->i_ptr = this->i_ptr->next; 

            return std::move(tmp);
        }

        ReversedIterator<Li>&& operator + (int) override
        {
            ReversedListIterator<Li>tmp(NULL);
            XANIN_DEBUG_RETURN(std::move(tmp));
        }

        ReversedIterator<Li>&& operator - (int) override
        {
            ReversedListIterator<Li>tmp(NULL);
            XANIN_DEBUG_RETURN(std::move(tmp));
        }

        ReversedIterator<Li>& operator = (const ReversedIterator<Li>& other)  override 
        {
            *this = other;
            return *this;
        }

        ReversedIterator<Li>& operator = (ReversedIterator<Li>&& other)  override 
        {
            *this = other;
            other.i_ptr = NULL;
            return *this;
        }

        Type& operator * ()
        {
            return this->i_ptr->value;
        }

        bool operator == (const ReversedIterator<Li>& other) override
        {
            return this->i_ptr == other.i_ptr;
        }

        bool operator != (const ReversedIterator<Li>& other) override
        {
            return this->i_ptr != other.i_ptr;
        }

        operator bool (void)
        {
            return this->i_ptr != NULL;
        }

};

}