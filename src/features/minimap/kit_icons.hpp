#pragma once

#include <windows.h>

#include <features/i_feature.hpp>
#include <features/feature_common.hpp>
#include <helpers/module_addresses.hpp>
#include <helpers/memory_operations.hpp>

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
