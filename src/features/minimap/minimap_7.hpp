#pragma once

#include <windows.h>

#include "features/feature_common.hpp"

namespace features::minimap
{

class Minimap7 : public FeatureCommon
{
public:
    Minimap7();

private:
    __attribute__((naked)) static void codecave();
};

}  // namespace features::minimap
