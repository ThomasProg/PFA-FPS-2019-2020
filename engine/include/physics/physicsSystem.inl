#include "physicsSystem.hpp"

#include "box.hpp"

#include "physicComponent.hpp"
#include "collisionComponent.hpp"
#include "physicComponentInterface.hpp"
#include "collisionComponentInterface.hpp"

#include "gTransform.hpp"
#include "engine.hpp"

#include "segment3D.hpp"
#include "sphere.hpp"
#include "collisions.hpp"

#include <vector>

// // template<typename T = Sphere>
// inline Physics::PhysicsSystem::PhysicCompIt Physics::PhysicsSystem::addPhysicComponent(Physics::PhysicComponentInterface* physicComp)
// {
//     // physicComponents.emplace(entity, Physics::PhysicComponent());
//     // return Physics::PhysicsSystem::PhysicCompIt{entity, &physicComponents};
//     physicComponents.emplace_back(physicComp);
//     return Physics::PhysicsSystem::PhysicCompIt{physicComponents.size() - 1};
// }


// template<typename COLLISIONS_CALLBACKS>
// void Physics::PhysicsSystem::simulate(COLLISIONS_CALLBACKS& callbacks, Core::Engine& engine)
// {
//     // for (std::pair<const Entity::EntityID, Physics::PhysicComponent>& physicComp : physicComponents)
//     for (Physics::PhysicComponentInterface* physicComp : physicComponents)
//     {
//         physicComp->physicComp.collider.worldCollider.center = physicComp->physicComp.collider.transform->transform.location;
        
//         for (std::pair<const Entity::EntityID, Physics::CollisionComponent<Box>>& box : boxes)
//         {
//             box.second.worldCollider.transform = box.second.transform->transformMatrixNode->worldData;
//                                             //    * physicComp->collider.transform->transformMatrixNode->worldData.getInverse();
//         }

//         if (physicComp->physicComp.isEnabled)
//         {
//             Physics::PhysicsSystem::PhysicsAdditionalData playerIgnoreData;
//             playerIgnoreData.ignoredEntities.emplace(physicComp);
//             // simulatePhysics(physicComp, 
//             //                 playerIgnoreData, 
//             //                 physicComp->velocity,
//             //                 callbacks);
//             Sphere sphere;
//             sphere.center = physicComp->physicComp.collider.worldCollider.center;
//             sphere.radius = physicComp->physicComp.collider.worldCollider.radius; 
//         // if (leftVelocity.y > 0)
//         // std::cout << leftVelocity << std::endl;

//             std::map<Entity::EntityID, bool>::iterator it = physicComp->physicComp.collider.collidingEntities.begin();
//             while (it != physicComp->physicComp.collider.collidingEntities.end())
//             {
//                 it->second = false;
//                 ++it;
//             }

//             Core::Maths::Vec3 temp = physicComp->physicComp.collider.transform->transform.location + physicComp->physicComp.velocity; 
//             Core::Maths::Vec3& leftVelocity = physicComp->physicComp.velocity;
//             physicComp->physicComp.collider.transform->transform.location = simulatePhysicsForASphere(sphere, 
//                                                                                             playerIgnoreData,  
//                                                                                             physicComp->physicComp.collider.collidingEntities,
//                                                                                             leftVelocity,
//                                                                                             physicComp,
//                                                                                             callbacks);

//             std::map<Entity::EntityID, bool>::iterator itColliding = physicComp->physicComp.collider.collidingEntities.begin();
//             while (itColliding != physicComp->physicComp.collider.collidingEntities.end())
//             {
//                 if (!itColliding->second)
//                 {
//                     callbacks.onCollisionExit(physicComp);//it->first);  
//                     itColliding = physicComp->physicComp.collider.collidingEntities.erase(itColliding);
//                 }
//                 else 
//                     ++itColliding;
//             }
//             // bool bCollides = temp != physicComp.second.collider.transform->transform.location;

//             // if (physicComp.second.collider.isColliding)
//             //     physicComp.second.velocity *= linearDamping;

//             simulateGravity(physicComp->physicComp, engine);

//             physicComp->physicComp.collider.transform->UpdateLocalTransformMatrix();
//             physicComp->physicComp.collider.transform->transformMatrixNode->cleanUpdate();
//         }
//     }
// }

// bool Physics::PhysicsSystem::sphereCollisionWithBoxes(const Sphere& sphere, 
//                                                       const Core::Maths::Vec3& velocity,
//                                                       const Physics::PhysicsSystem::PhysicsAdditionalData& data, 
//                                                       SegmentHit& hit,
//                                                       Physics::CollisionComponentInterface<Box>*& collidedMeshInterface)
// {
//     bool hasCollided = false;
//     SegmentHit segmentHit;
    
//     Segment3D seg{sphere.center, sphere.center + velocity};

//     // for (std::pair<const Entity::EntityID, Physics::CollisionComponent<Box>>& boxCollider : boxes)
//     for (Physics::CollisionComponentInterface<Box>* boxCollider : boxes)
//     {
//         if (!boxCollider->collider.isEnabled || boxCollider->collider.isOverlap)// || data.ignoredEntities.count(boxCollider.first) > 0) // TODO : ignored entities
//             continue;

//         // boxCollider->collider.worldCollider.transform = boxCollider->collider.transform->transformMatrixNode->worldData; 
//         //                                             // * physicComp.collider.transform->transformMatrixNode->worldData.getInverse();

//         boxCollider->collider.worldCollider.updateMatrixSizeFromMatrix();

//         if (Collisions::boxMovingShereCollision(boxCollider->collider.worldCollider, sphere, seg, segmentHit))
//         {
//             if (segmentHit.t <= hit.t)
//             {
//                 hit = segmentHit;

//                 hasCollided = true;
//                 collidedMeshInterface = boxCollider;
//             }
//         }
//     }

//     return hasCollided;
// }

// template<typename COLLISIONS_CALLBACKS>
// void Physics::PhysicsSystem::sphereFindOverlappingBoxes(const Sphere& sphere, 
//                                                       const Core::Maths::Vec3& lastLoc,
//                                                       const Physics::PhysicsSystem::PhysicsAdditionalData& data,
//                                                       const Entity::EntityID& physicCompID,
//                                                       COLLISIONS_CALLBACKS& callbacks)
// {
//     Segment3D seg{sphere.center, lastLoc};
//     if (seg.squaredLength() < 0.0001)
//         return;

//     SegmentHit hit;
    
//     // // TODO : opti with reserve
//     // std::vector<CollisionsCallbacksSentDataCpy> collisionsCallbacksSentDataList;
//     // collisionsCallbacksSentDataList.reserve(nextNbOverlapHint); 

//     for (std::pair<const Entity::EntityID, Physics::CollisionComponent<Box>>& boxCollider : boxes)
//     {
//         if (!boxCollider.second.isEnabled || !boxCollider.second.isOverlap || data.ignoredEntities.count(boxCollider.first) > 0)
//             continue;

//         boxCollider.second.worldCollider.updateMatrixSizeFromMatrix();

//         if (Collisions::boxMovingShereCollision(boxCollider.second.worldCollider, sphere, seg, hit))
//         {
//             // // Add Callbacks to list
//             // collisionsCallbacksSentDataList.emplace_back(hit, physicCompEntityID, boxCollider.first);
//             callbacks.onOverlap(CollisionsCallbacksSentData{hit, physicCompID, boxCollider.first});
//         }
//     }
// }

// template<typename COLLISIONS_CALLBACKS>
// inline Core::Maths::Vec3 Physics::PhysicsSystem::simulatePhysicsForASphere(const Sphere& sphere, 
//                                const Physics::PhysicsSystem::PhysicsAdditionalData& data, 
//                                std::map<Entity::EntityID, bool>& collidingEntities,
//                                Core::Maths::Vec3& leftVelocity,
//                                const Entity::EntityID& physicCompID,
//                                COLLISIONS_CALLBACKS& callbacks)
// {
//     if (leftVelocity.vectorSquareLength() < 0.00001f)
//     {
//         return sphere.center;
//     }

//     SegmentHit hit;
//     hit.t = 2.f;
//     // Entity::EntityID collidedEntity; // box
//     Physics::CollisionComponentInterface<Box>* collidedMeshInterface; // box

//     if (sphereCollisionWithBoxes(sphere, leftVelocity, data, hit, collidedMeshInterface))
//     {
//         std::pair<std::map<Entity::EntityID, bool>::iterator, bool> it = collidingEntities.emplace(collidedMeshInterface, true);
//         if (!it.second)
//         {
//             // Callback
//             CollisionsCallbacksSentData collisionsCallbacksSentData
//             {
//                 std::move(hit),
//                 physicCompID,
//                 collidedMeshInterface
//             };
//             callbacks.onCollisionEnter(collisionsCallbacksSentData);
//         }
         


//         Core::Maths::Vec3 newLoc = sphere.center + leftVelocity * hit.t;
//         Core::Maths::Vec3 newVelo = leftVelocity * hit.t;
//         Core::Maths::Vec3 velocityAfterContact = leftVelocity - newVelo;
//         float dot = Core::Maths::Vec3::dotProduct(hit.normal, velocityAfterContact);
//         Core::Maths::Vec3 finalLoc = sphere.center + velocityAfterContact - dot * hit.normal;// + 0.001f * hit.normal;
//         leftVelocity = (leftVelocity - dot * hit.normal);
//         leftVelocity *= 0.9f;

//         sphereFindOverlappingBoxes(sphere, finalLoc, data, physicCompID, callbacks);

        
//         // std::cout << "left : " << physicComp.second.velocity << std::endl;

//         // // We add a small value, because otherwises, they player would not be able to move due to constant collisions
//         // return finalLoc + hit.normal * minimalDistToGround;

//         Sphere s;
//         s.center = finalLoc;
//         s.radius= sphere.radius;

//         return simulatePhysicsForASphere(sphere, data, collidingEntities, leftVelocity, physicCompID, callbacks);
//     }
//     else 
//     {
//         Core::Maths::Vec3 finalLoc = sphere.center + leftVelocity; 
//         sphereFindOverlappingBoxes(sphere, finalLoc, data, physicCompID, callbacks);
//         return finalLoc;
//     }
// }

// template<typename COLLISIONS_CALLBACKS>
// void Physics::PhysicsSystem::simulatePhysics(std::pair<const Entity::EntityID, Physics::PhysicComponent>& physicComp, 
//                                                        const Physics::PhysicsSystem::PhysicsAdditionalData& data,
//                                                        const Core::Maths::Vec3& leftVelocity,
//                                                        COLLISIONS_CALLBACKS& callbacks)
// {
//     if (leftVelocity.vectorSquareLength() < 0.001f)
//         return;

//     // std::cout << leftVelocity << std::endl;
//     SegmentHit hit;
//     hit.t = 2.f;
//     Entity::EntityID collidedEntity; // box

//     if (staticBoxesFirstCollision(physicComp.second, physicComp.second.collider.transform->transform.location, hit, data, collidedEntity, leftVelocity))
//     {
//         Core::Maths::Vec3 newLoc = physicComp.second.collider.transform->transform.location + leftVelocity * hit.t;
//         // std::cout << "Old : " << physicComp.second.collider.transform->transform.location << '\t' << " New : " << newLoc << std::endl; 
//         // std::cout << "Hit.t : " << hit.t << std::endl;
//         staticBoxesOverlapCollision(physicComp.second, 
//                                     physicComp.second.collider.transform->transform.location, 
//                                     newLoc, 
//                                     data, 
//                                     physicComp.first, 
//                                     callbacks);

//         physicComp.second.collider.transform->transform.location = newLoc;

//         if (!physicComp.second.collider.isColliding)
//         {
//             physicComp.second.collider.isColliding = true;

//             // Callback
//             CollisionsCallbacksSentData collisionsCallbacksSentData
//             {
//                 std::move(hit),
//                 physicComp.first,
//                 collidedEntity
//             };
//             callbacks.onCollisionEnter(collisionsCallbacksSentData);
//         }

//         Core::Maths::Vec3 newVelo = leftVelocity * hit.t;

//         Core::Maths::Vec3 velocityAfterContact = leftVelocity - newVelo;

//         // hit.normal = hit.normal.unitVector();
//         float dot = Core::Maths::Vec3::dotProduct(hit.normal, velocityAfterContact);

//         Core::Maths::Vec3 finalLoc = physicComp.second.collider.transform->transform.location + velocityAfterContact - dot * hit.normal + 0.001f * hit.normal;
//         physicComp.second.velocity = (leftVelocity - dot * hit.normal);
//         // std::cout << "left : " << physicComp.second.velocity << std::endl;

//         // // We add a small value, because otherwises, they player would not be able to move due to constant collisions
//         // return finalLoc + hit.normal * minimalDistToGround;
//         simulatePhysics(physicComp, data, physicComp.second.velocity, callbacks);
//     }
//     else 
//     {
//         if (physicComp.second.collider.isColliding)
//         {
//             physicComp.second.collider.isColliding = false;

//             // Callback
//             callbacks.onCollisionExit(physicComp.first);
//         }

//         physicComp.second.collider.transform->transform.location = physicComp.second.collider.transform->transform.location + leftVelocity;
//     }
// }

// // template<typename COLLISIONS_CALLBACKS>
// // void Physics::PhysicsSystem::simulatePhysics(std::pair<const Entity::EntityID, Physics::PhysicComponent>& physicComp, 
// //                                                        const Physics::PhysicsSystem::PhysicsAdditionalData& data,
// //                                                        COLLISIONS_CALLBACKS& callbacks)
// // {
// //     if (!physicComp.second.isEnabled)
// //         return;

// //     SegmentHit hit;
// //     hit.t = 2.f;

// //     Entity::EntityID collidedEntity; // box

// //     if (staticBoxesFirstCollision(physicComp.second, physicComp.second.collider.transform->transform.location, hit, data, collidedEntity))
// //     {
// //         Core::Maths::Vec3 newLoc = collisionPhysicalResponse(physicComp.second, physicComp.second.collider.transform->transform.location, hit);

// //         staticBoxesOverlapCollision(physicComp.second, physicComp.second.collider.transform->transform.location, newLoc, data, physicComp.first, callbacks);

// //         if (!physicComp.second.collider.isColliding)
// //         {
// //             physicComp.second.collider.isColliding = true;

// //             // Callback
// //             CollisionsCallbacksSentData collisionsCallbacksSentData
// //             {
// //                 std::move(hit),
// //                 physicComp.first,
// //                 collidedEntity
// //             };
// //             callbacks.onCollisionEnter(collisionsCallbacksSentData);
// //         }

// //         physicComp.second.collider.transform->transform.location = newLoc;
// //         return;
// //     }
    
// //     // is not colliding
// //     else if (physicComp.second.collider.isColliding)
// //     {
// //         physicComp.second.collider.isColliding = false;

// //         // Callback
// //         callbacks.onCollisionExit(physicComp.first);
// //     }

// //     staticBoxesOverlapCollision(physicComp.second, physicComp.second.collider.transform->transform.location, 
// //                                 physicComp.second.velocity + physicComp.second.collider.transform->transform.location, 
// //                                 data, 
// //                                 physicComp.first, 
// //                                 callbacks);

// //     physicComp.second.collider.transform->transform.location = physicComp.second.collider.transform->transform.location + physicComp.second.velocity;
// //     return;
// // }

// template<typename COLLISIONS_CALLBACKS>
// void Physics::PhysicsSystem::staticBoxesOverlapCollision(Physics::PhysicComponent& physicComp, 
//                                                          const Core::Maths::Vec3& startLoc, 
//                                                          const Core::Maths::Vec3& endLoc, 
//                                                          const PhysicsAdditionalData& data,
//                                                          const Entity::EntityID& physicCompEntityID,
//                                                          COLLISIONS_CALLBACKS& callbacks)
// {   
//     Segment3D seg{startLoc, endLoc};
//     if (seg.squaredLength() < 0.0001)
//         return;
//     SegmentHit hit;
    
//     // // TODO : opti with reserve
//     // std::vector<CollisionsCallbacksSentDataCpy> collisionsCallbacksSentDataList;
//     // collisionsCallbacksSentDataList.reserve(nextNbOverlapHint); 

//     for (std::pair<const Entity::EntityID, Physics::CollisionComponent<Box>>& boxCollider : boxes)
//     {
//         if (!boxCollider.second.isEnabled || !boxCollider.second.isOverlap || data.ignoredEntities.count(boxCollider.first) > 0)
//             continue;

//         boxCollider.second.worldCollider.updateMatrixSizeFromMatrix();

//         if (Collisions::boxMovingShereCollision(boxCollider.second.worldCollider, physicComp.collider.worldCollider, seg, hit))
//         {
//             // // Add Callbacks to list
//             // collisionsCallbacksSentDataList.emplace_back(hit, physicCompEntityID, boxCollider.first);
//             callbacks.onOverlap(CollisionsCallbacksSentData{hit, physicCompEntityID, boxCollider.first});
//         }
//     }
// }