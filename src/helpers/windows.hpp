#pragma once

#include <functional>
#include <vector>
#include <windows.h>

namespace helpers::windows
{

HWND __stdcall get_main_window_handle();

void mutually_exclusive(const std::function<void()>& action);

std::vector<HANDLE> threads_for_current_process();

}  // namespace helpers::windows
