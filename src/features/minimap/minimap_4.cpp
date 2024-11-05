#include "minimap_4.hpp"

namespace features::minimap
{

namespace
{

const uintptr_t renddx9_plus_0x007787A7 = constants::modules::get_renddx9_address() + 0x007787A7;
const uintptr_t renddx9_plus_0x007787B7 = constants::modules::get_renddx9_address() + 0x007787B7;
const uintptr_t renddx9_plus_0x00778765 = constants::modules::get_renddx9_address() + 0x00778765;
const uintptr_t renddx9_plus_0x00778809 = constants::modules::get_renddx9_address() + 0x00778809;
const uintptr_t renddx9_plus_0x007787C3 = constants::modules::get_renddx9_address() + 0x007787C3;

}  // namespace

Minimap4::Minimap4()
    : FeatureCommon(renddx9_plus_0x007787A7, {0x8B, 0x4D, 0xF4, 0x3B, 0x41, 0x04}, &Minimap4::codecave) {}

__attribute__((naked)) void Minimap4::codecave()
{
    asm volatile (
        ".intel_syntax noprefix;"

        "mov ecx, dword ptr [ebp - 0xC];"
        "cmp eax, dword ptr [ecx + 0x4];"
        "jne LABEL1;"
        "test ebx, ebx;"
        "je LABEL2;"
        "cmp ebx, ebx;"
        "jne LABEL3;"
        "jmp dword ptr [%0];"

        "LABEL1:"
        "jmp dword ptr [%1];"

        "LABEL2:"
        "jmp dword ptr [%2];"

        "LABEL3:"
        "jmp dword ptr [%3];"

        ".att_syntax;"
        :
        : "m"(renddx9_plus_0x007787B7), "m"(renddx9_plus_0x00778765), "m"(renddx9_plus_0x00778809), "m"(renddx9_plus_0x007787C3)
    );
}

}  // namespace features::minimap
