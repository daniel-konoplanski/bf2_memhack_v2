#include "windows.hpp"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx9.h>

#include <app_states.hpp>
#include <gui/imgui_manager.hpp>
#include <hooks/original_functions.hpp>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace hooks::windows
{

LRESULT __stdcall hk_wndproc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
    if (msg == WM_KEYDOWN)
    {
        switch (w_param)
        {
        case VK_F12:
            bool old_state = managers::GuiManager::instance().is_rendering();
            managers::GuiManager::instance().set_rendering(!old_state);
            break;
        }
    }

    if (managers::GuiManager::instance().is_rendering())
    {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, w_param, l_param))
            return 0;
    }

    return CallWindowProc(g_original_wndproc, hwnd, msg, w_param, l_param);
}

} // namesapce hooks::windows
