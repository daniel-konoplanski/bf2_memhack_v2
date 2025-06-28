#include "nametags_display_distance.hpp"

#include "helpers/module_addresses.hpp"

namespace features::nametags
{

namespace
{

const uintptr_t renddx9_plus_0x00130357 = helpers::modules::get_renddx9_address() + 0x00130357;
const uintptr_t renddx9_plus_0x001303D5 = helpers::modules::get_renddx9_address() + 0x001303D5;
const uintptr_t renddx9_plus_0x00130365 = helpers::modules::get_renddx9_address() + 0x00130365;

}  // namespace

NametagsDisplayDistance::NametagsDisplayDistance()
    : FeatureCommon(renddx9_plus_0x00130357, {0x8B, 0x8E, 0xB8, 0x00, 0x00, 0x00}, &NametagsDisplayDistance::codecave)
{
}

__attribute__((naked)) void NametagsDisplayDistance::codecave()
{
    asm volatile (
        ".intel_syntax noprefix;"

        "mov ecx, dword ptr [esi + 0xB8];"
        "shr ecx, 0x3;"
        "test cl, 0x1;"
        "jne LABEL1;"
        "jmp dword ptr [%0];"

        "LABEL1:"
        "jmp dword ptr [%1];"

        ".att_syntax;"
        :
        : "m"(renddx9_plus_0x00130365), "m"(renddx9_plus_0x001303D5)
    );
}

}  // namespace features::nametags
