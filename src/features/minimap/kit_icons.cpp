#include "kit_icons.hpp"

namespace features::minimap
{

namespace
{

const uintptr_t renddx9_plus_0x00783C52 = constants::modules::get_renddx9_address() + 0x00783C52;
const uintptr_t renddx9_plus_0x00783C58 = constants::modules::get_renddx9_address() + 0x00783C58;

}  // namespace

KitIcons::KitIcons()
    : FeatureCommon(renddx9_plus_0x00783C52, {0x8A, 0x8E, 0x8D, 0x03, 0x00, 0x00}, &KitIcons::codecave) {}

__attribute__((naked)) void KitIcons::codecave()
{
    asm volatile (
        ".intel_syntax noprefix;"

        "mov cl, 0x01;"
        "jmp dword ptr [%0];"

        ".att_syntax;"
        :
        : "m"(renddx9_plus_0x00783C58)
    );
}

}  // namespace features::minimap
