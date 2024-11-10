#include <atomic>
#include <cstdint>
#include <minwindef.h>
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
#include <winnt.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using helpers::hooking::EndSceneFn;
using helpers::hooking::WndProcFn;

EndSceneFn g_oendscene{};
WndProcFn g_owndproc{};

std::atomic<bool> g_show_ui{false};
std::atomic<bool> g_enable_nametags{false};
std::atomic<bool> g_enable_maphack{false};

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

    bool enable_nametags{g_enable_nametags.load()};
    bool enable_maphack{g_enable_maphack.load()};

    if(ImGui::Checkbox("Enable Nametags", &enable_nametags))
    {
        g_enable_nametags.store(enable_nametags);

        if (g_enable_nametags.load())
            g_nametag_manager.enable();
        else
            g_nametag_manager.disable();
    }

    if(ImGui::Checkbox("Enable Maphack", &enable_maphack))
    {
        g_enable_maphack.store(enable_maphack);

        if (g_enable_maphack.load())
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
    static bool is_ui_initialized{false};

    if (!is_ui_initialized)
    {
        bool initialization_result = initialize_imgui(device);

        if (!initialization_result)
            return g_oendscene(device);

        is_ui_initialized = true;
    }

    if (!g_show_ui.load())
        return g_oendscene(device);

    render_imgui_ui();

    return g_oendscene(device);
}

LRESULT __stdcall hk_wndproc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
    if (g_show_ui.load())
    {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, w_param, l_param))
            return 0;
    }

    return CallWindowProc(g_owndproc, hwnd, msg, w_param, l_param);
}

void __stdcall hook_functions()
{
    HWND main_window_handle = helpers::windows::get_main_window_handle();
    LPVOID endscene_address = helpers::hooking::get_endscene_address();
    LPVOID hk_endscene_address = reinterpret_cast<LPVOID>(&hk_endscene);

    MH_STATUS initialize_result = MH_Initialize();

    if (initialize_result != MH_OK)
        return;

    MH_STATUS endscene_hook_result = MH_CreateHook(endscene_address, hk_endscene_address, reinterpret_cast<LPVOID*>(&g_oendscene));

    if ((endscene_hook_result != MH_OK))
        return;

    MH_STATUS enable_endscene_result = MH_EnableHook(endscene_address);

    LONG_PTR original_wndproc_address = SetWindowLongPtr(main_window_handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hk_wndproc));

    if (!original_wndproc_address)
        return;

    g_owndproc = reinterpret_cast<WndProcFn>(original_wndproc_address);
}

DWORD __stdcall cheatloop(LPVOID)
{
    hook_functions();

    SHORT previous_state_f12{0};

    while (true)
    {
        SHORT current_state_f12 = GetAsyncKeyState(VK_F12);

        if ((current_state_f12 & 0x8000) && !(previous_state_f12 & 0x8000))
        {
            g_show_ui.store(!g_show_ui.load());
        }

        previous_state_f12 = current_state_f12;

        Sleep(constants::KEY_POLL_VAL_MS);
    }

    return 0;
}

void __stdcall on_dll_attach()
{
    const char* message_box_text{"dll successfully injected"};
    const char* message_box_caption{"BF2_MEMHACK"};

    MessageBox(NULL, message_box_text, message_box_caption, MB_OK | MB_ICONINFORMATION);
    CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(cheatloop), NULL, 0, NULL);
}

void __stdcall on_dll_detach()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
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
