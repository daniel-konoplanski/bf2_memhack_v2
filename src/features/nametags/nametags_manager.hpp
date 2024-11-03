#pragma once

#include <list>
#include <memory>

#include <features/i_feature.hpp>

namespace managers
{

class NametagsManager
{
public:
    NametagsManager();
    void enable();
    void disable();

private:
    std::list<std::unique_ptr<features::IFeature>> m_features{};
};

using NametagsManagerPtr = std::unique_ptr<NametagsManager>;

}  // namespace managers
