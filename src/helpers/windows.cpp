#include "windows.hpp"

namespace helpers::windows
{

namespace
{

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

}  // namespace

HWND get_main_window_handle()
{
    HWND hwnd = NULL;
    EnumWindows(enum_windows_proc, reinterpret_cast<LPARAM>(&hwnd));
    return hwnd;
}

}  // namespace helpers::windows
