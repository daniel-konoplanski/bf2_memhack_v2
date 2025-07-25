#include "memory_operations.hpp"

#include "constants/assembly_instructions.hpp"

namespace helpers::memory_operarions
{

namespace
{

inline DWORD get_jump_adress(DWORD source_address, DWORD destination_address)
{
    constexpr uint32_t JMP_BYTE_SIZE{5};
    return destination_address - (source_address + JMP_BYTE_SIZE);
}

}  // namespace

void write_jump(void* source_ptr, void* destination_ptr, uint32_t write_byte_size)
{
    using namespace constants;

    constexpr uint32_t JMP_INSTRUCTION_OFFSET{5};

    BYTE* source_byte_ptr = reinterpret_cast<BYTE*>(source_ptr);

    *source_byte_ptr = assembly::JMP;

    auto source_address  = reinterpret_cast<uintptr_t>(source_ptr);
    auto destination_address  = reinterpret_cast<uintptr_t>(destination_ptr);

    *reinterpret_cast<DWORD*>(source_byte_ptr + 1) = get_jump_adress(source_address, destination_address);

    for (uint32_t i{JMP_INSTRUCTION_OFFSET}; i < write_byte_size; ++i)
    {
        *(source_byte_ptr + i) = assembly::NOOP;
    }
}

void write_bytes(void* source_ptr, const std::vector<BYTE>& data_to_write)
{
    BYTE* source_address{reinterpret_cast<BYTE*>(source_ptr)};

    for (uint32_t i{0}; i < data_to_write.size(); ++i)
    {
        BYTE* source_address_offset = source_address + i;
        *source_address_offset = data_to_write[i];
    }
}

uintptr_t find_pattern(uintptr_t start_offset, uintptr_t size, BYTE* pattern, char mask[])
{
    uintptr_t pos{0u};
    int searchLen = strlen(mask) - 1;

    for (uintptr_t retAddress = start_offset; retAddress < start_offset + size; retAddress++)
    {
        if (*reinterpret_cast<BYTE*>(retAddress) == pattern[pos] || mask[pos] == '?')
        {
            if (mask[pos + 1] == '\0')
                return (retAddress - searchLen);

            pos++;
        }
        else
        {
            pos = 0;
        }
    }

    return 0;
}

}  // namespace helpers::memory_operarions
