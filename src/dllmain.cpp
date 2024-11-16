#include <atomic>
#include <cstdint>
#include <minwindef.h>
#include <windows.h>
#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9.h>

#include <MinHook.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx9.h>

#include <app_states.hpp>
#include <constants/constants.hpp>
#include <helpers/module_addresses.hpp>
#include <features/nametags/nametags_manager.hpp>
#include <features/minimap/minimap_manager.hpp>
#include <helpers/hooking.hpp>
#include <helpers/windows.hpp>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void __stdcall on_dll_detach();

using helpers::hooking::EndSceneFn;
using helpers::hooking::WndProcFn;
using helpers::hooking::ResetFn;

EndSceneFn g_oendscene{};
WndProcFn g_owndproc{};
ResetFn g_oreset{};

auto g_nametag_manager = managers::NametagsManager{};
auto g_minimap_manager = managers::MinimapManager{};

bool __stdcall initialize_imgui(IDirect3DDevice9* device)
{
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    HWND main_window_handle = helpers::windows::get_main_window_handle();

    if (!ImGui_ImplWin32_Init(main_window_handle))
      return false;

    if (!ImGui_ImplDX9_Init(device))
      return false;

    return true;
}

void __stdcall render_imgui_ui()
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::Begin("BF2_MEMHACK MENU", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

    bool enable_nametags{app_states::g_enable_nametags.load()};
    bool enable_maphack{app_states::g_enable_maphack.load()};

    if(ImGui::Checkbox("Enable Nametags", &enable_nametags))
    {
        app_states::g_enable_nametags.store(enable_nametags);

        if (app_states::g_enable_nametags.load())
            g_nametag_manager.enable();
        else
            g_nametag_manager.disable();
    }

    if(ImGui::Checkbox("Enable Maphack", &enable_maphack))
    {
        app_states::g_enable_maphack.store(enable_maphack);

        if (app_states::g_enable_maphack.load())
            g_minimap_manager.enable();
        else
            g_minimap_manager.disable();
    }

    ImGui::End();

    ImGui::SetWindowFocus("BF2_MEMHACK MENU");

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

HRESULT __stdcall hk_endscene(IDirect3DDevice9* device)
{
    if (!app_states::g_imgui_initialized.load())
    {
        bool initialization_result = initialize_imgui(device);

        if (!initialization_result)
            return g_oendscene(device);

        app_states::g_imgui_initialized.store(true);
    }

    HRESULT result = device->TestCooperativeLevel();

    if (result == D3DERR_DEVICELOST || result == D3DERR_DEVICENOTRESET)
        return g_oendscene(device);

    if (!app_states::g_show_ui.load())
        return g_oendscene(device);

    render_imgui_ui();

    return g_oendscene(device);
}

LRESULT __stdcall hk_wndproc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
    if (msg == WM_KEYDOWN)
    {
        switch (w_param)
        {
        case VK_F12:
            app_states::g_show_ui.store(!app_states::g_show_ui.load());
            break;
        case VK_END:
            on_dll_detach();
            break;
        }
    }

    if (app_states::g_show_ui.load())
    {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, w_param, l_param))
            return 0;
    }

    return CallWindowProc(g_owndproc, hwnd, msg, w_param, l_param);
}

HRESULT __stdcall hk_reset(IDirect3DDevice9* p_device, D3DPRESENT_PARAMETERS* p_d3dpp)
{
    ImGui_ImplDX9_InvalidateDeviceObjects();

    ImGui_ImplWin32_Shutdown();

    auto main_window_handle = helpers::windows::get_main_window_handle();
    ImGui_ImplWin32_Init(main_window_handle);

    LONG_PTR original_wndproc_address = SetWindowLongPtr(main_window_handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hk_wndproc));

    g_owndproc = reinterpret_cast<WndProcFn>(original_wndproc_address);

    LPVOID reset_address = helpers::hooking::get_reset_address();
    LPVOID hk_reset_address = reinterpret_cast<LPVOID>(&hk_reset);

    MH_STATUS reset_hook_result = MH_CreateHook(reset_address, hk_reset_address, reinterpret_cast<LPVOID*>(&g_oreset));

    HRESULT hr = g_oreset(p_device, p_d3dpp);

    if (SUCCEEDED(hr))
    {
        ImGui_ImplDX9_CreateDeviceObjects();
    }

    return hr;
}

bool __stdcall hook_function_endscene()
{
    LPVOID endscene_address = helpers::hooking::get_endscene_address();
    LPVOID hk_endscene_address = reinterpret_cast<LPVOID>(&hk_endscene);

    MH_STATUS endscene_hook_result = MH_CreateHook(endscene_address, hk_endscene_address, reinterpret_cast<LPVOID*>(&g_oendscene));

    if ((endscene_hook_result != MH_OK))
        return false;

    MH_STATUS enable_endscene_result = MH_EnableHook(endscene_address);

    if ((enable_endscene_result != MH_OK))
        return false;

    return true;
}

bool __stdcall hook_function_reset()
{
    LPVOID reset_address = helpers::hooking::get_reset_address();
    LPVOID hk_reset_address = reinterpret_cast<LPVOID>(&hk_reset);

    MH_STATUS reset_hook_result = MH_CreateHook(reset_address, hk_reset_address, reinterpret_cast<LPVOID*>(&g_oreset));

    if ((reset_hook_result != MH_OK))
        return false;

    MH_STATUS reset_endscene_result = MH_EnableHook(reset_address);

    if (reset_endscene_result != MH_OK)
        return false;

    return true;
}

bool __stdcall hook_function_wndproc()
{
    HWND main_window_handle = helpers::windows::get_main_window_handle();
    LONG_PTR original_wndproc_address = SetWindowLongPtr(main_window_handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hk_wndproc));

    if (!original_wndproc_address)
        return false;

    g_owndproc = reinterpret_cast<WndProcFn>(original_wndproc_address);

    return true;
}

bool __stdcall hook_functions()
{
    MH_STATUS initialize_result = MH_Initialize();

    if (initialize_result != MH_OK)
        return false;

    app_states::g_minhook_initialized.store(true);

    bool enscene_hook_result = hook_function_endscene();
    bool reset_hook_result = hook_function_reset();
    bool wndproc_hook_result = hook_function_wndproc();

    return enscene_hook_result || reset_hook_result || wndproc_hook_result;
}

DWORD __stdcall bf2_memhack_init(LPVOID)
{
    const char* message_box_text{"dll successfully injected"};
    const char* message_box_caption{"BF2_MEMHACK"};

    MessageBox(NULL, message_box_text, message_box_caption, MB_OK | MB_ICONINFORMATION);

    bool result = hook_functions();

    if (!result)
        on_dll_detach();

    return 0;
}

void __stdcall on_dll_attach()
{
    CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(bf2_memhack_init), NULL, 0, NULL);
}

void __stdcall on_dll_detach()
{
    if (app_states::g_imgui_initialized.load())
    {
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    if (app_states::g_minhook_initialized.load())
    {
        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();
    }
}

BOOL WINAPI DllMain(HINSTANCE hinst_dll, DWORD fdw_reason, LPVOID lpv_reserved)
{
    switch (fdw_reason)
    {
        case DLL_PROCESS_ATTACH:
            on_dll_attach();
            break;
        case DLL_PROCESS_DETACH:
            on_dll_detach();
            break;
        default:
            break;
    }

    return true;
}
