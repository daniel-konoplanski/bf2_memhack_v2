#pragma once

#include <vector>
#include <windows.h>

#include "features/i_feature.hpp"

namespace features
{

class FeatureCommon : public IFeature
{
protected:
    FeatureCommon(uintptr_t adress, std::vector<BYTE>&& original_code, void (*codecave)()) noexcept;
    void enable() override;
    void disable() override;

protected:
    const uintptr_t m_adress;
    const std::vector<BYTE> m_original_code;
    void (*m_codecave)();
};

}  // namespace features
