
#pragma once


// you can pass lambda as value_on_error
#define TRY(variable, error_or_obj, value_on_error) \
if (error_or_obj.valid() == false) return value_on_error; variable = error_or_obj.value()

typedef int error_code_t;

template <typename T>
class ErrorOr
{
public:
    ErrorOr(void) : m_valid(false) {}
    explicit ErrorOr(error_code_t error_code) : m_valid(false), m_error_code(error_code) {}
    ErrorOr(T value) : m_valid(true), m_value(value) {}

    bool valid(void) const;
    error_code_t error_code(void) const;
    T& value(void);

private:
    T m_value;
    bool m_valid;
    error_code_t m_error_code;
};

template<typename T>
bool ErrorOr<T>::valid(void) const
{
    return m_valid;
}

template<typename T>
T& ErrorOr<T>::value(void)
{
    return m_value;
}

template<typename T>
error_code_t ErrorOr<T>::error_code(void) const
{
    return m_error_code;
}