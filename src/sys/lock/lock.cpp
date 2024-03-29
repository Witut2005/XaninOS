
// #include "./lock.h"
// #include <lib/libcpp/container/map.hpp>

// static std::UnorderedMap<xanin_lock_id_t, address_t> XaninLockedObjects;

// namespace xlck
// {

//     template <typename T>
//     xanin_lock_id_t lock(T *object)
//     {
//         xanin_lock_id_t id = rand();

//         while (XaninLockedObjects.exists(id))
//             id = rand();

//         XaninLockedObjects.insert(id, object);
//     }

//     template <typename T>
//     bool unlock(uint32_t id)
//     {
//         XaninLockedObjects.remove(id);
//     }

//     template <typename T>
//     bool is_locked(T *object)
//     {
//         for (auto a : XaninLockedObjects)
//         {
//             if (a.second == object)
//                 return true;
//         }
//         return false;
//     }

//     bool set(xanin_lock_id_t id, address_t object, T value)
//     {
//         if (*XaninLockedObjects.find(id) == object)
//             return false;

//         *object = value;
//         return true;
//     }

//     void wait(void)
//     {
//     }

// }