
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
    bool is_break_code(uint8_t scan_code) { return scan_code >= 0x80; }


    void handler_add(InputHandler handler) { handlers_get(handler.options.type).push_back(handler); }
    void handlers_call(void) { execute_on(m_handlers, [this](const InputHandler& handler) {handler.handler(m_key_info, handler.options.args);}); }
    bool handler_remove(int id, INPUT_TABLE_TYPE type) {
        auto& handlers = handlers_get(type);

        if (id < 0 || id >= handlers.end().index()) return false;
        handlers.erase(handlers.begin() + id);
        return true;
    }
    void user_handlers_remove(void) { m_handlers.user.clear(); }

private:
    InputManager() = default;

    constexpr std::vector<InputHandler>& handlers_get(INPUT_TABLE_TYPE type) { return type == INPUT_KERNEL ? m_handlers.kernel : m_handlers.user; }

    void execute_on(auto& table, auto f);

    static InputManager s_instance;

    bool m_dirty; //set to true if key_info_update is called by driver
    KeyInfo m_key_info;
    mapper_t m_mapper;
    Table<InputHandler> m_handlers;
};

void InputManager::execute_on(auto& table, auto f)
{
    for (auto& a : table.kernel) {
        f(a);
    }

    for (auto& a : table.user) {
        f(a);
    }
}