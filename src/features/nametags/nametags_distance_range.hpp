#pragma once

#include <windows.h>
#include <features/feature_common.hpp>

namespace features::nametags
{

class NametagsDistanceRange : public FeatureCommon
{
public:
    NametagsDistanceRange();

private:
    __attribute__((naked)) static void codecave();
};

}  // namespace features::nametags
