#pragma once

#include <windows.h>

#include <features/i_feature.hpp>
#include <features/feature_common.hpp>
#include <helpers/module_addresses.hpp>
#include <helpers/memory_operations.hpp>

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
