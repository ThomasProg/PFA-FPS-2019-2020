#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

namespace Resources
{
    class Texture
    {
    private:
        
    public:
        unsigned int textureData = 0;
        Texture(const char* pathToFile);
        ~Texture() {};
    };
}

#endif