
#pragma once
#include <sys/input/key_info.h>
#include <lib/libcpp/algorithm.h>
#include <lib/libcpp/container/vector.hpp>
#include <lib/libcpp/class.hpp>

class InputManager
{
public:
    // MAKE_OBJECT_NON_COPYABLE(InputManager);

    template<class T>
    struct Table {
        std::vector<T> kernel;
        std::vector<T> user;
    };

    enum class EntryType {
        Kernel,
        User
    };

    enum class TableTypes {
        Observables,
        Handlers
    };

    static InputManager& the(void) { return s_instance; };

    void scan_code_mapper_set(void);

    template<InputManager::TableTypes T, InputManager::EntryType Type>
    int add(const auto& entry);

    template<InputManager::TableTypes T, InputManager::EntryType Type>
    bool remove(int id);

    void user_handlers_remove(void);

    void handlers_call(KeyInfo key_info);
    void observables_update(KeyInfo key_info);

private:
    template<EntryType Type>
    constexpr std::vector<InputObservable>& observables_get(void) { return Type == EntryType::Kernel ? m_observables.kernel : m_observables.user; }

    template<EntryType Type>
    constexpr std::vector<InputHandler>& handlers_get(void) { return Type == EntryType::Kernel ? m_handlers.kernel : m_handlers.user; }

    template<TableTypes T, EntryType Type>
    constexpr auto& tables_get(void) {
        if constexpr (T == TableTypes::Observables) return observables_get<Type>();
        else if (T == TableTypes::Handlers) return handlers_get<Type>();
    }

    template<TableTypes T>
    void execute_on_tables(auto f);

    static InputManager s_instance;

    Table<InputObservable> m_observables;
    Table<InputHandler> m_handlers;
};

template<InputManager::TableTypes T, InputManager::EntryType Type>
int InputManager::add(const auto& entry)
{
    auto& table = tables_get<T, Type>();
    table.push_back(entry);
    return table.size();
}

template<InputManager::TableTypes T, InputManager::EntryType Type>
bool InputManager::remove(int id)
{
    auto& table = tables_get <T, Type>();
    auto it = std::remove(table, id);
    auto end = table.end(); // must be done before resize

    table.resize(it.index());
    return it != end;
}

template<InputManager::TableTypes T>
void InputManager::execute_on_tables(auto f)
{
    for (auto& a : tables_get<T, InputManager::EntryType::Kernel>()) {
        f(a);
    }

    for (auto& a : tables_get<T, InputManager::EntryType::User>()) {
        f(a);
    }
}
