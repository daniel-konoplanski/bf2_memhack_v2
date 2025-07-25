#pragma once

#include <d3d9.h>

#include <MinHook.h>

namespace helpers::hooking
{

using EndSceneFn = HRESULT(__stdcall*)(IDirect3DDevice9*);
using WndProcFn = LRESULT(__stdcall*)(HWND, UINT, WPARAM, LPARAM);
using ResetFn = HRESULT(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

void* get_endscene_address();
void* get_reset_address();

}  // namespace helpers::hooking
