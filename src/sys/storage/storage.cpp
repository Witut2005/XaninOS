
#include "./storage.h"
#include <lib/libcpp/container/map.hpp>

static std::UnorderedMap<const char *, address_t> XaninLocalStorage;

// template <typename T>
// bool __storage_add(const char *name, T *value)
// {
//     if (XaninLocalStorage.exists(name))
//         return false;
//     XaninLocalStorage.insert(name, (address_t)value);
//     return true;
// }

// template <typename T>
// void __storage_set(const char *name, T *value)
// {
//     XaninLocalStorage.insert_or_assign(name, (address_t)value);
// }

// address_t __storage_get(const char *name)
// {
//     // return *XaninLocalStorage.find(name);
// }
// void __storage_remove(const char *name)
// {
//     XaninLocalStorage.remove(name);
// }