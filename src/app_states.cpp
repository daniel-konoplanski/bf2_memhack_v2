#include <atomic>

namespace app_states
{

std::atomic<bool> g_show_ui{false};
std::atomic<bool> g_enable_nametags{false};
std::atomic<bool> g_enable_maphack{false};
std::atomic<bool> g_imgui_initialized{false};
std::atomic<bool> g_minhook_initialized{false};

} // namespace app_states
