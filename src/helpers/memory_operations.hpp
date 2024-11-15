#pragma once

#include <corecrt.h>
#include <vector>
#include <cstdint>
#include <windows.h>

namespace helpers::memory_operarions
{

/* @description
 * Writes x86 jmp instruction to the destination_ptr in the location of the source_ptr
 * write_size is used to provide the entire size of the segment that will be overriten with the jmp instruction
 */
void write_jump(void* source_ptr, void* destination_ptr, uint32_t write_byte_size);

/* @description
 * Writes the data_to_write_ptr to the source_ptr in the location of the source_ptr
 * write_size is used to provide the entire size of the segment that will be overriten with the data
 */
void write_bytes(void* source_ptr, const std::vector<BYTE>& data_to_write);

/* @description
 * Finds the patterm in the provided memory sequence [start_offset, start_offset + size]
 * based on the provided byte pattern and search mask
 */
uintptr_t find_pattern(uintptr_t start_offset, uintptr_t size, BYTE* pattern, char mask[]);

}  // namespace helpers::memory_operarions
