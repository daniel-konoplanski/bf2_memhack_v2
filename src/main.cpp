#include <cstdint>
#include <fstream>
#include <windows.h>
#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx9.h>

#include <constants/constants.hpp>
#include <constants/module_addresses.hpp>
#include <features/nametags/nametags_manager.hpp>
#include <features/minimap/minimap_manager.hpp>
#include <helpers/hooking.hpp>
#include <helpers/windows.hpp>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using helpers::hooking::EndSceneFn;
using helpers::hooking::WndProcFn;

EndSceneFn g_oendscene{};
WndProcFn g_owndproc{};

HRESULT __stdcall hk_endscene(IDirect3DDevice9* device)
{
    static bool is_initialized{false};

    if (!is_initialized)
    {
        ImGui::CreateContext();
        HWND main_window_handle = helpers::windows::get_main_window_handle();

        if (!ImGui_ImplWin32_Init(main_window_handle))
            return g_oendscene(device);

        if (!ImGui_ImplDX9_Init(device))
            return g_oendscene(device);

        is_initialized = true;
    }

    // Start the ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(400, 300), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(250, 250), ImGuiCond_FirstUseEver);
    ImGui::Begin("BF2_MEMHACK MENU", nullptr, ImGuiWindowFlags_None);
    ImGui::End();

    // Render ImGui
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    return g_oendscene(device);
}

LRESULT __stdcall hk_wndproc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, w_param, l_param))
        return true;

    return CallWindowProc(g_owndproc, hwnd, msg, w_param, l_param);
}

void __stdcall hook_functions()
{
    HWND main_window_handle = helpers::windows::get_main_window_handle();

    LPVOID endscene_address = helpers::hooking::get_endscene_address();
    LPVOID wndproc_address = reinterpret_cast<LPVOID>(GetWindowLongPtr(main_window_handle, GWLP_WNDPROC));
    LPVOID hk_endscene_address = reinterpret_cast<LPVOID>(&hk_endscene);
    LPVOID hk_wndproc_address = reinterpret_cast<LPVOID>(&hk_wndproc);

    MH_STATUS initialize_result = MH_Initialize();

    if (initialize_result != MH_OK)
        return;

    MH_STATUS endscene_hook_result = MH_CreateHook(endscene_address, hk_endscene_address, reinterpret_cast<LPVOID *>(&g_oendscene));
    MH_STATUS wndproc_hook_result = MH_CreateHook(wndproc_address, hk_wndproc_address, reinterpret_cast<LPVOID*>(&g_owndproc));

    if ((endscene_hook_result != MH_OK) || (wndproc_hook_result != MH_OK))
        return;

    MH_STATUS enable_endscene_result = MH_EnableHook(endscene_address);
    MH_STATUS enable_wndproc_result = MH_EnableHook(wndproc_address);
}

DWORD __stdcall cheatloop(LPVOID lpParam)
{
    using namespace managers;

    NametagsManagerPtr nametagsManager{std::make_unique<NametagsManager>()};
    MinimapManagerPtr minimapManager{std::make_unique<MinimapManager>()};

    bool nametag_enabled{false};
    bool minimap_enabled{false};

    SHORT previous_state_f10{0};
    SHORT previous_state_f11{0};

    hook_functions();

    while (true)
    {
        SHORT current_state_f10 = GetAsyncKeyState(VK_F10);
        SHORT current_state_f11 = GetAsyncKeyState(VK_F11);

        if ((current_state_f10 & 0x8000) && !(previous_state_f10 & 0x8000))
        {
            nametag_enabled ^= true;

            if (nametag_enabled)
                nametagsManager->enable();
            else
                nametagsManager->disable();
        }

        if ((current_state_f11 & 0x8000) && !(previous_state_f11 & 0x8000))
        {
            minimap_enabled ^= true;

            if (minimap_enabled)
                minimapManager->enable();
            else
                minimapManager->disable();
        }

        previous_state_f10 = current_state_f10;
        previous_state_f11 = current_state_f11;

        Sleep(constants::KEY_POLL_VAL_MS);
    }

    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        MessageBox(NULL, "bf2_memhack.dll successfully injected", "Injection Confirmation", MB_OK | MB_ICONINFORMATION);
        CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(cheatloop), NULL, 0, NULL);
    }

    return true;
}
