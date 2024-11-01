#include "feature_common.hpp"

namespace features
{

FeatureCommon::FeatureCommon(uintptr_t adress, std::vector<BYTE>&& original_code) noexcept
    : m_adress{adress}, m_original_code{original_code}
{
}

}  // namespace features
