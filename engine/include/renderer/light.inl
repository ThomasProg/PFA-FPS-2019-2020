#include "light.hpp"

inline Renderer::Light::Light(const Renderer::LightData& data)
    : lightData(data)
{

}

// Updates lightingData location
inline void Renderer::Light::update() 
{
    lightData.location = Core::Maths::Vec4{gTransform.transformMatrixNode->getWorldMatrix().getTranslationVector()};
}