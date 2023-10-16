
#pragma once

template<typename K, typename V, template <typename, typename> class UnorderedMap>
class UnorderedMapIterator 
{
    using Type = typename UnorderedMap<K, V>::ListElement;
    
    public:

    UnorderedMapIterator(Type* ptr) : i_ptr(ptr) {}

    UnorderedMapIterator& operator ++ () 
    {
        this->i_ptr = this->i_ptr->next;
        return *this;
    }

    UnorderedMapIterator operator ++ (int) 
    {
        UnorderedMapIterator tmp = this->i_ptr;
        this->i_ptr = this->i_ptr->next;
        return tmp;
    }

    UnorderedMapIterator& operator -- () 
    {
        this->i_ptr = this->i_ptr->previous;
        return this->i_ptr;
    }

    UnorderedMapIterator& previous()
    {
        return this->i_ptr->previous;
    }

    UnorderedMapIterator& next()
    {
        return this->i_ptr->next;
    }

    UnorderedMapIterator operator -- (int) 
    {
        UnorderedMapIterator tmp = this->i_ptr;
        this->i_ptr = this->i_ptr->previous;
        return tmp;
    }

    V& operator * ()
    {
        return this->i_ptr->item.second;
    }

    bool operator == (const UnorderedMapIterator x)
    {
        return this->i_ptr == x.i_ptr; 
    }

    bool operator != (const UnorderedMapIterator x)
    {
        return this->i_ptr != x.i_ptr; 
    }

    private:
    Type* i_ptr;

};
