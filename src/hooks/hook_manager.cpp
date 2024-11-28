#include "hook_manager.hpp"

#include <MinHook.h>

#include <helpers/hooking.hpp>
#include <helpers/windows.hpp>
#include <hooks/functions/dx9.hpp>
#include <hooks/functions/windows.hpp>
#include <hooks/original_functions.hpp>

namespace managers
{

HookManager& HookManager::instance()
{
    static HookManager instance{};
    return instance;
}

HookManager::HookManager()
{
}

bool HookManager::enable_hooks()
{
    std::lock_guard<std::mutex> lock{m_mutex};

    MH_STATUS initialize_result = MH_Initialize();

    if (initialize_result != MH_OK)
        return false;

    m_initialized = true;

    bool enscene_hook_result = hook_function_endscene();
    bool reset_hook_result = hook_function_reset();
    bool wndproc_hook_result = hook_function_wndproc();

    return enscene_hook_result || reset_hook_result || wndproc_hook_result;
}

void HookManager::disable_hooks()
{
    std::lock_guard<std::mutex> lock{m_mutex};

    if (m_initialized)
    {
        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();

        m_initialized = false;
    }
}

bool HookManager::is_initialized()
{
    std::lock_guard<std::mutex> lock{m_mutex};

    return m_initialized;
}

bool HookManager::hook_function_endscene()
{
    LPVOID endscene_address = helpers::hooking::get_endscene_address();
    LPVOID hk_endscene_address = reinterpret_cast<LPVOID>(&hooks::dx9::hk_endscene);

    MH_STATUS endscene_hook_result =
        MH_CreateHook(endscene_address, hk_endscene_address, reinterpret_cast<LPVOID*>(&hooks::g_original_endscene));

    if ((endscene_hook_result != MH_OK))
        return false;

    MH_STATUS enable_endscene_result = MH_EnableHook(endscene_address);

    if ((enable_endscene_result != MH_OK))
        return false;

    return true;
}

bool HookManager::hook_function_reset()
{
    LPVOID reset_address = helpers::hooking::get_reset_address();
    LPVOID hk_reset_address = reinterpret_cast<LPVOID>(&hooks::dx9::hk_reset);

    MH_STATUS reset_hook_result =
        MH_CreateHook(reset_address, hk_reset_address, reinterpret_cast<LPVOID*>(&hooks::g_original_reset));

    if ((reset_hook_result != MH_OK))
        return false;

    MH_STATUS reset_endscene_result = MH_EnableHook(reset_address);

    if (reset_endscene_result != MH_OK)
        return false;

    return true;
}

bool HookManager::hook_function_wndproc()
{
    HWND main_window_handle = helpers::windows::get_main_window_handle();
    LONG_PTR original_wndproc_address = SetWindowLongPtr(main_window_handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hooks::windows::hk_wndproc));

    if (!original_wndproc_address)
        return false;

    hooks::g_original_wndproc = reinterpret_cast<helpers::hooking::WndProcFn>(original_wndproc_address);

    return true;
}

} // namesapce managers
