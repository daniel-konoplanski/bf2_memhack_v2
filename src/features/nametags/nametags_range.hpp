#pragma once

#include <windows.h>

#include "features/feature_common.hpp"

namespace features::nametags
{

class NametagsRange : public FeatureCommon
{
public:
    NametagsRange();

private:
    __attribute__((naked)) static void codecave();
};

}  // namespace features::nametags
