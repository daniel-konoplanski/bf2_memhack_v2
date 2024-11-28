#include "dx9.hpp"
#include "hooks/functions/windows.hpp"
#include "gui/imgui_manager.hpp"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx9.h>

#include <hooks/functions/windows.hpp>
#include <helpers/hooking.hpp>
#include <helpers/windows.hpp>
#include <hooks/original_functions.hpp>
#include <gui/imgui_manager.hpp>

namespace hooks::dx9
{

HRESULT __stdcall hk_endscene(IDirect3DDevice9* device)
{
    if (!managers::GuiManager::instance().is_initialized())
    {
        bool initialization_result = managers::GuiManager::instance().initialize_imgui(device);

        if (!initialization_result)
            return g_original_endscene(device);
    }

    HRESULT result = device->TestCooperativeLevel();

    if (result == D3DERR_DEVICELOST || result == D3DERR_DEVICENOTRESET)
        return g_original_endscene(device);

    if (!managers::GuiManager::instance().is_rendering())
        return g_original_endscene(device);

    managers::GuiManager::instance().render_ui();

    return g_original_endscene(device);
}

HRESULT __stdcall hk_reset(IDirect3DDevice9* p_device, D3DPRESENT_PARAMETERS* p_d3dpp)
{
    ImGui_ImplDX9_InvalidateDeviceObjects();

    ImGui_ImplWin32_Shutdown();

    auto main_window_handle = helpers::windows::get_main_window_handle();
    ImGui_ImplWin32_Init(main_window_handle);

    LONG_PTR original_wndproc_address =
        SetWindowLongPtr(main_window_handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hooks::windows::hk_wndproc));

    g_original_wndproc = reinterpret_cast<helpers::hooking::WndProcFn>(original_wndproc_address);

    LPVOID reset_address = helpers::hooking::get_reset_address();
    LPVOID hk_reset_address = reinterpret_cast<LPVOID>(&hk_reset);

    MH_STATUS reset_hook_result = MH_CreateHook(reset_address, hk_reset_address, reinterpret_cast<LPVOID*>(&g_original_reset));

    HRESULT hr = g_original_reset(p_device, p_d3dpp);

    if (SUCCEEDED(hr))
    {
        ImGui_ImplDX9_CreateDeviceObjects();
    }

    return hr;
}

} // namesapce hooks::dx9
