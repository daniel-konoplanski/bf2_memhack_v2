#pragma once

#include <mutex>

#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9.h>

namespace managers
{

class HookManager
{
public:
    static HookManager& instance();
    bool enable_hooks();
    void disable_hooks();
    bool is_initialized();

private:
    HookManager();
    HookManager(const HookManager&) = delete;
    HookManager& operator=(const HookManager&) = delete;

private:
    bool hook_functions();
    bool hook_function_endscene();
    bool hook_function_reset();
    bool hook_function_wndproc();

private:
    mutable std::mutex m_mutex{};
    bool m_initialized{false};
};

} // namesapce managers
