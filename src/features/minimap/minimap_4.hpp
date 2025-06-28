#pragma once

#include <windows.h>

#include "features/feature_common.hpp"

namespace features::minimap
{

class Minimap4 : public FeatureCommon
{
public:
    Minimap4();

private:
    __attribute__((naked)) static void codecave();
};

}  // namespace features::minimap
