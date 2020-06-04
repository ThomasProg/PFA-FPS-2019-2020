#ifndef _PHONG_FLAT_HPP_
#define _PHONG_FLAT_HPP_

#include "shader.hpp"

namespace Resources 
{
    class PhongFlat : public Shader
    {
    private:
        GLint lightsBlockID;     // index of "lightsBlock"
        GLint nbCurrentLightsID; // index of "nbCurrentLights"

        GLint mvpID;             // index of "mvp"
        GLint modelID;           // index of "model"
        GLint colorID;           // index of "color"

    public:
        PhongFlat(const char* vsFilename, const char* fsFilename);

        void loadUniformValuesLocation() override;

        void useUniformValues(const Renderer::Camera& cam, const Renderer::Mesh&) const override;
        void useLightsUniformValues(const Renderer::LightManager&) const override;
    };
}

#endif