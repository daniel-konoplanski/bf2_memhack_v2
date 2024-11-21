#pragma once

#include <windows.h>

namespace hooks::windows
{

LRESULT __stdcall hk_wndproc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);

} // namesapce hooks::windows
