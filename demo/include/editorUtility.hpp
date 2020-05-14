#ifndef _EDITOR_UTILITY_HPP_
#define _EDITOR_UTILITY_HPP_

namespace Renderer
{
    class Camera;
}
namespace Entity
{
    class BasicEntity;
}
namespace Core
{
    class Engine;
}

class SegmentHit;

class EditorUtility
{
private:

public:
    static bool isInFrontOfPlayer(Renderer::Camera& camera,  
                                  const Entity::BasicEntity& object,
                                  SegmentHit& hit);

    static void moveEntityWithCursor(Renderer::Camera& camera, Entity::BasicEntity& object, const Core::Engine& engine);

    static void rotateEntityWithCursor(Entity::BasicEntity& object, 
                                       const Core::Engine& engine, const unsigned int axis);

    static void scaleEntityWithCursor(Entity::BasicEntity& object, 
                                      const Core::Engine& engine, const unsigned int axis);
};

#endif