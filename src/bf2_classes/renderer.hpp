#pragma once

#include <cstdint>
#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9.h>
#include <windows.h>

namespace bf2_classes
{

class CRenderer
{
  public:
    char _unused_0x0000[8];
    IDirect3D9* m_direct3d_device;
    IDirect3DDevice9* m_game_device;
    char _unused__0x0010[20];
    D3DXVECTOR2 m_screen_info;
    char _unused_0x002C[1648];
    BYTE N00FA8F34;
    BYTE N01061B57;
    BYTE N01053840;
    BYTE m_draw_water;
    BYTE N00FA8F33;
    BYTE N01048FDA;
    BYTE N0104978B;
    BYTE m_show_bodies;
    BYTE N00FA8F32;
    BYTE m_show_ground;
    BYTE m_show_grass;
    BYTE m_show_trees;
    BYTE m_show_friendly_tags;
    BYTE N00FBAEB4;
    BYTE N00FC0334;
    BYTE N00FBAEB5;
    int32_t m_draw_fps;
    BYTE N00DCC523;
    BYTE m_draw_hud;
    BYTE m_draw_sky;
    BYTE m_draw_sunflare;
    BYTE m_draw_console;
};

} // namespace bf2_classes
