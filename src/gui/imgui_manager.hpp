#pragma once

#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9.h>

namespace managers
{

class GuiManager
{
public:
    static GuiManager& instance();
    bool initialize_imgui(IDirect3DDevice9* device);
    void deinitialize_imgui();
    bool is_initialized();
    bool is_rendering();
    void set_rendering(bool state);
    void render_ui();

private:
    GuiManager();
    GuiManager(const GuiManager&) = delete;
    GuiManager& operator=(const GuiManager&) = delete;

    void render_content();

private:
    bool m_initialized{false};
    bool m_render_state{false};
};

} // namesapce managers
