#include "minimap_1.hpp"

namespace features::minimap
{

namespace
{

constexpr uintptr_t address_0x007786CF = 0x007786CF;
constexpr uintptr_t address_0x007786DB = 0x007786DB;
constexpr uintptr_t address_0x00778846 = 0x00778846;
constexpr uintptr_t address_0x007786E7 = 0x007786E7;

}  // namespace

Minimap1::Minimap1()
    : FeatureCommon(address_0x007786CF, {0x85, 0xDB, 0x0F, 0x84, 0x6F, 0x01, 0x00, 0x00}, &Minimap1::codecave) {}

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
        : "m"(address_0x007786DB), "m"(address_0x00778846), "m"(address_0x007786E7)
    );
}

}  // namespace features::minimap
