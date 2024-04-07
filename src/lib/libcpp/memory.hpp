
#pragma once

#include <stdint.h>
#include <sys/devices/com/com.h>
#include <lib/libc/stdlibx.h>

#ifdef KERNEL_MODULE
#include <sys/pmmngr/alloc.h>
#define UNIQUE_PTR_FREE(p) kfree(p)
#else
#define UNIQUE_PTR_FREE(p) free(p)
#endif

namespace std {

template<typename T>
class UniquePtr
{

public:
    UniquePtr(T* ptr = nullptr)
        : m_ptr(ptr)
    {
    }
    UniquePtr(UniquePtr<T> const& obj) = delete;
    UniquePtr& operator=(UniquePtr<T> const& obj) = delete;
    UniquePtr(UniquePtr<T>&& obj)
    {
        m_ptr = obj.m_ptr;
        obj.res = nullptr;
    }

    ~UniquePtr<T>()
    {
        if (m_ptr != nullptr)
        {
            UNIQUE_PTR_FREE(m_ptr);
            m_ptr = nullptr;
        }
    }

    UniquePtr& operator=(UniquePtr<T>&& obj)
    {
        if (this != &obj)
        {
            if (m_ptr != nullptr)
            {
                UNIQUE_PTR_FREE(m_ptr);
            }
            m_ptr = obj.m_ptr;
            obj.m_ptr = nullptr;
        }
        return *this;
    }

    T* operator->() const
    {
        return m_ptr;
    }

    T& operator*(void) const
    {
        return *m_ptr;
    }

    operator bool(void) const
    {
        return m_ptr != nullptr;
    }

    T* get(void) const
    {
        return m_ptr;
    }

private:
    T* m_ptr;
};

} //namespace

// template<class K, class X>
// void make_unique(unique_ptr<K>& ptr, X obj)
// {
//     ptr.ptr = (X*)calloc(SIZE_OF(X));
//     *(ptr.ptr) = obj;
// }

// template<class T>
// class shared_ptr {

// private:
//     T* ptr;
//     uint32_t share_count;

// public:
//     uint32_t get_share_count() const
//     {
//         return this->share_count;
//     }

//     shared_ptr() { this->share_count = 1; }

//     shared_ptr(shared_ptr<T> const& copy)
//     {
//         *this = copy;
//         share_count++;
//     }

//     T* get(void) const
//     {
//         return this->ptr;
//     }

//     T& operator*(void)
//     {
//         return *ptr;
//     }

//     bool used()
//     {
//         return this->is_used;
//     }

//     ~shared_ptr()
//     {
//         this->share_count--;

//         if (!this->share_count)
//         {
//             free(this->ptr);
//         }
//     }

//     template<class K, class X>
//     friend void make_shared(shared_ptr<K>& ptr, X obj);

//     template<class X>
//     friend class weak_ptr;
// };

// template<class T>
// class weak_ptr {

// private:
//     T* ptr;
//     shared_ptr<T> parent;
//     uint32_t const* share_count;

// public:
//     uint32_t get_share_count()
//     {
//         return *this->share_count;
//     }

//     weak_ptr() { }

//     weak_ptr(weak_ptr<T> const& copy)
//     {
//         *this = copy;
//     }

//     weak_ptr& operator=(weak_ptr<T> const& copy)
//     {
//         *this = copy;
//     }

//     weak_ptr& operator=(shared_ptr<T>& copy)
//     {
//         this->ptr = copy.get();
//         this->share_count = &copy.share_count;
//         this->parent = copy;
//         return *this;
//     }

//     T* get(void)
//     {
//         return this->ptr;
//     }

//     shared_ptr<T>* lock()
//     {
//         if (!(*this->share_count))
//             return NULL;
//         else
//         {
//             return &this->parent;
//         }
//     }
// };

// template<class K, class X>
// void make_shared(shared_ptr<K>& ptr, X obj)
// {
//     ptr.ptr = (X*)calloc(SIZE_OF(X));
//     *(ptr.ptr) = obj;
// }

// }
