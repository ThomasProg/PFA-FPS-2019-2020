#ifndef _LIGHT_MANAGER_HPP_
#define _LIGHT_MANAGER_HPP_

#include "light.hpp"

#include "glad/glad.h"
#include <vector>

namespace Renderer
{
    class LightManager
    {
    private:
        std::vector<Light> lights;

    public:
        GLuint lightsUniformBuffer = 0;
        bool hasLightChanged = true; // dirty flag to send the data to gpu again when lights are modified

        inline const std::vector<Light>& getLights() const
        {
            return lights;
        }

        inline std::vector<Light>& getLightsToModifyThem()
        {
            hasLightChanged = true;
            return lights;
        }

        inline LightManager(unsigned int nbMaxLights, GLenum usage = GL_DYNAMIC_DRAW)
        {
            glGenBuffers(1, &lightsUniformBuffer);
            glBindBuffer(GL_UNIFORM_BUFFER, lightsUniformBuffer);
            glBufferData(GL_UNIFORM_BUFFER, nbMaxLights * sizeof(Renderer::LightData), NULL, usage);
        }
    };
}

#endif