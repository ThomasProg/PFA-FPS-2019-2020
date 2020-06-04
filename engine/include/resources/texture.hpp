#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Resources
{
    class Texture
    {
    public:
        // Index of the texture on opengl.
        unsigned int textureData = 0;
        Texture(const char* pathToFile);
        ~Texture() {};

        // Loads a texture from the disk with the corresponding filename, 
        // and send it to the gpu.
        // - data is the index of the texture on opengl
        // - width and height are the width and the height of the loaded texture
        // Returns true if the texture is loaded correctly, false otherwise.
        static bool loadTexture(const char* filename, GLuint& data, int* width = nullptr, int* height = nullptr);
    };
}

#endif