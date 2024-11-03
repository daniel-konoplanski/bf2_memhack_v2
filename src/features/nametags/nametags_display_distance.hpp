#pragma once

#include <windows.h>
#include <features/feature_common.hpp>

namespace features::nametags
{

class NametagsDisplayDistance : public FeatureCommon
{
public:
    NametagsDisplayDistance();
    void enable() override;
    void disable() override;

private:
    __attribute__((naked)) static void codecave();
};

}  // namespace features::nametags
