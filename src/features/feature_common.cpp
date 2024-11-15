#include "feature_common.hpp"

#include <helpers/memory_operations.hpp>

namespace features
{

FeatureCommon::FeatureCommon(uintptr_t adress, std::vector<BYTE>&& original_code, void (*codecave)()) noexcept
    : m_adress{adress}
    , m_original_code{original_code}
    , m_codecave{codecave}
{
}

void FeatureCommon::enable()
{
    using namespace helpers::memory_operarions;

    DWORD old_protection{}, dummy_protection{};

    VirtualProtect(reinterpret_cast<LPVOID>(m_adress), m_original_code.size(), PAGE_EXECUTE_READWRITE, &old_protection);
    write_jump(reinterpret_cast<void*>(m_adress), reinterpret_cast<void*>(m_codecave), m_original_code.size());
    VirtualProtect(reinterpret_cast<LPVOID>(m_adress), m_original_code.size(), old_protection, &dummy_protection);
}

void FeatureCommon::disable()
{
    using namespace helpers::memory_operarions;

    DWORD old_protection{}, dummy_protection{};

    VirtualProtect(reinterpret_cast<LPVOID>(m_adress), m_original_code.size(), PAGE_EXECUTE_READWRITE, &old_protection);
    write_bytes(reinterpret_cast<void*>(m_adress), m_original_code);
    VirtualProtect(reinterpret_cast<LPVOID>(m_adress), m_original_code.size(), old_protection, &dummy_protection);
}

}  // namespace features
