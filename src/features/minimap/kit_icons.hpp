#pragma once

#include <windows.h>

#include "features/feature_common.hpp"

namespace features::minimap
{

class KitIcons : public FeatureCommon
{
public:
    KitIcons();

private:
    __attribute__((naked)) static void codecave();
};

}  // namespace features::minimap
