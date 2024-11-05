#pragma once

#include <windows.h>
#include <features/feature_common.hpp>

namespace features::nametags
{

class NametagsDisplayDistance : public FeatureCommon
{
public:
    NametagsDisplayDistance();

private:
    __attribute__((naked)) static void codecave();
};

}  // namespace features::nametags
