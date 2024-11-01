#pragma once

namespace features
{

class IFeature
{
public:
    virtual ~IFeature() = default;
    virtual void enable() = 0;
    virtual void disable() = 0;
};

}  // features
