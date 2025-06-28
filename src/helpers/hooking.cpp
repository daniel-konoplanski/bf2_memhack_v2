#include "hooking.hpp"

#include <algorithm>
#include <array>

#include "helpers/bf2_classes.hpp"

namespace helpers::hooking
{

namespace
{

void** get_d3d_vtable()
{
    static std::array<void*, 119> vtable_copy{};
    static bool initialized = false;

    if (!initialized)
    {
        auto renderer_ptr = helpers::bf2_classes::get_renderer_address();
        auto dx9_device_ptr = renderer_ptr->m_game_device;

        void** vtable = *reinterpret_cast<void***>(dx9_device_ptr);
        std::copy(vtable, vtable + vtable_copy.size(), vtable_copy.begin());

        initialized = true;
    }

    return vtable_copy.data();
}

} // namespace

void* get_endscene_address()
{
    void** d3d_vtable = get_d3d_vtable();
    void* endscene_address = d3d_vtable[42];

    return endscene_address;
}

void* get_reset_address()
{
    void** d3d_vtable = get_d3d_vtable();
    void* reset_address = d3d_vtable[16];

    return reset_address;
}

}  // namespace helpers::hooking
