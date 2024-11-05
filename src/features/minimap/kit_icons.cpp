#include "kit_icons.hpp"

namespace features::minimap
{

namespace
{

constexpr uintptr_t address_0x00783C52 = 0x00783C52;
constexpr uintptr_t address_0x00783C58 = 0x00783C58;

}  // namespace

KitIcons::KitIcons()
    : FeatureCommon(address_0x00783C52, {0x8A, 0x8E, 0x8D, 0x03, 0x00, 0x00}, &KitIcons::codecave) {}

__attribute__((naked)) void KitIcons::codecave()
{
    asm volatile (
        ".intel_syntax noprefix;"

        "mov cl, 0x01;"
        "jmp dword ptr [%0];"

        ".att_syntax;"
        :
        : "m"(address_0x00783C58)
    );
}

}  // namespace features::minimap
