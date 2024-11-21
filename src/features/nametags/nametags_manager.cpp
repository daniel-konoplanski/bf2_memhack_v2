#include "nametags_manager.hpp"

#include <features/nametags/nametags.hpp>
#include <features/nametags/nametags_range.hpp>
#include <features/nametags/nametags_display_distance.hpp>
#include <features/nametags/nametags_distance_range.hpp>

namespace managers
{

NametagsManager& NametagsManager::instance()
{
    static NametagsManager instance{};
    return instance;
}

NametagsManager::NametagsManager()
{
    using namespace features::nametags;

    m_features.push_back(std::make_unique<Nametags>());
    m_features.push_back(std::make_unique<NametagsRange>());
    m_features.push_back(std::make_unique<NametagsDisplayDistance>());
    m_features.push_back(std::make_unique<NametagsDistanceRange>());
}

bool NametagsManager::is_enabled()
{
    return m_enabled;
}

void NametagsManager::enable()
{
    for (const auto& feature : m_features)
    {
        feature->enable();
    }

    m_enabled = true;
}

void NametagsManager::disable()
{
    for (const auto& feature : m_features)
    {
        feature->disable();
    }

    m_enabled = false;
}

}  // namespace managers
