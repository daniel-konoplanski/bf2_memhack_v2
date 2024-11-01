#include "module_addresses.hpp"

namespace constants::modules
{

void initialize_module_addresses()
{
    RENDDX9 = reinterpret_cast<uintptr_t>(GetModuleHandle("RendDX9.dll"));
}

uintptr_t RENDDX9;

}  // namespace constants::asm
