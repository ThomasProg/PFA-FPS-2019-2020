#ifndef _FLAT_SHADER_HPP_
#define _FLAT_SHADER_HPP_

#include "shader.hpp"

namespace Resources 
{
    class FlatShader : public Shader
    {
    private:
        GLint mvpID;             // index of "mvp"
        GLint colorID;           // index of "color"

    public:
        FlatShader(const char* vsFilename, const char* fsFilename)
            : Shader(vsFilename, fsFilename)
        {
            loadUniformValuesLocation();
        }

        void loadUniformValuesLocation() override;

        void useUniformValues(const Renderer::Camera& cam, const Renderer::Mesh&) const override;
    };
}


#endif