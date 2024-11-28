#pragma once

#include <list>
#include <memory>
#include <mutex>

#include <features/i_feature.hpp>

namespace managers
{

class NametagsManager
{
public:
    static NametagsManager& instance();
    bool is_enabled();
    void enable();
    void disable();

private:
    NametagsManager();
    NametagsManager(const NametagsManager&) = delete;
    NametagsManager& operator=(const NametagsManager&) = delete;

private:
    mutable std::mutex m_mutex{};
    bool m_enabled{false};
    std::list<features::IFeaturePtr> m_features{};
};

using NametagsManagerPtr = std::unique_ptr<NametagsManager>;

}  // namespace managers
