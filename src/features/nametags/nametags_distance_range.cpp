#include <constants/module_addresses.hpp>
#include <helpers/memory_operations.hpp>

#include "nametags_distance_range.hpp"

namespace features::nametags
{

namespace
{

constexpr float range{ 2000.0f };
const uintptr_t range_address{ reinterpret_cast<uintptr_t>(&range) };
const uintptr_t renddx9_plus_0x00130368 = constants::modules::get_renddx9_address() + 0x00130368;
const uintptr_t renddx9_plus_0x0013036E  = constants::modules::get_renddx9_address() + 0x0013036E;

}  // namespace

NametagsDistanceRange::NametagsDistanceRange()
    : FeatureCommon(renddx9_plus_0x00130368, {0xD8, 0x1D, 0x84, 0x9A, 0x9F, 0x07}, &NametagsDistanceRange::codecave)
{
}

__attribute__((naked)) void NametagsDistanceRange::codecave()
{
    asm volatile (
        ".intel_syntax noprefix;"                                   // Switch to Intel syntax

        "fcomp DWORD PTR [%0];"
        "jmp DWORD PTR [%1];"

        ".att_syntax;"                                              // Switch back to AT&T syntax
        :
        : "m"(range_address), "m"(renddx9_plus_0x0013036E)
    );
}

}  // namespace features::nametags
