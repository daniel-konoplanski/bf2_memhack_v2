#pragma once

#include <stdint.h>
#include <windows.h>

namespace helpers::memory_operarions
{

void write_jump(void* source_ptr, void* destination_ptr, uint32_t write_size);
void write_bytes(void* source_ptr, const BYTE* const data_to_write_ptr, uint32_t write_size);

}  // namespace helpers::memory_operarions
