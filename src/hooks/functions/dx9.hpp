#pragma once

#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9.h>

namespace hooks::dx9
{

HRESULT __stdcall hk_endscene(IDirect3DDevice9* device);
HRESULT __stdcall hk_reset(IDirect3DDevice9* p_device, D3DPRESENT_PARAMETERS* p_d3dpp);

} // namesapce hooks::dx9
