#pragma once

#include <windows.h>

#include "features/feature_common.hpp"

namespace features::nametags
{

class Nametags : public FeatureCommon
{
public:
    Nametags();

private:
    __attribute__((naked)) static void codecave();
};

}  // namespace features::nametags
