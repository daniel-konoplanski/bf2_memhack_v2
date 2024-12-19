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

std::vector<HANDLE> threads_for_current_process()
{
    std::vector<HANDLE> threadHandles;
    DWORD currentProcessId = GetCurrentProcessId();

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        THREADENTRY32 te;
        te.dwSize = sizeof(THREADENTRY32);

        if (Thread32First(hSnapshot, &te))
        {
            do
            {
                if (te.th32OwnerProcessID == currentProcessId)
                {
                    HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME | THREAD_QUERY_INFORMATION, FALSE, te.th32ThreadID);

                    if (hThread != NULL)
                    {
                        threadHandles.push_back(hThread);
                    }
                    else
                    {
                        DWORD error = GetLastError();
                    }
                }
            } while (Thread32Next(hSnapshot, &te));
        }

        CloseHandle(hSnapshot);
    }
    else
    {
        DWORD error = GetLastError();
    }

    return threadHandles;
}

void mutually_exclusive(const std::function<void()>& action)
{
    const auto current_threads = threads_for_current_process();

    std::vector<HANDLE> handles;

    DWORD currentThreadId = GetCurrentThreadId();

    for (const auto& h : current_threads)
    {
        if (GetThreadId(h) != currentThreadId)
        {
            handles.push_back(h);
        }
        else
        {
            CloseHandle(h);
        }
    }

    for (const auto& handle : handles)
    {
        SuspendThread(handle);
    }

    action();

    for (const auto& handle : handles)
    {
        ResumeThread(handle);
        CloseHandle(handle);
    }
}

} // namespace helpers::windows
