#pragma once

#include <memory>

namespace features
{

class IFeature
{
public:
    virtual ~IFeature() = default;
    virtual void enable() = 0;
    virtual void disable() = 0;
};

using IFeaturePtr = std::unique_ptr<features::IFeature>;

}  // features
