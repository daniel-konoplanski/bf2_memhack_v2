#include "nametags.hpp"

namespace features::nametags
{

namespace
{

const uintptr_t renddx9_plus_0x0012EDC2 = constants::modules::get_renddx9_address() + 0x0012EDC2;
const uintptr_t renddx9_plus_0x0012EEEB = constants::modules::get_renddx9_address() + 0x0012EEEB;
const uintptr_t renddx9_plus_0x0012EDF8 = constants::modules::get_renddx9_address() + 0x0012EDF8;

}  // namespace

Nametags::Nametags() : FeatureCommon(renddx9_plus_0x0012EDC2, {0x8B, 0x86, 0xB8, 0x00, 0x00, 0x00})
{
}

void Nametags::enable()
{
    using namespace helpers::memory_operarions;

	VirtualProtect(reinterpret_cast<LPVOID>(m_adress), m_original_code.size(), PAGE_EXECUTE_READWRITE, nullptr);
    write_jump(reinterpret_cast<void*>(m_adress), reinterpret_cast<void*>(&Nametags::codecave), m_original_code.size());
}

void Nametags::disable()
{
    using namespace helpers::memory_operarions;

    write_bytes(reinterpret_cast<void*>(m_adress), m_original_code);
	VirtualProtect(reinterpret_cast<LPVOID>(m_adress), m_original_code.size(), PAGE_EXECUTE_READ, nullptr);
}

__attribute__((naked)) void Nametags::codecave()
{
    asm volatile (
        ".intel_syntax noprefix;"         // Switch to Intel syntax

        "mov eax, [esi + 0xB8];"          // MOV EAX, DWORD PTR DS:[ESI + 0xB8]
        "shr eax, 0xA;"                   // SHR EAX, 0xA
        "test al, 0x1;"                   // TEST AL, 0x1
        "jne LABEL1;"                     // JNE LABEL1
        "mov edx, [edi];"                 // MOV EDX, DWORD PTR DS:[EDI]
        "mov ecx, edi;"                   // MOV ECX, EDI
        "call [edx + 0xE4];"              // CALL DWORD PTR DS:[EDX + 0xE4]
        "cmp eax, [esi + 0x80];"          // CMP EAX, DWORD PTR DS:[ESI + 0x80]
        "jne LABEL1;"                     // JNE LABEL1
        "mov eax, [ebx];"                 // MOV EAX, DWORD PTR DS:[EBX]
        "mov ecx, ebx;"                   // MOV ECX, EBX
        "call [eax + 0x68];"              // CALL DWORD PTR DS:[EAX + 0x68]
        "test al, al;"                    // TEST AL, AL
        "je LABEL1;"                      // JE LABEL1
        "jmp DWORD PTR [%0];"             // JMP to the address RendDX9_0x0012EDF8
        "LABEL1:"
        "jmp DWORD PTR [%1];"             // JMP to the address RendDX9_0x0012EEEB

        ".att_syntax;"                    // Switch back to AT&T syntax
        :
        : "m"(renddx9_plus_0x0012EDF8), "m"(renddx9_plus_0x0012EEEB)  // input operands
    );
}

}  // namespace features::nametags
