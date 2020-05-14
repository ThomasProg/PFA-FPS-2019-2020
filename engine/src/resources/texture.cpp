#include "texture.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

Resources::Texture::Texture(const char* pathToFile)
{
    if(pathToFile == nullptr)
        return;
    
    unsigned char* image = nullptr;
    int w, h;
    stbi_set_flip_vertically_on_load(true);
    image = stbi_load(pathToFile, &w, &h, nullptr, STBI_rgb_alpha);

    if(image == nullptr)
        std::cout << "fail to load texture" << std::endl;
        
    glGenTextures(1, &textureData);
    glBindTexture(GL_TEXTURE_2D, textureData);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   
}