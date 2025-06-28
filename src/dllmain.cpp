#include "gui/imgui_manager.hpp"

#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9.h>
#include <minwindef.h>
#include <synchapi.h>
#include <windows.h>

#include <MinHook.h>

#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <imgui.h>

#include <constants/constants.hpp>
#include <features/minimap/minimap_manager.hpp>
#include <features/nametags/nametags_manager.hpp>
#include <gui/imgui_manager.hpp>
#include <helpers/hooking.hpp>
#include <helpers/module_addresses.hpp>
#include <helpers/windows.hpp>
#include <hooks/functions/dx9.hpp>
#include <hooks/functions/windows.hpp>
#include <hooks/hook_manager.hpp>
#include <hooks/original_functions.hpp>
#include <worker.hpp>

void __stdcall on_dll_detach();

bool __stdcall initialize_imgui(IDirect3DDevice9* device)
{
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    HWND main_window_handle = helpers::windows::get_main_window_handle();

    if (!ImGui_ImplWin32_Init(main_window_handle))
        return false;

    if (!ImGui_ImplDX9_Init(device))
        return false;

    return true;
}

DWORD __stdcall bf2_memhack_init(LPVOID)
{
    const char* message_box_text{"dll successfully injected"};
    const char* message_box_caption{"BF2_MEMHACK"};

    MessageBox(NULL, message_box_text, message_box_caption, MB_OK | MB_ICONINFORMATION);

    threads::worker.start();

    bool result = managers::HookManager::instance().enable_hooks();

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
    threads::worker.stop();

    if (managers::GuiManager::instance().is_initialized())
    {
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    if (managers::HookManager::instance().is_initialized())
    {
        managers::HookManager::instance().disable_hooks();
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
