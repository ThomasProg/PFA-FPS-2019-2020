#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Resources
{
    class Texture
    {
    private:
        
    public:
        unsigned int textureData = 0;
        Texture(const char* pathToFile);
        ~Texture() {};

        static bool loadTexture(const char* filename, GLuint& data, int* width = nullptr, int* height = nullptr);
    };
}

#endif