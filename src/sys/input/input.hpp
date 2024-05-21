
#pragma once
#include <sys/input/key_info.h>
#include <lib/libcpp/algorithm.h>
#include <lib/libcpp/container/vector.hpp>
#include <lib/libcpp/class.hpp>
#include <sys/input/input.h>

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
        Handlers
    };

    using mapper_t = void (*)(uint8_t);

    static constexpr InputManager& the(void) { return s_instance; };

    bool dirty_get(void) { return m_dirty; }
    void dirty_set(void) { m_dirty = true; }
    void dirty_clear(void) { m_dirty = false; }
    void dirty_toggle(void) { m_dirty = !m_dirty; }

    void mapper_set(mapper_t mapper) { m_mapper = mapper; }
    void mapper_call(uint8_t scan_code) { m_mapper(scan_code); }

    void key_info_update(KeyInfo key_info) { m_key_info = key_info; dirty_set(); } //is called by keyboard driver
    KeyInfo key_info_get(void) { return m_key_info; }
    KeyInfo& key_info_ref_get(void) { return m_key_info; } // use it carefully

    bool is_key_pressed(uint8_t scan_code, bool is_special) { return is_special ? m_key_info.special_keys_pressed[scan_code] : m_key_info.keys_pressed[scan_code]; }

    template<InputManager::TableType T, InputManager::EntryType Type>
    auto get(int id); //do not check if id is valid be careful :^)

    template<InputManager::TableType T, InputManager::EntryType Type>
    int add(const auto& entry);

    template<InputManager::TableType T, InputManager::EntryType Type>
    bool remove(int id);

    void user_handlers_remove(void) { m_handlers.user.clear(); }
    void handlers_call(void) { execute_on_tables<InputManager::TableType::Handlers>([this](const InputHandler& handler) {handler.handler(m_key_info, handler.options.args);}); }

private:
    InputManager() = default;

    template<EntryType Type>
    constexpr std::vector<InputHandler>& handlers_get(void) { return Type == EntryType::Kernel ? m_handlers.kernel : m_handlers.user; }

    template<TableType T, EntryType Type>
    constexpr auto& tables_get(void) {
        if (T == TableType::Handlers) return handlers_get<Type>();
    }

    template<TableType T>
    void execute_on_tables(auto f);

    static InputManager s_instance;

    bool m_dirty; //set to true if key_info_update is called by driver
    KeyInfo m_key_info;
    mapper_t m_mapper;
    Table<InputHandler> m_handlers;

};


template<InputManager::TableType T, InputManager::EntryType Type>
auto InputManager::get(int id)
{
    auto& table = tables_get<T, Type>();
    return table[id];
}

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
