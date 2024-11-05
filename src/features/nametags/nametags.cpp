#include "nametags.hpp"

namespace features::nametags
{

namespace
{

const uintptr_t renddx9_plus_0x0012EDC2 = constants::modules::get_renddx9_address() + 0x0012EDC2;
const uintptr_t renddx9_plus_0x0012EEEB = constants::modules::get_renddx9_address() + 0x0012EEEB;
const uintptr_t renddx9_plus_0x0012EDF8 = constants::modules::get_renddx9_address() + 0x0012EDF8;

}  // namespace

Nametags::Nametags() : FeatureCommon(renddx9_plus_0x0012EDC2, {0x8B, 0x86, 0xB8, 0x00, 0x00, 0x00}, &Nametags::codecave)
{
}

__attribute__((naked)) void Nametags::codecave()
{
    asm volatile (
        ".intel_syntax noprefix;"

        "mov eax, [esi + 0xB8];"
        "shr eax, 0xA;"
        "test al, 0x1;"
        "jne LABEL1;"
        "mov edx, [edi];"
        "mov ecx, edi;"
        "call [edx + 0xE4];"
        "cmp eax, [esi + 0x80];"
        "jne LABEL1;"
        "mov eax, [ebx];"
        "mov ecx, ebx;"
        "call [eax + 0x68];"
        "test al, al;"
        "je LABEL1;"
        "jmp dword ptr [%0];"
        "LABEL1:"
        "jmp dword ptr [%1];"

        ".att_syntax;"
        :
        : "m"(renddx9_plus_0x0012EDF8), "m"(renddx9_plus_0x0012EEEB)  // input operands
    );
}

}  // namespace features::nametags
