#pragma once

#include <list>
#include <memory>

#include <features/i_feature.hpp>

namespace managers
{

class MinimapManager
{
public:
    MinimapManager();
    void enable();
    void disable();

private:
    std::list<std::unique_ptr<features::IFeature>> m_features{};
};

using MinimapManagerPtr = std::unique_ptr<MinimapManager>;

}  // namespace managers
