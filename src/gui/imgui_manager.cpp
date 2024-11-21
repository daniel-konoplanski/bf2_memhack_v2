#include "imgui_manager.hpp"
#include "features/minimap/minimap_manager.hpp"
#include "features/nametags/nametags_manager.hpp"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx9.h>

#include <app_states.hpp>
#include <helpers/windows.hpp>

namespace managers
{

GuiManager::GuiManager()
{
}

GuiManager& GuiManager::instance()
{
    static GuiManager instance{};
    return instance;
}

bool GuiManager::initialize_imgui(IDirect3DDevice9* device)
{
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange | ImGuiConfigFlags_NavEnableKeyboard;

    HWND main_window_handle = helpers::windows::get_main_window_handle();

    if (!ImGui_ImplWin32_Init(main_window_handle))
        return false;

    if (!ImGui_ImplDX9_Init(device))
        return false;

    m_initialized = true;

    return true;
}

void GuiManager::deinitialize_imgui()
{
    if (m_initialized)
    {
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    m_initialized = false;
}

bool GuiManager::is_initialized()
{
    return m_initialized;
}

void GuiManager::render_ui()
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX));
    auto window_options = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
    ImGui::Begin("BF2_MEMHACK MENU", nullptr, window_options);

    render_content();

    ImGui::End();
    ImGui::SetWindowFocus("BF2_MEMHACK MENU");
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void GuiManager::render_content()
{
    bool enable_nametags{managers::NametagsManager::instance().is_enabled()};
    bool enable_maphack{managers::MinimapManager::instance().is_enabled()};

    if (ImGui::Checkbox("Enable Nametags", &enable_nametags))
    {
        if (enable_nametags)
            managers::NametagsManager::instance().enable();
        else
            managers::NametagsManager::instance().disable();
    }

    if (ImGui::Checkbox("Enable Maphack", &enable_maphack))
    {
        if (enable_maphack)
            managers::MinimapManager::instance().enable();
        else
            managers::MinimapManager::instance().disable();
    }
}

void GuiManager::set_rendering(bool state)
{
    m_render_state = state;
}

bool GuiManager::is_rendering()
{
    return m_render_state;
}

} // namesapce managers
