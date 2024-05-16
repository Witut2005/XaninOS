
#pragma once
#include <sys/input/key_info.h>
#include <lib/libcpp/algorithm.h>
#include <lib/libcpp/container/vector.hpp>
#include <lib/libcpp/class.hpp>

class InputManager
{
public:
    MAKE_OBJECT_NON_COPYABLE(InputManager);

    template<class T>
    struct Table {
        std::vector<T> kernel;
        std::vector<T> user;
    };

    enum class EntryType {
        Kernel,
        User
    };

    enum class TableType {
        Observables,
        Handlers
    };

    using mapper_t = void (*)(uint8_t);

    static constexpr InputManager& the(void) { return s_instance; };

    void mapper_set(mapper_t mapper) { m_mapper = mapper; }
    mapper_t mapper_get(mapper_t mapper) { return m_mapper; }

    template<InputManager::TableType T, InputManager::EntryType Type>
    int add(const auto& entry);

    template<InputManager::TableType T, InputManager::EntryType Type>
    bool remove(int id);

    void user_handlers_remove(void) { m_handlers.user.clear(); }

    void handlers_call(KeyInfo key_info) { execute_on_tables<InputManager::TableType::Handlers>([&key_info](const InputHandler& handler) {handler.handler(key_info, handler.options.args);}); }
    void observables_update(KeyInfo key_info);

private:
    InputManager() = default;

    template<EntryType Type>
    constexpr std::vector<InputObservable>& observables_get(void) { return Type == EntryType::Kernel ? m_observables.kernel : m_observables.user; }

    template<EntryType Type>
    constexpr std::vector<InputHandler>& handlers_get(void) { return Type == EntryType::Kernel ? m_handlers.kernel : m_handlers.user; }

    template<TableType T, EntryType Type>
    constexpr auto& tables_get(void) {
        if constexpr (T == TableType::Observables) return observables_get<Type>();
        else if (T == TableType::Handlers) return handlers_get<Type>();
    }

    template<TableType T>
    void execute_on_tables(auto f);

    static InputManager s_instance;

    mapper_t m_mapper;
    Table<InputObservable> m_observables;
    Table<InputHandler> m_handlers;
};

template<InputManager::TableType T, InputManager::EntryType Type>
int InputManager::add(const auto& entry)
{
    auto& table = tables_get<T, Type>();
    table.push_back(entry);
    return table.size();
}

template<InputManager::TableType T, InputManager::EntryType Type>
bool InputManager::remove(int id)
{
    if (id < 0) return false;

    auto& table = tables_get <T, Type>();
    table.erase(table.begin() + id);

    return true;
}

template<InputManager::TableType T>
void InputManager::execute_on_tables(auto f)
{
    for (auto& a : tables_get<T, InputManager::EntryType::Kernel>()) {
        f(a);
    }

    for (auto& a : tables_get<T, InputManager::EntryType::User>()) {
        f(a);
    }
}
