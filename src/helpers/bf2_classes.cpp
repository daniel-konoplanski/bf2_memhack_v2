#include "bf2_classes.hpp"

#include <windows.h>

#include "helpers/module_addresses.hpp"

namespace helpers::bf2_classes
{

namespace
{
    // RendDX9.dll+23D098
    constexpr uintptr_t renderer_offset = 0x23D098;
}

::bf2_classes::CRenderer* get_renderer_address()
{
    using CRenderer = ::bf2_classes::CRenderer;

    uintptr_t renddx9_address = helpers::modules::get_renddx9_address();
    return *reinterpret_cast<CRenderer**>(renddx9_address + renderer_offset);
}

} // namespace helpers::bf2_classes
