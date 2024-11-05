#include "minimap_manager.hpp"

#include <features/minimap/minimap_1.hpp>
#include <features/minimap/minimap_4.hpp>
#include <features/minimap/minimap_7.hpp>
#include <features/minimap/kit_icons.hpp>

namespace managers
{

MinimapManager::MinimapManager()
{
    using namespace features::minimap;

    m_features.push_back(std::make_unique<Minimap1>());
    m_features.push_back(std::make_unique<Minimap4>());
    m_features.push_back(std::make_unique<Minimap7>());
    m_features.push_back(std::make_unique<KitIcons>());
}

void MinimapManager::enable()
{
    for (const auto& feature : m_features)
    {
        feature->enable();
    }
}

void MinimapManager::disable()
{
    for (const auto& feature : m_features)
    {
        feature->disable();
    }
}

}  // namespace managers
