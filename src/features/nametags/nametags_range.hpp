#pragma once

#include <windows.h>
#include <features/feature_common.hpp>

namespace features::nametags
{

class NametagsRange : public FeatureCommon
{
public:
    NametagsRange();
    void enable() override;
    void disable() override;

private:
    __attribute__((naked)) static void codecave();
};

}  // namespace features::nametags
