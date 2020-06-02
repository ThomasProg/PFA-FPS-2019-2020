#include "flatShader.hpp"

#include "mesh.hpp"
#include "lightManager.hpp"

void Resources::FlatShader::loadUniformValuesLocation()
{
    mvpID = getUniformLocation("mvp");  

    colorID = getUniformLocation("color");
}

void Resources::FlatShader::useUniformValues(const Renderer::Camera& cam, const Renderer::Mesh& mesh) const
{
    Core::Maths::Matrix4x4 mvp = (cam.viewProjectionMatrix * mesh.transform->transformMatrixNode->worldData);
    mvp.transposeThis();

    glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

    // TODO : change &mesh.color.x to &mesh.components (or something like that)
    glUniform4fv(colorID, 1, &mesh.color.x);
}
