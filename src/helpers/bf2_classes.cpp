#include "bf2_classes.hpp"

#include <span>
#include <corecrt.h>
#include <minwindef.h>
#include <windows.h>

#include <helpers/module_addresses.hpp>
#include <helpers/memory_operations.hpp>

namespace helpers::bf2_classes
{

namespace
{

char class_manager_pattern[] =
    "\x89\x35\x99\x99\x99\x99\xFF\x15\x99\x99\x99\x99\x8B\x0D\x99\x99\x99\x99\x8B\x01";
char class_manager_pattern_mask[] = "xx????xx????xx????xx";

} // namespace

::bf2_classes::CClassManager* get_class_manager_address()
{
    using CClassManager = ::bf2_classes::CClassManager;

    HMODULE hModule = reinterpret_cast<HMODULE>(helpers::modules::get_renddx9_address());

    if (hModule == NULL)
        return nullptr;

    DWORD dwInitDLL = reinterpret_cast<DWORD>(GetProcAddress(hModule, "initDll"));

    if (!dwInitDLL)
        return nullptr;

    DWORD dwPointerOffset = helpers::memory_operarions::find_pattern(
        dwInitDLL, dwInitDLL + 512, reinterpret_cast<BYTE*>(class_manager_pattern), class_manager_pattern_mask);

    if (!dwPointerOffset)
        return nullptr;

    auto ptr_to_class_manager = reinterpret_cast<DWORD*>(dwPointerOffset + 2);

    return *reinterpret_cast<CClassManager**>(reinterpret_cast<DWORD>(*ptr_to_class_manager));
}

::bf2_classes::CRenderer* get_renderer_address()
{
    using CRenderer = ::bf2_classes::CRenderer;

    uintptr_t renddx9_address = helpers::modules::get_renddx9_address();
    return *reinterpret_cast<CRenderer**>(renddx9_address + 0x23D098);
}

} // namespace helpers::bf2_classes
