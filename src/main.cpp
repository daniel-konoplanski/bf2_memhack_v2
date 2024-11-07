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

using helpers::hooking::EndSceneFn;

EndSceneFn g_oendscene{};

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

    // Optionally set window position
    ImGui::SetNextWindowPos(ImVec2(400, 300), ImGuiCond_FirstUseEver);

    // Set the initial size to 800x600 pixels
    ImGui::SetNextWindowSize(ImVec2(250, 250), ImGuiCond_Always);

    // Your ImGui rendering code
    ImGui::Begin("Example Window");
    ImGui::Text("Hello, ImGui!");
    ImGui::End();

    // Render ImGui
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    return g_oendscene(device);
}

void __stdcall hook_endscene(void* endscene_address)
{
    // Initialize MinHook
    if (MH_Initialize() != MH_OK)
        return;

    // Create the hook for EndScene
    if (MH_CreateHook(endscene_address, reinterpret_cast<LPVOID>(&hk_endscene),
                      reinterpret_cast<LPVOID*>(&g_oendscene)) != MH_OK)
    {
        return;
    }

    // Enable the hook
    if (MH_EnableHook(endscene_address) != MH_OK)
        return;
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

    void* endscene_address = helpers::hooking::get_endscene_address();

    hook_endscene(endscene_address);

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
