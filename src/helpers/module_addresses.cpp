#include "module_addresses.hpp"

namespace helpers::modules
{

uintptr_t get_renddx9_address()
{
    static uintptr_t renddx9 = reinterpret_cast<uintptr_t>(GetModuleHandle("RendDX9.dll"));
    return renddx9;
}

}  // namespace helpers::modules
