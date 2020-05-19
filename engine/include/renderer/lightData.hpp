#ifndef _LIGHT_DATA_HPP_
#define _LIGHT_DATA_HPP_

#include "vec3.hpp"
#include "vec4.hpp"

namespace Renderer
{
    // Light data updated to GPU 
    // This struct is used to set a uniform block.
    // Thus, the memory size should be aligned correctly.
    struct LightData
    {
        // Basic lighting data 
        Core::Maths::Vec4 ambient {0.0f, 0.0f, 0.0f, 0.3f};
        Core::Maths::Vec4 dif {1.0f, 1.0f, 1.0f, 0.2f};
        Core::Maths::Vec4 spec {0.2f, 0.2f, 0.2f, 0.2f};

        // updated by setLight() 
        // should not be updated anywhere else
        Core::Maths::Vec4 location {1.f, -28.f, 0, 0.0}; 

        Core::Maths::Vec4 spotDirection = Core::Maths::Vec3(0,1,0);

        float light_cutOff = 0.99; // spot light center cutoff
        float light_outerCutOff = 0.98; // spot light outer cutoff

        float constant = 1.0, linear = 0.00001, quadratic = 0.0000001; // attenuation values
        float brillance = 16; // value used to compute specular
        int lightType = 0; // 0 = pointLight, 1 = directionalLight, 2 = flashLight, 3 = spotLight
        float padding8 = 1; // to align the memory size with gpu's struct
    };
}

#endif