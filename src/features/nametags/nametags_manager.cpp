#include "nametags_manager.hpp"

#include "features/nametags/nametags.hpp"
#include "features/nametags/nametags_range.hpp"
#include "features/nametags/nametags_display_distance.hpp"
#include "features/nametags/nametags_distance_range.hpp"

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
    std::lock_guard<std::mutex> lock{m_mutex};

    return m_enabled;
}

void NametagsManager::enable()
{
    std::lock_guard<std::mutex> lock{m_mutex};

    for (const auto& feature : m_features)
    {
        feature->enable();
    }

    m_enabled = true;
}

void NametagsManager::disable()
{
    std::lock_guard<std::mutex> lock{m_mutex};

    for (const auto& feature : m_features)
    {
        feature->disable();
    }

    m_enabled = false;
}

}  // namespace managers
