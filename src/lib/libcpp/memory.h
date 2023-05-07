
#pragma once

#include <stdint.h>

namespace std
{

template <class T>
class unique_ptr 
{

    private:
        T* ptr;

    public:

        unique_ptr(const unique_ptr&) = delete;
        unique_ptr& operator = (const unique_ptr&) = delete;

        unique_ptr()
        {
            return;
        }

        T* get(void)
        {
            return this->ptr;
        }
    
        T& operator * (void)
        {
            return *ptr;
        }

        ~unique_ptr()
        {
            free(this->ptr);
        }

        template <class K, class X>
        friend void make_unique(unique_ptr<K>& ptr, X obj);

};

template <class K, class X>
void make_unique(unique_ptr<K>& ptr, X obj)
{
    ptr.ptr = (X*)calloc(sizeof(X));
    *(ptr.ptr) = obj;
}

template <class T>
class shared_ptr
{

    private:
        T* ptr;
        uint32_t share_count;

    public:

        uint32_t get_share_count() const
        {
            return this->share_count;
        }

        shared_ptr(){this->share_count = 1;}

        shared_ptr(const shared_ptr<T>& copy)
        {
            *this = copy;
            share_count++;
        }

        T* get(void) const
        {
            return this->ptr;
        }

        T& operator * (void)
        {
            return *ptr;
        }

        bool used()
        {
            return this->is_used;
        }

        ~shared_ptr()
        {
            this->share_count--;

            if(!this->share_count)
            {
                free(this->ptr);
            }
        }

        template <class K, class X>
        friend void make_shared(shared_ptr<K>& ptr, X obj);

        template <class X>
        friend class weak_ptr;
};




template <class T>
class weak_ptr 
{

    private:
        T* ptr;
        shared_ptr<T> parent;
        const uint32_t* share_count;

    public:

        uint32_t get_share_count()
        {
            return *this->share_count;
        }

        weak_ptr(){}

        weak_ptr(const weak_ptr<T>& copy)
        {
            *this = copy;
        }

        weak_ptr& operator = (const weak_ptr<T>& copy)
        {
            *this = copy;
        }

        weak_ptr& operator = (shared_ptr<T>& copy)
        {
            this->ptr = copy.get();
            this->share_count = &copy.share_count; 
            this->parent = copy;
            return *this;
        }

        T* get(void)
        {
            return this->ptr;
        }
    
        shared_ptr<T>* lock()
        {
            if(!(*this->share_count))
                return NULL;
            else
            {
                return &this->parent;
            }
        }


};

template <class K, class X>
void make_shared(shared_ptr<K>& ptr, X obj)
{
    ptr.ptr = (X*)calloc(sizeof(X));
    *(ptr.ptr) = obj;
}




}

