#include "editorUtility.hpp"

#include "camera.hpp"
#include "basicEntity.hpp"
#include "engine.hpp"

bool EditorUtility::isInFrontOfPlayer(Renderer::Camera& camera,  
                                      const Entity::BasicEntity& object,
                                      SegmentHit& hit)
{
    Segment3D seg;
    seg.p1 = {0.f, 0, 0};
    seg.p2 = {0.f, 0, -100};
    seg.p1 = camera.transform.transformMatrixNode->worldData * seg.p1;
    seg.p2 = camera.transform.transformMatrixNode->worldData * seg.p2;

    return (Collisions::boxSegmentCollision(object.colliderCompo->second.worldCollider, seg, hit));
}

void EditorUtility::moveEntityWithCursor(Renderer::Camera& camera, Entity::BasicEntity& object, const Core::Engine& engine)
{
    {
        Core::Maths::Vec3 dir = {0.f, - 200 * engine.deltaMouseLoc.y, 0};
        dir = camera.transform.transformMatrixNode->worldData * Core::Maths::Vec4{dir, 0};
        Core::Maths::Vec3 camWorld = dir;
        // dir = Core::Maths::Matrix4x4(object.mesh->transform.transformMatrixNode->worldData.getInverse()) * Core::Maths::Vec4{dir, 0};
        object.mesh->transform.transform.location += dir;
    }

    {
        Core::Maths::Vec3 dir = {200  * engine.deltaMouseLoc.x , 0, 0};
        dir = camera.transform.transformMatrixNode->worldData * Core::Maths::Vec4{dir, 0};
        // dir = Core::Maths::Matrix4x4(object.mesh->transform.transformMatrixNode->worldData.getInverse()) * Core::Maths::Vec4{dir, 0};
        object.mesh->transform.transform.location += dir;
    }

    object.mesh->transform.UpdateLocalTransformMatrix();
    object.mesh->transform.transformMatrixNode->setDirtySelfAndChildren();
    object.mesh->transform.transformMatrixNode->cleanUpdate();
}

void EditorUtility::rotateEntityWithCursor(Entity::BasicEntity& object, 
                                           const Core::Engine& engine, const unsigned int axis)
{
    {
        object.mesh->transform.transform.rotation[axis] += 2 * engine.deltaMouseLoc.x;
    }

    object.mesh->transform.UpdateLocalTransformMatrix();
    object.mesh->transform.transformMatrixNode->setDirtySelfAndChildren();
    object.mesh->transform.transformMatrixNode->cleanUpdate();
}

void EditorUtility::scaleEntityWithCursor(Entity::BasicEntity& object, 
                                          const Core::Engine& engine, const unsigned int axis)
{
    {
        object.mesh->transform.transform.scale[axis] += 2 * engine.deltaMouseLoc.x;
        if (object.mesh->transform.transform.scale[axis] < 0.f)
            object.mesh->transform.transform.scale[axis] = 0.f;
    }

    object.mesh->transform.UpdateLocalTransformMatrix();
    object.mesh->transform.transformMatrixNode->setDirtySelfAndChildren();
    object.mesh->transform.transformMatrixNode->cleanUpdate();
}
