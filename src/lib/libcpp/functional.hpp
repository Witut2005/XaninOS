//https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper

namespace detail
{
template<class T> constexpr T& FUN(T& t) noexcept { return t; }
template<class T> void FUN(T&&) = delete;
}

template<class T>
class reference_wrapper
{
public:
    // types
    using value_type = T;

    constexpr reference_wrapper(reference_wrapper<T>&& other) : m_ptr(other.m_ptr) { other.m_ptr = nullptr; }
    constexpr reference_wrapper(const T& other) { m_ptr = &(const_cast<T&>(other)); } // we need to use normal reference :((    

    reference_wrapper(const reference_wrapper&) = default;

    // assignment
    reference_wrapper& operator=(const reference_wrapper& x) = default;

    // access
    // constexpr operator T& () const { return *m_ptr; }

    constexpr T& get() { return *m_ptr; }
    constexpr const T& get() const { return *m_ptr; }
    constexpr T& set(T* ptr) const { m_ptr = ptr; return *m_ptr; }

private:
    T* m_ptr;
};
