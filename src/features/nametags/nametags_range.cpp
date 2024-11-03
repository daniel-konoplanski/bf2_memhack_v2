#include <constants/module_addresses.hpp>
#include <helpers/memory_operations.hpp>

#include "nametags_range.hpp"

namespace features::nametags
{

namespace
{

constexpr DWORD range = 0x44160000;
const uintptr_t renddx9_plus_0x0012EEF7 = constants::modules::get_renddx9_address() + 0x0012EEF7;
const uintptr_t renddx9_plus_0x0012EEFD = constants::modules::get_renddx9_address() + 0x0012EEFD;
const uintptr_t range_adress = reinterpret_cast<uintptr_t>(&range);

}  // namespace

NametagsRange::NametagsRange() : FeatureCommon(renddx9_plus_0x0012EEF7, { 0xD9, 0x05, 0x3C, 0x9A, 0x9A, 0x07 })
{
}

void NametagsRange::enable()
{
    using namespace helpers::memory_operarions;
	VirtualProtect(reinterpret_cast<LPVOID>(m_adress), m_original_code.size(), PAGE_EXECUTE_READWRITE, nullptr);
    write_jump(reinterpret_cast<void*>(m_adress), reinterpret_cast<void*>(&NametagsRange::codecave), m_original_code.size());
}

void NametagsRange::disable()
{
    using namespace helpers::memory_operarions;

}

__attribute__((naked)) void NametagsRange::codecave()
{
    asm volatile (
        ".intel_syntax noprefix;"

        "fld dword ptr [%0];"                                     // Load the float value at address range_adr into the FPU stack
        "jmp DWORD PTR [%1];"                                     // Jump to the address stored in renddx9_plus_0x0012EEFD
        ".att_syntax;"                    
        :
        : "m"(range_adress), "m"(renddx9_plus_0x0012EEFD)         // Load RendDX9_0x0012EEFD into a register as a runtime address
    );
}

}  // namespace features::nametags
