#include <helpers/hooking.hpp>

namespace hooks
{

helpers::hooking::EndSceneFn g_original_endscene{};
helpers::hooking::ResetFn g_original_reset{};
helpers::hooking::WndProcFn g_original_wndproc{};

} // namesapce hooks::dx9
