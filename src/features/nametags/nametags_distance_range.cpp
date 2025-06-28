#include "nametags_distance_range.hpp"

#include "helpers/module_addresses.hpp"
#include "constants/constants.hpp"

namespace features::nametags
{

namespace
{

const uintptr_t range_address{ reinterpret_cast<uintptr_t>(&constants::RANGE) };
const uintptr_t renddx9_plus_0x00130368 = helpers::modules::get_renddx9_address() + 0x00130368;
const uintptr_t renddx9_plus_0x0013036E  = helpers::modules::get_renddx9_address() + 0x0013036E;

}  // namespace

NametagsDistanceRange::NametagsDistanceRange()
    : FeatureCommon(renddx9_plus_0x00130368, {0xD8, 0x1D, 0x84, 0x9A, 0x9F, 0x07}, &NametagsDistanceRange::codecave)
{
}

__attribute__((naked)) void NametagsDistanceRange::codecave()
{
    asm volatile (
        ".intel_syntax noprefix;"

        "fcomp dword ptr [%0];"
        "jmp dword ptr [%1];"

        ".att_syntax;"
        :
        : "m"(range_address), "m"(renddx9_plus_0x0013036E)
    );
}

}  // namespace features::nametags
