#include "windows.hpp"

#include <cstddef>
#include <functional>
#include <tlhelp32.h>
#include <vector>

namespace helpers::windows
{

namespace
{

HWND __stdcall get_foreground_window()
{
    HWND foreground_window = GetForegroundWindow();
    DWORD current_process_id = GetCurrentProcessId();

    if (foreground_window)
    {
        DWORD foreground_process_id{};
        GetWindowThreadProcessId(foreground_window, &foreground_process_id);

        if (foreground_process_id == current_process_id)
            return foreground_window;
    }

    return nullptr;
}

BOOL __stdcall enum_windows_proc(HWND hwnd, LPARAM lParam)
{
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);

    if (processId == GetCurrentProcessId())
    {
        *reinterpret_cast<HWND*>(lParam) = hwnd;
        return FALSE;
    }

    return TRUE;
}

HWND __stdcall get_enum_windows()
{
    HWND hwnd{};
    EnumWindows(enum_windows_proc, reinterpret_cast<LPARAM>(&hwnd));
    return hwnd;
}

} // namespace

HWND __stdcall get_main_window_handle()
{
    HWND hwnd{};
    hwnd = get_foreground_window();

    if (hwnd)
        return hwnd;

    hwnd = get_enum_windows();

    return hwnd;
}

} // namespace helpers::windows
