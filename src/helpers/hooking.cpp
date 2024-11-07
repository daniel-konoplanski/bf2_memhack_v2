#include "hooking.hpp"

namespace helpers::hooking
{

void* get_endscene_address()
{
    IDirect3D9* p_direct3d9 = Direct3DCreate9(D3D_SDK_VERSION);

    if (!p_direct3d9)
        return nullptr;

    D3DPRESENT_PARAMETERS params{};
    params.SwapEffect = D3DSWAPEFFECT_DISCARD;
    params.hDeviceWindow = GetForegroundWindow();
    params.Windowed = true;

    IDirect3DDevice9* device_ptr{};

    p_direct3d9->CreateDevice(D3DADAPTER_DEFAULT,
                              D3DDEVTYPE_HAL,
                              params.hDeviceWindow,
                              D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                              &params,
                              &device_ptr);

    void** vTable = *reinterpret_cast<void***>(device_ptr);
    void* endscene_address = vTable[42];

    return endscene_address;
}

}  // namespace helpers::hooking
