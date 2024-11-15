#pragma once

#include <vector>
#include <cstdint>

#include <windows.h>

#include <features/i_feature.hpp>
#include <features/feature_common.hpp>
#include <helpers/module_addresses.hpp>
#include <helpers/memory_operations.hpp>

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
