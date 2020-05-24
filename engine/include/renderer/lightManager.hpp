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

    public:
        std::vector<Light> lights;
        GLuint lightsUniformBuffer = 0;

        inline LightManager(unsigned int nbMaxLights, GLenum usage = GL_DYNAMIC_DRAW)
        {
            glGenBuffers(1, &lightsUniformBuffer);
            glBindBuffer(GL_UNIFORM_BUFFER, lightsUniformBuffer);
            glBufferData(GL_UNIFORM_BUFFER, nbMaxLights * sizeof(Renderer::LightData), NULL, usage);
        }
    };
}

#endif