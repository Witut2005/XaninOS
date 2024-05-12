
#pragma once
#include <sys/input/key_info.h>
#include <lib/libcpp/container/vector.hpp>

class InputManager
{
public:

    enum class ObservableType {
        Kernel,
        User
    };

    enum class HandlerType {
        Kernel,
        User
    };

    void scan_code_mapper_set(void);

    //returns observable id
    template<ObservableType Type>
    int observable_add(const InputObservable& observable);

    template<ObservableType Type>
    bool observable_remove(int observable_id);

    template<HandlerType Type>
    int handler_add(const InputHandler& handler); //returns handler_id

    template<HandlerType Type>
    bool handler_remove(int handler_id);

    void handlers_call(void);

private:
    template<ObservableType Type>
    constexpr std::vector<InputObservable>& observables_get(void) { return Type == ObservableType::Kernel ? m_kernel_observables : m_user_observables; }

    template<HandlerType Type>
    constexpr std::vector<InputHandler>& handlers_get(void) { return Type == HandlerType::Kernel ? m_kernel_handlers : m_user_handlers; }

    static InputManager s_instance;

    std::vector<InputObservable> m_kernel_observables;
    std::vector<InputObservable> m_user_observables;

    std::vector<InputHandler> m_kernel_handlers;
    std::vector<InputHandler> m_user_handlers;
};


template<InputManager::ObservableType Type>
int InputManager::observable_add(const InputObservable& observable)
{
    auto& observables = observables_get <Type>();
    observables.push_back(observable);
    return observables.size();
}

template<InputManager::ObservableType Type>
bool InputManager::observable_remove(int observable_id)
{
    return true;
    // TODO std::remove like in vector
    // if constexpr (Type == InputManager::ObservableType::Kernel) {
    //     m_kernel_observables.push_back(observable);
    // }
    // else {
    //     m_user_observables.push_back(observable);
    // }
}

template<InputManager::HandlerType Type>
int InputManager::handler_add(const InputHandler& handler)
{

    auto& handlers = handlers_get<Type>();
    handlers.push_back(handler);
    return handlers.size();
}

template<InputManager::HandlerType Type>
bool InputManager::handler_remove(int handler_id)
{
    return true;
    //TODO
}