#pragma once

#include <list>
#include <memory>

#include <features/i_feature.hpp>

namespace managers
{

class MinimapManager
{
public:
    static MinimapManager& instance();
    bool is_enabled();
    void enable();
    void disable();

private:
    MinimapManager();
    MinimapManager(const MinimapManager&) = delete;
    MinimapManager& operator=(const MinimapManager&) = delete;

private:
    bool m_enabled{false};
    std::list<features::IFeaturePtr> m_features{};
};

using MinimapManagerPtr = std::unique_ptr<MinimapManager>;

}  // namespace managers
