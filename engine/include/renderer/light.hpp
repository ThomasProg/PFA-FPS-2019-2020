#ifndef _LIGHT_HPP_
#define _LIGHT_HPP_


#include "glad/glad.h"
#include "lightData.hpp"
#include "gTransform.hpp"

namespace Renderer
{
    class Light 
    {
    public:
        // Data sent to GPU
        LightData lightData;
        Physics::GTransform gTransform;

        Light() = default;
        inline Light(const LightData& data);
        inline Light(const Core::Maths::Vec3& location);
        Light(const Light&) = default;
        Light& operator=(Light&) = default;
        ~Light() = default;
        
        // Updates lightingData location
        inline void update();
    };
}

#include "light.inl"

#endif