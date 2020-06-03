#include "light.hpp"

inline Renderer::Light::Light(const Renderer::LightData& data)
    : lightData(data)
{

}

inline Renderer::Light::Light(const Core::Maths::Vec3& location)
{
    lightData.location = Core::Maths::Vec4{location, 1.f};
}

// Updates lightingData location
inline void Renderer::Light::update() 
{
    lightData.location = Core::Maths::Vec4{gTransform.transformMatrixNode->getWorldMatrix().getTranslationVector()};
}