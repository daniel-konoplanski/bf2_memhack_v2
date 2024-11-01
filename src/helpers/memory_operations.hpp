#pragma once

#include <stdint.h>
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
void write_bytes(void* source_ptr, const BYTE* const data_to_write_ptr, uint32_t write_byte_size);

}  // namespace helpers::memory_operarions
