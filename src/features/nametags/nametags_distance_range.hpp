#pragma once

#include <windows.h>
#include <features/feature_common.hpp>

namespace features::nametags
{

class NametagsDistanceRange : public FeatureCommon
{
public:
    NametagsDistanceRange();
    void enable() override;
    void disable() override;

private:
    __attribute__((naked)) static void codecave();
};

}  // namespace features::nametags
