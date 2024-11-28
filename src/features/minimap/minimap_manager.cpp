#include "minimap_manager.hpp"

#include <features/minimap/minimap_1.hpp>
#include <features/minimap/minimap_4.hpp>
#include <features/minimap/minimap_7.hpp>
#include <features/minimap/kit_icons.hpp>

namespace managers
{

MinimapManager& MinimapManager::instance()
{
    static MinimapManager instance{};
    return instance;
}

MinimapManager::MinimapManager()
{
    using namespace features::minimap;

    m_features.push_back(std::make_unique<Minimap1>());
    m_features.push_back(std::make_unique<Minimap4>());
    m_features.push_back(std::make_unique<Minimap7>());
    m_features.push_back(std::make_unique<KitIcons>());
}

bool MinimapManager::is_enabled()
{
    std::lock_guard<std::mutex> lock{m_mutex};

    return m_enabled;
}

void MinimapManager::enable()
{
    std::lock_guard<std::mutex> lock{m_mutex};

    for (const auto& feature : m_features)
    {
        feature->enable();
    }

    m_enabled = true;
}

void MinimapManager::disable()
{
    std::lock_guard<std::mutex> lock{m_mutex};

    for (const auto& feature : m_features)
    {
        feature->disable();
    }

    m_enabled = false;
}

}  // namespace managers
