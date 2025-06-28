#pragma once

#include <list>
#include <memory>
#include <mutex>

#include "features/i_feature.hpp"

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
    mutable std::mutex m_mutex{};
    bool m_enabled{};
    std::list<features::IFeaturePtr> m_features{};
};

using MinimapManagerPtr = std::unique_ptr<MinimapManager>;

}  // namespace managers
