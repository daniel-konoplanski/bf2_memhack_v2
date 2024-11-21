#pragma once

#include <helpers/hooking.hpp>

namespace hooks
{

extern helpers::hooking::EndSceneFn g_original_endscene;
extern helpers::hooking::ResetFn g_original_reset;
extern helpers::hooking::WndProcFn g_original_wndproc;

} // namesapce hooks::dx9
