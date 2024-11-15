#include "nametags_range.hpp"

#include <helpers/module_addresses.hpp>
#include <constants/constants.hpp>
#include <helpers/memory_operations.hpp>

namespace features::nametags
{

namespace
{

const uintptr_t range_adress = reinterpret_cast<uintptr_t>(&constants::RANGE);
const uintptr_t renddx9_plus_0x0012EEF7 = helpers::modules::get_renddx9_address() + 0x0012EEF7;
const uintptr_t renddx9_plus_0x0012EEFD = helpers::modules::get_renddx9_address() + 0x0012EEFD;

}  // namespace

NametagsRange::NametagsRange()
    : FeatureCommon(renddx9_plus_0x0012EEF7, { 0xD9, 0x05, 0x3C, 0x9A, 0x9A, 0x07 }, &NametagsRange::codecave)
{
}

__attribute__((naked)) void NametagsRange::codecave()
{
    asm volatile (
        ".intel_syntax noprefix;"

        "fld dword ptr [%0];"
        "jmp dword ptr [%1];"
        ".att_syntax;"
        :
        : "m"(range_adress), "m"(renddx9_plus_0x0012EEFD)
    );
}

}  // namespace features::nametags
