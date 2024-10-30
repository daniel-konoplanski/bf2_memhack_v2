#pragma once

#include "../constants/assembly_instructions.hpp"
#include "memory_operations.hpp"

namespace helpers::memory_operarions
{

namespace
{

inline DWORD get_jump_adress(DWORD source_address, DWORD destination_address)
{
    return source_address - (destination_address + 5);
}

}  // namespace

void write_jump(void* source_ptr, void* destination_ptr, uint32_t write_size)
{
    using namespace constants;

    constexpr uint32_t JMP_INSTRUCTION_OFFSET{5};

    BYTE* source_byte_ptr = reinterpret_cast<BYTE*>(source_ptr);

    *source_byte_ptr = assembly::JMP;

    auto source_address  = reinterpret_cast<uintptr_t>(source_ptr);
    auto destination_address  = reinterpret_cast<uintptr_t>(destination_ptr);

    *reinterpret_cast<DWORD*>(source_byte_ptr + 1) = get_jump_adress(source_address, destination_address);

    for (uint32_t i{JMP_INSTRUCTION_OFFSET}; i < write_size; ++i)
    {
        *(source_byte_ptr + i) = assembly::NOOP;
    }
}

void write_bytes(void* source_ptr, const BYTE* const data_to_write_ptr, uint32_t write_size)
{
    BYTE* source_address{reinterpret_cast<BYTE*>(source_ptr)};

    for (int i{0}; i < write_size; ++i)
    {
        BYTE* source_address_offset = source_address + i;
        *source_address_offset = data_to_write_ptr[i];
    }
}

}  // namespace helpers::memory_operarions
