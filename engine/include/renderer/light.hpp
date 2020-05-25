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

        Light() = default;
        Light(const LightData& data)
            : lightData(data)
        {

        }

        Physics::GTransform gTransform;
        
        // Updates lightingData location
        void update() 
        {
            lightData.location = gTransform.transformMatrixNode->worldData.getTranslationVector();
        }
    };
}

#endif