#include "minimap_1.hpp"

namespace features::minimap
{

namespace
{

const uintptr_t renddx9_plus_0x007786CF = constants::modules::get_renddx9_address() + 0x007786CF;
const uintptr_t renddx9_plus_0x007786DB = constants::modules::get_renddx9_address() + 0x007786DB;
const uintptr_t renddx9_plus_0x00778846 = constants::modules::get_renddx9_address() + 0x00778846;
const uintptr_t renddx9_plus_0x007786E7 = constants::modules::get_renddx9_address() + 0x007786E7;

}  // namespace

Minimap1::Minimap1()
    : FeatureCommon(renddx9_plus_0x007786CF, {0x85, 0xDB, 0x0F, 0x84, 0x6F, 0x01, 0x00, 0x00}, &Minimap1::codecave) {}

__attribute__((naked)) void Minimap1::codecave()
{
    asm volatile (
        ".intel_syntax noprefix;"

        "test ebx, ebx;"
        "je LABEL1;"
        "cmp ebx, ebx;"
        "jne LABEL2;"
        "jmp dword ptr [%0];"

        "LABEL1:"
        "jmp dword ptr [%1];"

        "LABEL2:"
        "jmp dword ptr [%2];"

        ".att_syntax;"
        :
        : "m"(renddx9_plus_0x007786DB), "m"(renddx9_plus_0x00778846), "m"(renddx9_plus_0x007786E7)
    );
}

}  // namespace features::minimap
