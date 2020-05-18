#ifndef _COLLISIONS_HPP_
#define _COLLISIONS_HPP_

#include "plane.hpp"
#include "shapes.hpp"
#include "vec3.hpp"

#include <limits>

class Collisions
{
public:
    static bool planeSegmentCollision(const Plane& plane, const Segment3D& segment, SegmentHit& hit)
    {
        const Core::Maths::Vec3 lineDir  = segment.p2 - segment.p1;

        const float dotProd = Core::Maths::Vec3::dotProduct(plane.normal, lineDir);

        // if colinear, return false,
        // since the segment is either already inside, either outside of the plane.
        if (dotProd == 0)
            return false;

        hit.t                   = (plane.distance - Core::Maths::Vec3::dotProduct(plane.normal, segment.p1)) / dotProd;
        // We could use a branch (if) to prevent the computation of the collisionPoint and the normal.
        // However, branches are expensives, and we can get extra data (i.e. about the line) by not doing it.
        hit.collisionPoint      = segment.p1 + hit.t * lineDir;
        hit.normal              = dotProd < 0 ? plane.normal : - plane.normal;

        return (hit.t >= 0.f && hit.t <= 1.f);
    }

private:
    // return true if the projection of the vectors is between 0 and 1.
    static bool isProjectionValid(const Core::Maths::Vec3& v1, const Core::Maths::Vec3& v2)
    {
        float proj = Core::Maths::Vec3::dotProduct(v1, v2);
        return (proj >= 0 && proj <= v2.vectorSquareLength());
    }

public:
    static bool rectangleSegmentCollision(const Rectangle& rect, const Segment3D& segment, SegmentHit& hit)
    {
        Plane plane(rect.point, rect.normal);

        if (!planeSegmentCollision(plane, segment, hit)) // if not in plane
            return false;

        Core::Maths::Vec3 rectangleToPointVec = (hit.collisionPoint - rect.point);

        // if (rectangleToPointVec.vectorSquareLength() == 0)
        //     return true;
        
        // rectangleToPointVec = rectangleToPointVec.unitVector();

        return (isProjectionValid(rectangleToPointVec, rect.seg1) && isProjectionValid(rectangleToPointVec, rect.seg2));
    }


    // TESTED
    static bool centeredSphereSegmentCollision(const CenteredSphere& sphere, const Segment3D& segment, SegmentHit& hit)
    {
        const Core::Maths::Vec3 circleCenterToSeg1 = segment.p1;
        const Core::Maths::Vec3 segDir = segment.p2 - segment.p1;

        const float a = segDir.vectorSquareLength();
        const float b = 2 * Core::Maths::Vec3::dotProduct(circleCenterToSeg1, segDir); 
        const float c = circleCenterToSeg1.vectorSquareLength() - sphere.radius * sphere.radius;

        float delta = b * b - 4 * a * c;    

        if (delta < 0)
        {
            return false; // no collision line / sphere
        }
        // We could put an if statement to test if delta == 0,
        // and it would increase the performance when that is the case.
        // However, actually, that would be very rare in case of collisions.
        // That is why, by not adding the if statement, 
        // we do not decrease overall optimization. 

        // We know there is no collision if (0 < t < 1).
        // t = (- b - sqrt(delta)) / (2 * a) 

        // The other solution of the equation is 
        // hit.t = (- b + sqrt(delta)) / (2 * a);
        // However, we know that this value would be higher than the other,
        // and we want the smaller value.
        // The exception is when seg.p1 is inside the sphere.
        // That would mean we want the second value.

        if (sphere.isInside(segment.p1))
            hit.t = (- b + sqrt(delta)) / (2 * a);
        else 
            hit.t = (- b - sqrt(delta)) / (2 * a);

        if (hit.t < 0 || hit.t > 1)
        {
            return false;
        }

        hit.collisionPoint = segment.p1 + hit.t * segDir;
        hit.normal = hit.collisionPoint.unitVector();

        return true; 
    }

    // TESTED
    static bool sphereSegmentCollision(const Sphere& sphere, const Segment3D& segment, SegmentHit& hit)
    {
        const Core::Maths::Vec3 circleCenterToSeg1 = segment.p1 - sphere.center;
        const Core::Maths::Vec3 segDir = segment.p2 - segment.p1;

        const float a = segDir.vectorSquareLength();
        const float b = 2 * Core::Maths::Vec3::dotProduct(circleCenterToSeg1, segDir); 
        const float c = circleCenterToSeg1.vectorSquareLength() - sphere.radius * sphere.radius;

        float delta = b * b - 4 * a * c;    

        if (delta < 0)
        {
            return false; // no collision line / sphere
        }
        // We could put an if statement to test if delta == 0,
        // and it would increase the performance when that is the case.
        // However, actually, that would be very rare in case of collisions.
        // That is why, by not adding the if statement, 
        // we do not decrease overall optimization. 

        // We know there is no collision if (0 < t < 1).
        // t = (- b - sqrt(delta)) / (2 * a) 

        // The other solution of the equation is 
        // hit.t = (- b + sqrt(delta)) / (2 * a);
        // However, we know that this value would be higher than the other,
        // and we want the smaller value.
        // The exception is when seg.p1 is inside the sphere.
        // That would mean we want the second value.

        if (sphere.isInside(segment.p1))
            hit.t = (- b + sqrt(delta)) / (2 * a);
        else 
            hit.t = (- b - sqrt(delta)) / (2 * a);

        if (hit.t < 0 || hit.t > 1)
        {
            return false;
        }

        hit.collisionPoint = segment.p1 + hit.t * segDir;
        hit.normal = (hit.collisionPoint - sphere.center).unitVector();

        return true; 
    }


    static bool infiniteCylinderSegmentCollision(const Cylinder& cylinder, const Segment3D& segment, SegmentHit& hit)
    {
        const Core::Maths::Vec3 cross1 = Core::Maths::Vec3::crossProduct(segment.p1 - cylinder.location, cylinder.dirInWorldLoc - cylinder.location);
        const Core::Maths::Vec3 cross2 = Core::Maths::Vec3::crossProduct(segment.p2 - segment.p1, cylinder.dirInWorldLoc - cylinder.location);

        const float s = (cylinder.dirInWorldLoc - cylinder.location).vectorSquareLength();

        if (s == 0.f || cross2.vectorSquareLength() == 0.f)
            return false;

        const float c = cross1.vectorSquareLength() / s - cylinder.radius * cylinder.radius;
        const float b = 2 * Core::Maths::Vec3::dotProduct(cross1, cross2) / s;
        const float a = cross2.vectorSquareLength() / s;

        const float delta = b * b - 4 * a * c;
        if (delta < 0.f)
            return false;

        const float result1 = (- b + sqrt(delta)) / (2.f * a);
        const float result2 = (- b - sqrt(delta)) / (2.f * a);

        hit.t = result2;
        if (result1 >= 0 && result1 <= result2)
        {
            hit.t = result1;
        }
        hit.collisionPoint = segment.p1 + hit.t * (segment.p2 - segment.p1);
        
        // we know that (cylinder.dirInWorldLoc - cylinder.location) length is not 0 from before
        Core::Maths::Vec3 u = (cylinder.dirInWorldLoc - cylinder.location).unitVector();

        hit.normal = hit.collisionPoint - (u * Core::Maths::Vec3::dotProduct(hit.collisionPoint - cylinder.location, u) + cylinder.location);
        
        if (hit.normal.vectorSquareLength() == 0.f)
            return false;
        hit.normal = hit.normal.unitVector();
        
        return (hit.t >= 0 && hit.t <= 1);
    }

    // TODO : Test planes
    static bool cylinderSegmentCollision(const Cylinder& cylinder, const Segment3D& seg, SegmentHit& hit)
    {
        Plane plane1 {cylinder.location, {(cylinder.dirInWorldLoc - cylinder.location).unitVector()}};
        bool isSphereCollision1 = planeSegmentCollision(plane1, seg, hit);

        if (isSphereCollision1 && (hit.collisionPoint - cylinder.location).vectorSquareLength() <= cylinder.radius * cylinder.radius)
        {
            return true;
        }

        Plane plane2 {cylinder.dirInWorldLoc, {(cylinder.location - cylinder.dirInWorldLoc).unitVector()}};
        bool isSphereCollision2 = planeSegmentCollision(plane2, seg, hit);

        if (isSphereCollision2 && (hit.collisionPoint - cylinder.location).vectorSquareLength() <= cylinder.radius * cylinder.radius)
        {
            return true;
        }
        
        if (infiniteCylinderSegmentCollision(cylinder, seg, hit))
        {
            float p = Core::Maths::Vec3::dotProduct(hit.collisionPoint - cylinder.location, (cylinder.dirInWorldLoc - cylinder.location).unitVector());

            return p >= 0 && p <= (cylinder.location - cylinder.dirInWorldLoc).vectorLength();
        }

        return false;
    }

    // TODO : TO TEST
    static float squaredDistPointSegment(const Core::Maths::Vec3& point, const Segment3D& seg)
    {
        float proj = Core::Maths::Vec3::dotProduct(point - seg.p1, seg.p2 - seg.p1);

        if (proj <= 0)
        {
            return (point - seg.p1).vectorSquareLength();
        }
        else if (proj >= 1)
        {
            return (point - seg.p2).vectorSquareLength();
        }
        else 
        {
            return Core::Maths::Vec3::crossProduct(point - seg.p1, seg.p2 - seg.p1).vectorSquareLength() / (seg.p2 - seg.p1).vectorSquareLength();
        }
    }

    static bool emptyCylinderSegmentCollision(const Cylinder& cylinder, const Segment3D& seg, SegmentHit& hit)
    {
        if (infiniteCylinderSegmentCollision(cylinder, seg, hit))
        {
            // Is inside the finite cylinder : 
            float p = Core::Maths::Vec3::dotProduct(hit.collisionPoint - cylinder.location, (cylinder.dirInWorldLoc - cylinder.location).unitVector());
            return p >= 0 && p <= (cylinder.location - cylinder.dirInWorldLoc).vectorLength();
        }
        else 
            return false;
    }

    static bool capsuleSegmentCollision(const Cylinder& capsule, const Segment3D& seg, SegmentHit& hit)
    {   
        bool hasCollided = false;

        const float dotp1 = Core::Maths::Vec3::dotProduct(seg.p1 - capsule.location, capsule.dirInWorldLoc - capsule.location); 
        const float dotp2 = Core::Maths::Vec3::dotProduct(seg.p1 - capsule.location, capsule.dirInWorldLoc - capsule.location);

        const float squareCylSeg = (capsule.dirInWorldLoc - capsule.location).vectorSquareLength();
        const float squareRadius = capsule.radius * capsule.radius;

        // otherwise, we should use the sqrt function
        const float dotp1FromOtherSide = Core::Maths::Vec3::dotProduct(seg.p1 - capsule.dirInWorldLoc, capsule.location - capsule.dirInWorldLoc); 
        const float dotp2FromOtherSide = Core::Maths::Vec3::dotProduct(seg.p1 - capsule.dirInWorldLoc, capsule.location - capsule.dirInWorldLoc);

        // If the projections are "upper" than the cylinder or if they are "lower", then there is no collision.
        if ((dotp1 < - squareRadius && dotp2 < - squareRadius) 
        || (dotp1FromOtherSide < - squareRadius && dotp2FromOtherSide < - squareRadius))
            return false; 

        // If p1 is in the side of cylinder.location and p2 is in another side,
        // or if p2 is in the side of cylinder.location:
        if (dotp1 <= 0 || dotp2 <= 0)
        {
            SegmentHit tempHit;
            Sphere sphere2 {capsule.location, capsule.radius};
            bool isSphereCollision2 = sphereSegmentCollision(sphere2, seg, hit);

            // Is colliding with circle
            if (isSphereCollision2 && Core::Maths::Vec3::dotProduct((capsule.dirInWorldLoc - capsule.location).unitVector(), hit.collisionPoint - capsule.location) < 0)
            {
                hasCollided = true;
                hit = tempHit;
            }
        }

        // Same than before, but with the different side
        if (dotp1 >= squareCylSeg || dotp2 >= squareCylSeg)
        {
            SegmentHit tempHit;
            Sphere sphere1 {capsule.dirInWorldLoc, capsule.radius};
            bool isSphereCollision1 = sphereSegmentCollision(sphere1, seg, hit);

            // Is colliding with circle
            if (isSphereCollision1 && Core::Maths::Vec3::dotProduct((capsule.dirInWorldLoc - capsule.location).unitVector(), hit.collisionPoint - capsule.location) > 1)
            {
                if (hasCollided)
                {
                    if (tempHit.t < hit.t)
                    {
                        hit = tempHit;
                    }
                }
                else 
                {
                    hasCollided = true;
                    hit = tempHit;
                }
            }
        }

        // If segment between the two spheres, then test collision.
        if (!((dotp1 < 0.f && dotp2 < 0.f) 
        || (dotp1 > squareCylSeg && dotp2 > squareCylSeg)))
        {
            SegmentHit tempHit;
            if (emptyCylinderSegmentCollision(capsule, seg, tempHit))
            {
                if (hasCollided)
                {
                    if (tempHit.t < hit.t)
                    {
                        hit = tempHit;
                    }
                }
                else 
                {
                    hasCollided = true;
                    hit = tempHit;
                }
            }
        }

        return hasCollided;
    }

    static inline bool areHigherThan(float p1, float p2, float f)
    {
        return (p1 > f && p2 > f);
    }

    static inline bool areLessThan(float p1, float p2, float f)
    {
        return (p1 < f && p2 < f);
    }

    static bool centeredAABBSegmentCollision(const CenteredAABB& aabb, const Segment3D& seg, SegmentHit& hit)
    {
        constexpr float epsiLocal = 0.5f;
        if (   areLessThan(seg.p1.x, seg.p2.x, - aabb.size.x - epsiLocal)
            || areHigherThan(seg.p1.x, seg.p2.x, aabb.size.x + epsiLocal)
            || areLessThan(seg.p1.y, seg.p2.y, - aabb.size.y - epsiLocal)
            || areHigherThan(seg.p1.y, seg.p2.y, aabb.size.y + epsiLocal)
            || areLessThan(seg.p1.z, seg.p2.z, - aabb.size.z - epsiLocal)
            || areHigherThan(seg.p1.z, seg.p2.z, aabb.size.z + epsiLocal))
            return false;

        // Set collision flag to false.
        bool hasCollided = false;

        // We use hit.t later on to know if the new t found is valid.
        // However, for the first time looping, we have no value to compare with,
        // but we also know it must be less thn 1.
        // By doing this assignment, we prevent some weird exception
        hit.t = 1.f;

        // for each axi
        for (unsigned int axisID = 0; axisID < Core::Maths::Vec3::getAxisNumber(); ++axisID)
        {
            float boundingValue = aabb.size[axisID];
            float isNegative = true;

            // for negative and positive bounding case
            for (unsigned char i = 0; i < 2; ++i)
            {
                // is not parallel to axis
                if (seg.p1[axisID] != seg.p2[axisID])
                {
                    // Computes new t
                    float newHitT = (boundingValue - seg.p1[axisID]) / (seg.p2[axisID] - seg.p1[axisID]);

                    // If plane is colliding with the segment
                    if (newHitT >= 0 && newHitT <= hit.t) // hit.t is initalized with 1 on the first try, 
                                                        // so hit.t will always be less or equal to 1
                    {
                        Core::Maths::Vec3 hitPoint = seg.p1 + newHitT * (seg.p2 - seg.p1);
                        
                        // computes indicies of other axis
                        const unsigned int axisIDn1 = (axisID + 1) % 3;
                        const unsigned int axisIDn2 = (axisID + 2) % 3;

                        // is collision inside a square
                        if (hitPoint[axisIDn1] * hitPoint[axisIDn1] <= aabb.size[axisIDn1] * aabb.size[axisIDn1]
                            && hitPoint[axisIDn2] * hitPoint[axisIDn2] <= aabb.size[axisIDn2] * aabb.size[axisIDn2]) 
                        {
                            hit.t = newHitT;
                            // TODO : add static Core::Maths::Vec3 getAxis(unsigned int id) in Core::Maths::Vec3 class ? 

                            hit.normal = Core::Maths::Vec3::getAxis(axisID);

                            //== Remove the following if the segment should not start inside the box ==/
                            // Since it would be useless in that case, removing it would increase performance.
                            if (!isNegative)
                            {
                                if (seg.p1[axisID] < boundingValue)
                                    hit.normal *= -1.f;
                            }
                            else if (seg.p1[axisID] > boundingValue)
                                hit.normal *= -1.f;
                            //===========================================================================/

                            if (isNegative)
                                hit.normal *= -1.f;

                            hit.collisionPoint = hitPoint;

                            // Set collision flag, so we know there is at least one collision
                            // However, we want to compute SegmentHit, so we can't stop the function here.
                            // We have to know which plane is colliding.
                            hasCollided = true;
                        }
                    }
                } 
                boundingValue *= -1;    
                isNegative = false;
            }
        }        

        return hasCollided;
    }

    // TODO : to verify
    static bool aabbSegmentCollision(const AABB& aabb, const Segment3D& seg, SegmentHit& hit)
    {
        if (centeredAABBSegmentCollision(aabb.centeredAABB, 
                                        Segment3D{seg.p1 - aabb.location, seg.p2 - aabb.location}, 
                                        hit))
        {
            hit.collisionPoint += aabb.location;
            return true;
        }
        else 
            return false;
    }

    // TODO : to verify
    static bool boxSegmentCollision(const Box& box, const Segment3D& seg, SegmentHit& hit)
    {
        Segment3D transSeg = Segment3D{Core::Maths::Matrix4x4{box.transform.getInverse()} * Core::Maths::Vec4{seg.p1, 1}, 
                                    Core::Maths::Matrix4x4{box.transform.getInverse()} * Core::Maths::Vec4{seg.p2, 1}};

        if (centeredAABBSegmentCollision(box.aabb, transSeg, hit))
        {
            // TODO : opti by just translating the point
            hit.collisionPoint = box.transform * Core::Maths::Vec4{hit.collisionPoint, 1.f};
            hit.normal = box.transform * Core::Maths::Vec4{hit.normal, 0.f};
            return true;
        }
        else 
            return false;
    }

    // TODO : to verify
    static bool boxSegmentCollision(const Box& box, const Segment3D& seg, SegmentHit& hit, SegmentHit& localHit, Segment3D& localSeg)
    {
        localSeg = Segment3D{Core::Maths::Matrix4x4{box.transform.getInverse()} * Core::Maths::Vec4{seg.p1, 1}, 
                                Core::Maths::Matrix4x4{box.transform.getInverse()} * Core::Maths::Vec4{seg.p2, 1}};

        if (centeredAABBSegmentCollision(box.aabb, localSeg, hit))
        {
            localHit = hit;
            // TODO : opti by just translating the point
            hit.collisionPoint = box.transform * Core::Maths::Vec4{hit.collisionPoint, 1.f};
            hit.normal = box.transform * Core::Maths::Vec4{hit.normal, 0.f};
            return true;
        }
        else 
            return false;
    }

    template<class T>
    static constexpr const T& clamp( const T& v, const T& lo, const T& hi )
    {
        assert( !(hi < lo) );
        return (v < lo) ? lo : (hi < v) ? hi : v;
    }

    static bool centeredAABBSphereCollision(const CenteredAABB& centeredAABB, const Sphere& sphere)
    {
        Core::Maths::Vec3 nearestPoint 
        {
            clamp(sphere.center.x, - centeredAABB.size.x, centeredAABB.size.x),
            clamp(sphere.center.y, - centeredAABB.size.y, centeredAABB.size.y),
            clamp(sphere.center.z, - centeredAABB.size.z, centeredAABB.size.z)
        };

        return (nearestPoint - sphere.center).vectorSquareLength() < sphere.radius * sphere.radius;
    }

    // Works only if the box has no scale in its transform matrix
    static bool boxSphereCollision(const Box& box, const Sphere& sphere)
    {
        Core::Maths::Vec3 newSphereCenter = (Core::Maths::Matrix4x4(box.transform.getInverse()) * Core::Maths::Vec4(sphere.center, 1));
        if (   std::abs(newSphereCenter.x) > box.aabb.size.x + sphere.radius
            && std::abs(newSphereCenter.y) > box.aabb.size.y + sphere.radius
            && std::abs(newSphereCenter.z) > box.aabb.size.z + sphere.radius)
        {
            return false;
        }   

        return centeredAABBSphereCollision(std::move(box.aabb), {newSphereCenter, sphere.radius});
    }

    static bool aabbAabbCollision(const AABB& aabb1, const AABB& aabb2)
    {
        // Compares the range for each dimension ;
        return aabb1.location.x - aabb1.centeredAABB.size.x < aabb2.location.x + aabb2.centeredAABB.size.x 
            && aabb1.location.x + aabb1.centeredAABB.size.x > aabb2.location.x - aabb2.centeredAABB.size.x 

            && aabb1.location.y - aabb1.centeredAABB.size.y < aabb2.location.y + aabb2.centeredAABB.size.y 
            && aabb1.location.y + aabb1.centeredAABB.size.y > aabb2.location.y - aabb2.centeredAABB.size.y 

            && aabb1.location.z - aabb1.centeredAABB.size.z < aabb2.location.z + aabb2.centeredAABB.size.z 
            && aabb1.location.z + aabb1.centeredAABB.size.z > aabb2.location.z - aabb2.centeredAABB.size.z;
    }

    static bool boxBoxCollision(const Box& box1, const Box& box2)
    {  
        // if (aabbAabbCollision(box1, box2))
        {
            Core::Maths::Vec3 axisArray[] = 
            {
                box1.transform.getXAxis(), box1.transform.getYAxis(), box1.transform.getZAxis(), 
                box2.transform.getXAxis(), box2.transform.getYAxis(), box2.transform.getZAxis(), 
                
                Core::Maths::Vec3::crossProduct(box1.transform.getXAxis(), box2.transform.getXAxis()),
                Core::Maths::Vec3::crossProduct(box1.transform.getXAxis(), box2.transform.getYAxis()),
                Core::Maths::Vec3::crossProduct(box1.transform.getXAxis(), box2.transform.getZAxis()),

                Core::Maths::Vec3::crossProduct(box1.transform.getYAxis(), box2.transform.getXAxis()),
                Core::Maths::Vec3::crossProduct(box1.transform.getYAxis(), box2.transform.getYAxis()),
                Core::Maths::Vec3::crossProduct(box1.transform.getYAxis(), box2.transform.getZAxis()),

                Core::Maths::Vec3::crossProduct(box1.transform.getZAxis(), box2.transform.getXAxis()),
                Core::Maths::Vec3::crossProduct(box1.transform.getZAxis(), box2.transform.getYAxis()),
                Core::Maths::Vec3::crossProduct(box1.transform.getZAxis(), box2.transform.getZAxis()),
            };

            // Compute before loop to prevent too much computation
            const std::array<Core::Maths::Vec3, Box::nbPoints> box1Points = box1.getPoints();
            const std::array<Core::Maths::Vec3, Box::nbPoints> box2Points = box2.getPoints();

            for (Core::Maths::Vec3& axis : axisArray)
            {   
                Range2D projection1 = Box::projectOnAxis(axis, box1Points); 
                Range2D projection2 = Box::projectOnAxis(axis, box2Points); 

                if (!Range2D::isIntersecting(projection1, projection2))
                {
                    return false;
                }
            }

            return true;
        }
        // else 
        //     return false;
    }
    
    static unsigned int pointBoxRelativeLocationData(const Core::Maths::Vec3& p, const CenteredAABB& aabb, std::array<int, 3>& areAxisPositive)
    {
        unsigned int nbOutsideCollisions = 0;
        for (unsigned int i = 0; i < 3; i++)
        {
            if (p[i] > aabb.size[i])
            {
                areAxisPositive[i] = 1;
                nbOutsideCollisions++;
            }

            else if (p[i] < - aabb.size[i])
            {
                areAxisPositive[i] = -1;
                nbOutsideCollisions++;
            }

            else 
                areAxisPositive[i] = 0;
        }
        return nbOutsideCollisions;
    }

    static bool centeredAABBMovingSphereCollision(CenteredAABB aabb, const Sphere& sphere, const Segment3D& seg, SegmentHit& hit)
    {
        constexpr float epsiLocal = 0.01f;
        if (   areLessThan(  seg.p1.x, seg.p2.x, - aabb.size.x - sphere.radius - epsiLocal)
            || areLessThan(  seg.p1.y, seg.p2.y, - aabb.size.y - sphere.radius - epsiLocal)
            || areLessThan(  seg.p1.z, seg.p2.z, - aabb.size.z - sphere.radius - epsiLocal)
            || areHigherThan(seg.p1.y, seg.p2.y,   aabb.size.y + sphere.radius + epsiLocal)
            || areHigherThan(seg.p1.z, seg.p2.z,   aabb.size.z + sphere.radius + epsiLocal)
            || areHigherThan(seg.p1.y, seg.p2.y,   aabb.size.y + sphere.radius + epsiLocal))
            return false;

        SegmentHit testHit;

        // We want to call functions callable only for this function.
        // We can't define a function in a function without declaring a class.
        // This way, we prevent polluting the code with too much uncallable functions.
        class SubCollisions
        {
        public:
            // When this function is called, with valid arguments,
            // the function will test the collision between a segment 
            // and the nearest sphere corresponding to a box "vertex". 
            // At the end, the distance between testedPoint and the sphere is minimal compared to other possible spheres.
            static bool tryCollisionsForNearestSphere(const Core::Maths::Vec3& testedPoint, CenteredAABB aabb, const Sphere& sphere, const Segment3D& seg, SegmentHit& hit)
            {
                // Prevents other computations if the segment goes on a sphere
                Sphere s;
                s.radius = sphere.radius;
                // We know that testHit.collisionPoint[i] > aabb.size[i] or testHit.collisionPoint[i] < - aabb.size[i], so :
                for (unsigned int i = 0; i < Core::Maths::Vec3::getAxisNumber(); i++)
                    s.center[i] = clamp(testedPoint[i], - aabb.size[i], aabb.size[i]);

                return (sphereSegmentCollision(s, seg, hit));
            }

            // When this function is called, with valid arguments,
            // the function will test the collision between a segment 
            // and the nearest cylinder corresponding to a box "edge". 
            // At the end, the distance between testedPoint and the cylinder is minimal compared to other possible cylinders.
            static bool tryCollisionsForNearestCylinder(const Core::Maths::Vec3& testedPoint, CenteredAABB aabb, const Sphere& sphere, const Segment3D& seg, SegmentHit& hit)
            {
                Cylinder cyl;
                cyl.radius = sphere.radius;
                for (unsigned int i = 0; i < Core::Maths::Vec3::getAxisNumber(); i++)
                {
                    if (std::abs(testedPoint[i]) >= aabb.size[i])
                    {
                        cyl.location[i] = cyl.dirInWorldLoc[i] = clamp(testedPoint[i], - aabb.size[i], aabb.size[i]);
                    }
                    else 
                    {
                        cyl.location[i] = - aabb.size[i];
                        cyl.dirInWorldLoc[i] = aabb.size[i];
                    }
                }

                return (cylinderSegmentCollision(cyl, seg, hit));
            }
        };

        // To be sure the player is inside the box (that is if abs(seg.p1) < aabb.size - sphere.radius),
        // or outside (abs(seg.p1) > aabb.size + sphere.radius / 2)
        bool isInside = aabb.isPointInside(seg.p1, 0);
        aabb.size += sphere.radius;
        if (isInside)
        {
            aabb.size -= 2*sphere.radius;
        }

        if (centeredAABBSegmentCollision(aabb, seg, testHit))
        {
            if (isInside)
                aabb.size += 2*sphere.radius;
            aabb.size -= sphere.radius;

            unsigned int n = 0;
            for (unsigned int i = 0; i < Core::Maths::Vec3::getAxisNumber(); i++)
                if (std::abs(testHit.collisionPoint[i]) >= aabb.size[i])
                    n++;

            switch (n)
            {
            case 0:
            case 1:
                hit = testHit;
                return true;
            case 2: // test with cylinders
                return (SubCollisions::tryCollisionsForNearestCylinder(testHit.collisionPoint, aabb, sphere, seg, hit));
                break;
            case 3:
                return (SubCollisions::tryCollisionsForNearestSphere(testHit.collisionPoint, aabb, sphere, seg, hit));
                break;
            }
        }

        else 
        {
            if (isInside)
                aabb.size += 2*sphere.radius;
            aabb.size -= sphere.radius;

            // Counter for detecting if it is inside the OOBB
            unsigned int m = 0;
            for (unsigned int i = 0; i < Core::Maths::Vec3::getAxisNumber(); i++)
                if (std::abs(seg.p1[i]) <= aabb.size[i] + sphere.radius)
                    m++;
                
            // if is not inside OOBB
            if (m < 3)
                return false;

            unsigned int n = 0;
            for (unsigned int i = 0; i < Core::Maths::Vec3::getAxisNumber(); i++)
                if (std::abs(seg.p1[i]) >= aabb.size[i])
                    n++;

            switch (n)
            {
            // if the cube's plane is cutting the sphere in two, 
            // let's say there is no collisions, 
            // because we can't compute normal and collision point.
            case 0:
            case 1:
                return false;
                break;
            case 2: // test with cylinders
                return SubCollisions::tryCollisionsForNearestCylinder(seg.p1, aabb, sphere, seg, hit);
                break;
            case 3:
                return SubCollisions::tryCollisionsForNearestSphere(seg.p1, aabb, sphere, seg, hit);
                break;
            }
        }

        // shouldn't be executed, removes errors
        return true;
    }

    static bool boxMovingShereCollision(const Box& box, const Sphere& sphere, const Segment3D& seg, SegmentHit& hit)
    {
        Sphere s;
        // s.center = // we won't use this value, so we won't have to set it
        s.radius = sphere.radius;
        Segment3D seg2;

        Core::Maths::Matrix4x4 m = box.transform;
        m.normalizeScale();
        Core::Maths::Matrix4x4 mInverse = m.getInverse();

        seg2.p1 = mInverse * seg.p1;
        seg2.p2 = mInverse * seg.p2;

        if (centeredAABBMovingSphereCollision(box.transform.getScale(), s, seg2, hit))
        {
            hit.collisionPoint = m * Core::Maths::Vec4(hit.collisionPoint, 1);
            hit.normal         = m * Core::Maths::Vec4(hit.normal, 0);
            hit.normal         = hit.normal.unitVector();
            return true;
        }
        else 
            return false;
    }
};

#endif