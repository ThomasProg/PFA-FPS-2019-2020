#include "flatShader.hpp"

#include "mesh.hpp"
#include "lightManager.hpp"

Resources::FlatShader::FlatShader(const char* vsFilename, const char* fsFilename)
    : Shader(vsFilename, fsFilename)
{
    loadUniformValuesLocation();
}

void Resources::FlatShader::loadUniformValuesLocation()
{
    mvpID = getUniformLocation("mvp");  

    colorID = getUniformLocation("color");
}

void Resources::FlatShader::useUniformValues(const Renderer::Camera& cam, const Renderer::Mesh& mesh) const
{
    Core::Maths::Matrix4x4 mvp = (cam.viewProjectionMatrix * mesh.transform->transformMatrixNode->getWorldMatrix());
    mvp.transposeThis();

    glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

    glUniform4fv(colorID, 1, &mesh.color.components[0]);
}
