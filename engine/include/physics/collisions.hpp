#ifndef _COLLISIONS_HPP_
#define _COLLISIONS_HPP_

#include "plane.hpp"
#include "shapes.hpp"
#include "vec3.hpp"
#include "sphere.hpp"
#include "segment3D.hpp"

#include <limits>

class Collisions
{
public:
    static bool planeSegmentCollision(const Plane& plane, const Segment3D& segment, SegmentHit& hit);

    static bool rectangleSegmentCollision(const Rectangle& rect, const Segment3D& segment, SegmentHit& hit);

    static bool centeredSphereSegmentCollision(const CenteredSphere& sphere, const Segment3D& segment, SegmentHit& hit);

    static bool sphereSegmentCollision(const Sphere& sphere, const Segment3D& segment, SegmentHit& hit);

    static bool infiniteCylinderSegmentCollision(const Cylinder& cylinder, const Segment3D& segment, SegmentHit& hit);

    // Detects the collision between a segment and a cylinder empty inside, 
    // which would have two holes with the shape of a circle.
    static bool emptyCylinderSegmentCollision(const Cylinder& cylinder, const Segment3D& seg, SegmentHit& hit);

    static bool cylinderSegmentCollision(const Cylinder& cylinder, const Segment3D& seg, SegmentHit& hit);

    static float squaredDistPointSegment(const Core::Maths::Vec3& point, const Segment3D& seg);

    static bool capsuleSegmentCollision(const Cylinder& capsule, const Segment3D& seg, SegmentHit& hit);

    static bool centeredAABBSegmentCollision(const CenteredAABB& aabb, const Segment3D& seg, SegmentHit& hit);

    static bool aabbSegmentCollision(const AABB& aabb, const Segment3D& seg, SegmentHit& hit);

    static bool boxSegmentCollision(const Box& box, const Segment3D& seg, SegmentHit& hit);

    static bool centeredAABBSphereCollision(const CenteredAABB& centeredAABB, const Sphere& sphere);

    static bool sphereSphereCollision(const Sphere& sphere1, const Sphere& sphere2);

    static bool boxSphereCollision(const Box& box, const Sphere& sphere);

    static bool aabbAabbCollision(const AABB& aabb1, const AABB& aabb2);

    static bool boxBoxCollision(const Box& box1, const Box& box2);

    static bool centeredAABBMovingSphereCollision(CenteredAABB aabb, const Sphere& sphere, const Segment3D& seg, SegmentHit& hit);

    static bool boxMovingShereCollision(const Box& box, const Sphere& sphere, const Segment3D& seg, SegmentHit& hit);



public:
    static constexpr float epsilon = 0.f;
    static constexpr float negEpsilon = - epsilon;

    static inline bool isNearlyNull(float f);
    static inline bool areNearlyEqual(float f1, float f2);

    template<class T>
    static constexpr const T& clamp(const T& v, const T& lo, const T& hi);

    static inline bool areHigherThan(float p1, float p2, float f);
    static inline bool areLessThan(float p1, float p2, float f);


private:
    // return true if the projection of the vectors is between 0 and v2.vectorSquareLength().
    static inline bool isProjectionBetween0AndV2SquareLength(const Core::Maths::Vec3& v1, const Core::Maths::Vec3& v2);
};

#include "collisions.inl"

#endif