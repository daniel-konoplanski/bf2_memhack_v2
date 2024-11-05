#include "minimap_7.hpp"

namespace features::minimap
{

namespace
{

constexpr uintptr_t address_0x007836EB = 0x007836EB;
constexpr uintptr_t address_0x007836FB = 0x007836FB;
constexpr uintptr_t address_0x00783701 = 0x00783701;

}  // namespace

Minimap7::Minimap7()
    : FeatureCommon(address_0x007836EB, {0x8B, 0x17, 0x8B, 0xCF, 0x8B, 0xD8}, &Minimap7::codecave) {}

__attribute__((naked)) void Minimap7::codecave()
{
    asm volatile (
        ".intel_syntax noprefix;"

        "mov edx, dword ptr [edi];"
        "mov ecx, edi;"
        "mov ebx, eax;"
        "call dword ptr [edx + 0xE4];"
        "cmp eax, eax;"
        "jne LABEL1;"
        "jmp dword ptr [%0];"

        "LABEL1:"
        "jmp dword ptr [%1];"

        ".att_syntax;"
        :
        : "m"(address_0x007836FB), "m"(address_0x00783701)
    );
}

}  // namespace features::minimap
