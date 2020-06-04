#include "texture.hpp"
#include "log.hpp"

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
        Core::Debug::Log::newLog("fail to load texture");
        
    glGenTextures(1, &textureData);
    glBindTexture(GL_TEXTURE_2D, textureData);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   
}

bool Resources::Texture::loadTexture(const char* filename, GLuint& data, int* width, int* height)
{
    // Load from file
    unsigned char* image_data;

    int tempWidth, tempHeight;
    image_data = stbi_load(filename, &tempWidth, &tempHeight, NULL, 4);

    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tempWidth, tempHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    data = image_texture;

    if (width != nullptr && height != nullptr)
    {
        *width  = tempWidth;
        *height = tempHeight;
    } 

    return true;
}