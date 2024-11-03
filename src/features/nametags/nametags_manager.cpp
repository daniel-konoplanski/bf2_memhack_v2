#include "nametags_manager.hpp"

#include <features/nametags/nametags.hpp>
#include <features/nametags/nametags_range.hpp>
#include <features/nametags/nametags_display_distance.hpp>

namespace managers
{

NametagsManager::NametagsManager()
{
    using namespace features::nametags;

    m_features.push_back(std::make_unique<Nametags>());
    m_features.push_back(std::make_unique<NametagsRange>());
    m_features.push_back(std::make_unique<NametagsDisplayDistance>());
}
    
void NametagsManager::enable()
{
    for (const auto& feature : m_features)
    {
        feature->enable();
    }
}

void NametagsManager::disable()
{
    for (const auto& feature : m_features)
    {
        feature->disable();
    }
}

}  // namespace managers
