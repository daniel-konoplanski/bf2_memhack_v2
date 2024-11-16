#pragma once

#include <atomic>

namespace app_states
{

extern std::atomic<bool> g_show_ui;
extern std::atomic<bool> g_enable_nametags;
extern std::atomic<bool> g_enable_maphack;
extern std::atomic<bool> g_imgui_initialized;
extern std::atomic<bool> g_minhook_initialized;

} // namespace app_states
