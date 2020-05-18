#include "collisions.hpp"

bool Collisions::planeSegmentCollision(const Plane& plane, const Segment3D& segment, SegmentHit& hit)
{
    // The collision point should respect :
    // - the segment equation : OM = OA + t* AB
    // - the plane   equation : n . OM = d
    // This leads to a new equation (cf schema) :
    // t = (d - n . OA) / (n . AB)

    // Getting AB vector.
    const Core::Maths::Vec3 lineDir  = segment.p2 - segment.p1;

    const float dotProd = Core::Maths::Vec3::dotProduct(plane.normal, lineDir);

    // If the line is parallel to the plane, then there is no collision,
    // since the segment is either already inside, either outside of the plane.
    if (isNearlyNull(dotProd))
        return false;

    // Now that we know that dotProd won't be 0, we can divide by it
    hit.t                   = (plane.distance - Core::Maths::Vec3::dotProduct(plane.normal, segment.p1)) / dotProd;
    hit.collisionPoint      = segment.p1 + hit.t * lineDir;
    hit.normal              = dotProd < 0 ? plane.normal : - plane.normal;

    return (hit.t >= 0.f && hit.t <= 1.f);
}

bool Collisions::rectangleSegmentCollision(const Rectangle& rect, const Segment3D& segment, SegmentHit& hit)
{
    Plane plane(rect.point, rect.normal);

    // If not in plane, there is no collision with the quad.
    if (!planeSegmentCollision(plane, segment, hit)) 
        return false;

    // Returns if the collision point of the plane is inside the quad.
    Core::Maths::Vec3 rectangleToPointVec = (hit.collisionPoint - rect.point);
    return (isProjectionBetween0AndV2SquareLength(rectangleToPointVec, rect.seg1) 
         && isProjectionBetween0AndV2SquareLength(rectangleToPointVec, rect.seg2));
}

bool Collisions::centeredSphereSegmentCollision(const CenteredSphere& sphere, const Segment3D& segment, SegmentHit& hit)
{
    // The collision point should respect :
    // - the segment equation : OM = OA + t* AB
    // - the sphere  equation : IM . IM = r²
    // This leads to a new equation :
    // - t² * ||AB||² + 2*t*(IA.AB) + ||IA||² - r² = 0 (cf Schema)
    // To find t, we have to resolve the equation. 
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
    // Also : if the segment is "facing" the sphere, there are 2 collision points, 
    // with both positive t values. So we want the lowest.
    // If the segment is not facing the sphere, both values of t would be negative and invalid,
    // so we don't care.
    // The exception is when seg.p1 is inside the sphere.
    // That would mean we want the maximum value, which is positive, 
    // since the minimum one would be negative. (The segment would always be "facing" the sphere.)

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

bool Collisions::sphereSegmentCollision(const Sphere& sphere, const Segment3D& segment, SegmentHit& hit)
{
    // The collision point should respect :
    // - the segment equation : OM = OA + t* AB
    // - the sphere  equation : IM . IM = r²
    // This leads to a new equation :
    // - t² * ||AB||² + 2*t*(IA.AB) + ||IA||² - r² = 0 (cf Schema)
    // To find t, we have to resolve the equation. 

    // Prevents division by 0.
    assert(!isNearlyNull(segment.squaredLength()));

    const Core::Maths::Vec3 circleCenterToSeg1 = segment.p1 - sphere.center;
    const Core::Maths::Vec3 segDir = segment.p2 - segment.p1;

    const float segDirSquared = segDir.vectorSquareLength();
    const float circleCenterToSeg1Squared = circleCenterToSeg1.vectorSquareLength();
    const float radiusSquared = sphere.radius * sphere.radius;

    // The square of Core::Maths::Vec3::dotProduct(sphere.center - segment.p1, segDir) 
    // is equal to the square of Core::Maths::Vec3::dotProduct(segDir, sphere.center - segment.p1),
    // so we can reuse circleCenterToSeg1 to gain performance.
    const float dotCircleCenterToSeg1ToSegDir = Core::Maths::Vec3::dotProduct(circleCenterToSeg1, segDir);
    // We test if the minimal distance between the line and the sphere origin is higher than the sphere radius.
    // If that's true, we are sure that there is no collision.

    // We have ||AB ^ AP||² / AB² > r² (if no collisions)
    // ||AB ^ AP||² = AB² * AP² - (AB.AP)², so : 
    // (AB² * AP² - (AB.AP)²) / AB² > r²
    // Not sure if it's really optimizing, we should benchmark.
    if ((circleCenterToSeg1Squared * segDirSquared 
        - dotCircleCenterToSeg1ToSegDir * dotCircleCenterToSeg1ToSegDir) / segDirSquared
        > radiusSquared)
        return false;

    const float a = segDirSquared;
    const float b = 2 * dotCircleCenterToSeg1ToSegDir; 
    const float c = circleCenterToSeg1Squared - radiusSquared;

    float delta = b * b - 4 * a * c;    

    // We already know it's not 0, 
    // since we know from the previous condition that the line is close enough for a collision.
    if (delta < 0)
    {
        return false; // no collision line / sphere
    }
    // We could put an if statement to test if delta == 0,
    // and it would increase the performance when that is the case.
    // However, actually, that would be very rare in case of collisions.
    // That is why, by not adding the if statement, 
    // we do not decrease overall optimization. 

    // We have two possible values for t : Let's call them t1 and t2.
    // t1 = (- b + sqrt(delta)) / (2 * a);
    // t2 = (- b - sqrt(delta)) / (2 * a);

    if (sphere.isInside(segment.p1)) // if segment.p1 is inside the sphere :   
    {
        // Then min(t1, t2) would be negative, 
        // because it would be in the opposite direction of segDir.
        // We already now from that t2 <= t1, so t = t1.

        hit.t = (- b + sqrt(delta)) / (2 * a);

        // Is Segment colliding with Sphere :
        // We know that hit.t will always be positive in this case.
        if (hit.t > 1.f)
            return false;

        hit.collisionPoint = segment.p1 + hit.t * segDir;
        hit.normal = (sphere.center - hit.collisionPoint).unitVector();
    }
    else // if segment.p1 is outside the sphere :   
    { 
        // Then t = min(t1, t2), because t1 and t2 would both be positive,
        // because they would be in the same direction of segDir (if its direction is towards the sphere).
        // We already now from that t2 <= t1, so t = t2.
        hit.t = (- b - sqrt(delta)) / (2 * a);

        // Is Segment colliding with Sphere :
        if (hit.t < 0.f || hit.t > 1.f)
            return false;

        hit.collisionPoint = segment.p1 + hit.t * segDir; 
        hit.normal = (hit.collisionPoint - sphere.center).unitVector(); 
    }

    return true; 
}


bool Collisions::infiniteCylinderSegmentCollision(const Cylinder& cylinder, const Segment3D& segment, SegmentHit& hit)
{
    // Prevents division by 0.
    assert(!isNearlyNull(segment.squaredLength()));
    assert(!isNearlyNull((cylinder.location - cylinder.dirInWorldLoc).vectorSquareLength()));

    const Core::Maths::Vec3 cross1 = Core::Maths::Vec3::crossProduct(segment.p1 - cylinder.location, cylinder.dirInWorldLoc - cylinder.location);
    const float cross1sqrt = cross1.vectorSquareLength();

    const float s = (cylinder.dirInWorldLoc - cylinder.location).vectorSquareLength();

    const float segSquaredLength = segment.squaredLength();
    const float squaredRadius = cylinder.radius * cylinder.radius;

    // The basic condition would be : 
    // if (sqrt(cross1sqrt) / sqrt(s) > cylinder.radius + sqrt(segment.squaredLength()))
    // However, computing the sqrt function would be slow, and we want to gain performance.
    // By computing the square instead, it gives :
    // if (cross1sqrt / s > squaredRadius 
    //                    + 2 * cylinder.radius * sqrt(segment.squaredLength()) 
    //                    + segment.squaredLength())
    // That is fine, but there is still one sqrt to compute.
    // However, we know that : segment.squaredLength() / 2 > sqrt(segment.squaredLength()))
    // This way, we can replace sqrt(segment.squaredLength())) by segment.squaredLength() / 2.
    // Of course, we lose some precision, and this optimization would only be useful if the collision is really far away.
    // However, if we optimize the collisions, that is because we want to put a lot of them without lags, so that's not a problem.

    // We already know that s != 0 from the previous assert
    if (cross1sqrt / s > squaredRadius
                       + cylinder.radius * segSquaredLength 
                       + segSquaredLength)
        return false;

    const Core::Maths::Vec3 cross2 = Core::Maths::Vec3::crossProduct(segment.p2 - segment.p1, cylinder.dirInWorldLoc - cylinder.location);


    const float c = cross1sqrt - squaredRadius * s;
    const float b = 2 * Core::Maths::Vec3::dotProduct(cross1, cross2);
    const float a = cross2.vectorSquareLength();

    const float delta = b * b - 4 * a * c;
    if (delta < 0.f)
        return false;

    // We have two possible values for t : Let's call them t1 and t2.
    // t1 = (- b + sqrt(delta)) / (2 * a);
    // t2 = (- b - sqrt(delta)) / (2 * a);

    if (cross1sqrt < squaredRadius) // if segment.p1 is inside the cylinder :   
    {
        // Then min(t1, t2) would be negative, 
        // because it would be in the opposite direction of segDir.
        // We already now from that t2 <= t1, so t = t1.
        hit.t = (- b + sqrt(delta)) / (2 * a);
        if (hit.t < 0 || hit.t > 1)
            return false;

        hit.collisionPoint = segment.p1 + hit.t * (segment.p2 - segment.p1);

        Core::Maths::Vec3 u = (cylinder.dirInWorldLoc - cylinder.location).unitVector();

        hit.normal = (u * Core::Maths::Vec3::dotProduct(hit.collisionPoint - cylinder.location, u) + cylinder.location) - hit.collisionPoint;
    }
    else // if segment.p1 is outside of the cylinder :   
    { 
        // Then t = min(t1, t2), because t1 and t2 would both be positive,
        // because they would be in the same direction of segDir (if its direction is towards the sphere).
        // We already now from that t2 <= t1, so t = t2.
        hit.t = (- b - sqrt(delta)) / (2 * a);
        if (hit.t < 0 || hit.t > 1)
            return false;

        hit.collisionPoint = segment.p1 + hit.t * (segment.p2 - segment.p1);

        Core::Maths::Vec3 u = (cylinder.dirInWorldLoc - cylinder.location).unitVector();

        hit.normal = hit.collisionPoint - (u * Core::Maths::Vec3::dotProduct(hit.collisionPoint - cylinder.location, u) + cylinder.location);
    }

    hit.normal = hit.normal.unitVector();

    return true;
}

bool Collisions::emptyCylinderSegmentCollision(const Cylinder& cylinder, const Segment3D& seg, SegmentHit& hit)
{
    if (infiniteCylinderSegmentCollision(cylinder, seg, hit))
    {
        // Is inside the finite cylinder, meaning that the projection is valid : 
        return isProjectionBetween0AndV2SquareLength(hit.collisionPoint - cylinder.location, cylinder.dirInWorldLoc - cylinder.location);
    }
    else 
        return false;
}

bool Collisions::cylinderSegmentCollision(const Cylinder& cylinder, const Segment3D& seg, SegmentHit& hit)
{
    // Set t higher than 1, so the condition tempHit.t < hit.t will work on the first time, 
    // without other branches.
    hit.t = 2.f;
    bool hasCollided = false;

    const float dotp1 = Core::Maths::Vec3::dotProduct(seg.p1 - cylinder.location, cylinder.dirInWorldLoc - cylinder.location); 
    const float dotp2 = Core::Maths::Vec3::dotProduct(seg.p2 - cylinder.location, cylinder.dirInWorldLoc - cylinder.location);

    const float squareCylSeg = (cylinder.dirInWorldLoc - cylinder.location).vectorSquareLength();

    // If the projections are "upper" than the cylinder or if they are "lower", then there is no collision.
    if ((dotp1 < 0.f && dotp2 < 0.f) || (dotp1 > squareCylSeg && dotp2 > squareCylSeg))
        return false; 

    // If p1 is in the side of cylinder.location and p2 is in another side,
    // or if p2 is in the side of cylinder.location and p1 is in the middle side 
    // (if p1 was in the side of cylinder.dirInWorldLoc, 
    // if there was a collision with the circle, 
    // then there would have been a collision with a different shape first). 
    if (dotp1 <= 0 || (dotp2 <= 0 && dotp1 <= squareCylSeg))
    {
        SegmentHit tempHit;
        Plane plane1 {cylinder.location, {(cylinder.dirInWorldLoc - cylinder.location).unitVector()}};
        bool isSphereCollision1 = planeSegmentCollision(plane1, seg, tempHit);

        // Is colliding with circle
        if (isSphereCollision1 && (tempHit.collisionPoint - cylinder.location).vectorSquareLength() < cylinder.radius * cylinder.radius)
        {
            hasCollided = true;
            hit = tempHit;
        }
    }

    // Same than before, but with the different side
    if (dotp1 >= squareCylSeg || (dotp2 >= squareCylSeg && dotp1 >= 0))
    {
        SegmentHit tempHit;
        Plane plane2 {cylinder.dirInWorldLoc, {(cylinder.location - cylinder.dirInWorldLoc).unitVector()}};
        bool isSphereCollision2 = planeSegmentCollision(plane2, seg, tempHit);

        // Is colliding with circle
        if (isSphereCollision2 && (tempHit.collisionPoint - cylinder.dirInWorldLoc).vectorSquareLength() < cylinder.radius * cylinder.radius)
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
    
    // always test for a potential collision with the empty cylinder
    SegmentHit tempHit;
    if (emptyCylinderSegmentCollision(cylinder, seg, tempHit))
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

    return hasCollided;
}

float Collisions::squaredDistPointSegment(const Core::Maths::Vec3& point, const Segment3D& seg)
{
    const Core::Maths::Vec3 segDir = seg.p2 - seg.p1;
    float proj = Core::Maths::Vec3::dotProduct(point - seg.p1, segDir);

    if (proj <= 0)
    {
        return (point - seg.p1).vectorSquareLength();
    }
    else if (proj >= segDir.vectorSquareLength())
    {
        return (point - seg.p2).vectorSquareLength();
    }
    else 
    {
        // Returns closest distance from the point to the straight line
        return Core::Maths::Vec3::crossProduct(point - seg.p1, segDir).vectorSquareLength() / segDir.vectorSquareLength();
    }
}

bool Collisions::capsuleSegmentCollision(const Cylinder& capsule, const Segment3D& seg, SegmentHit& hit)
{   
    // Set t higher than 1, so the condition tempHit.t < hit.t will work on the first time, 
    // without other branches.
    hit.t = 2.f;
    bool hasCollided = false;

    const float dotp1 = Core::Maths::Vec3::dotProduct(seg.p1 - capsule.location, capsule.dirInWorldLoc - capsule.location); 
    const float dotp2 = Core::Maths::Vec3::dotProduct(seg.p2 - capsule.location, capsule.dirInWorldLoc - capsule.location);

    const float squareRadius = capsule.radius * capsule.radius;

    // otherwise, we should use the sqrt function
    const float dotp1FromOtherSide = Core::Maths::Vec3::dotProduct(seg.p1 - capsule.dirInWorldLoc, capsule.location - capsule.dirInWorldLoc); 
    const float dotp2FromOtherSide = Core::Maths::Vec3::dotProduct(seg.p2 - capsule.dirInWorldLoc, capsule.location - capsule.dirInWorldLoc);

    // // If the projections are "upper" than the cylinder or if they are "lower", then there is no collision.
    if ((dotp1 < - squareRadius && dotp2 < - squareRadius) 
     || (dotp1FromOtherSide < - squareRadius && dotp2FromOtherSide < - squareRadius))
     {
        return false;
     } 

    // If p1 is in the side of cylinder.location and p2 is in another side,
    // or if p2 is in the side of cylinder.location:
    if (dotp1 <= 0 || dotp2 <= 0)
    {
        Sphere sphere2 {capsule.location, capsule.radius};
        bool isSphereCollision2 = sphereSegmentCollision(sphere2, seg, hit);

        // Is colliding with circle
        if (isSphereCollision2 && Core::Maths::Vec3::dotProduct((capsule.dirInWorldLoc - capsule.location), hit.collisionPoint - capsule.location) < 0)
        {
            hasCollided = true;
        }
    }

    // Same than before, but with the different side
    if (dotp1FromOtherSide < 0 || dotp2FromOtherSide < 0)
    {
        SegmentHit tempHit;
        Sphere sphere1 {capsule.dirInWorldLoc, capsule.radius};
        bool isSphereCollision1 = sphereSegmentCollision(sphere1, seg, tempHit);

        // Is colliding with circle
        if (isSphereCollision1 && Core::Maths::Vec3::dotProduct((capsule.location - capsule.dirInWorldLoc), tempHit.collisionPoint - capsule.dirInWorldLoc) < 0)
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
     || (dotp1FromOtherSide <= 0.f && dotp2FromOtherSide <= 0.f)))
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

bool Collisions::centeredAABBSegmentCollision(const CenteredAABB& aabb, const Segment3D& seg, SegmentHit& hit)
{
    // If both points are on the same side of the cube,
    // then we are sure they are not colliding.
    // We can return false to prevent too much computation.
    if (areLessThan(seg.p1.x, seg.p2.x, - aabb.size.x)
        || areHigherThan(seg.p1.x, seg.p2.x, aabb.size.x)
        || areLessThan(seg.p1.y, seg.p2.y, - aabb.size.y)
        || areHigherThan(seg.p1.y, seg.p2.y, aabb.size.y)
        || areLessThan(seg.p1.z, seg.p2.z, - aabb.size.z)
        || areHigherThan(seg.p1.z, seg.p2.z, aabb.size.z))
        return false;

    // Set collision flag to false.
    bool hasCollided = false;

    // We use hit.t later on to know if the new t found is valid.
    // However, for the first time looping, we have no value to compare with,
    // but we also know it must be less thn 1.
    // By doing this assignment, we prevent some weird exception
    hit.t = 1.f;

    // Prevents the use of the modulo to check the next axis indices.
    static constexpr const unsigned int nextIndexTableP1[3] = 
    {
        1, 2, 0
    };
    static constexpr const unsigned int nextIndexTableP2[3] = 
    {
        2, 0, 1
    };

    // for each axis
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
                    const unsigned int axisIDn1 = nextIndexTableP1[axisID];
                    const unsigned int axisIDn2 = nextIndexTableP2[axisID];

                    // is collision inside a square
                    if (hitPoint[axisIDn1] * hitPoint[axisIDn1] <= aabb.size[axisIDn1] * aabb.size[axisIDn1]
                        && hitPoint[axisIDn2] * hitPoint[axisIDn2] <= aabb.size[axisIDn2] * aabb.size[axisIDn2]) 
                    {
                        hit.t = newHitT;
                        // TODO : add static Core::Maths::Vec3 getAxis(unsigned int id) in Core::Maths::Vec3 class ? 

                        hit.normal = Core::Maths::Vec3::getAxis(axisID);

                        //== Remove the following if the segment should not start inside the box ==/
                        // Since it would be useless in that case, removing it would increase performance.
                        if (!isNegative && seg.p1[axisID] < boundingValue)
                            hit.normal *= -1.f;
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


bool Collisions::aabbSegmentCollision(const AABB& aabb, const Segment3D& seg, SegmentHit& hit)
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

bool Collisions::boxSegmentCollision(const Box& box, const Segment3D& seg, SegmentHit& hit)
{
    Core::Maths::Matrix4x4 mInv = box.transform.getInverse();
    Segment3D transSeg = Segment3D{mInv * Core::Maths::Vec4{seg.p1, 1}, 
                                   mInv * Core::Maths::Vec4{seg.p2, 1}};

    if (centeredAABBSegmentCollision(box.aabb, transSeg, hit))
    {
        hit.collisionPoint = box.transform * Core::Maths::Vec4{hit.collisionPoint, 1.f};
        hit.normal = box.transform * Core::Maths::Vec4{hit.normal, 0.f};
        hit.normal = hit.normal.unitVector();
        return true;
    }
    else 
        return false;
}

bool Collisions::centeredAABBSphereCollision(const CenteredAABB& centeredAABB, const Sphere& sphere)
{
    Core::Maths::Vec3 nearestPoint 
    {
        clamp(sphere.center.x, - centeredAABB.size.x, centeredAABB.size.x),
        clamp(sphere.center.y, - centeredAABB.size.y, centeredAABB.size.y),
        clamp(sphere.center.z, - centeredAABB.size.z, centeredAABB.size.z)
    };

    return (nearestPoint - sphere.center).vectorSquareLength() < sphere.radius * sphere.radius;
}

bool Collisions::boxSphereCollision(const Box& box, const Sphere& sphere)
{
    Core::Maths::Matrix4x4 mInv = box.transform;
    mInv.normalizeScale();

    Core::Maths::Vec3 newSphereCenter = Core::Maths::Matrix4x4(mInv.getInverse()) * sphere.center;

    return centeredAABBSphereCollision(box.transform.getScale(), {newSphereCenter, sphere.radius});
}

bool Collisions::aabbAabbCollision(const AABB& aabb1, const AABB& aabb2)
{
    // Compares the range for each dimension ;
    return aabb1.location.x - aabb1.centeredAABB.size.x < aabb2.location.x + aabb2.centeredAABB.size.x 
        && aabb1.location.x + aabb1.centeredAABB.size.x > aabb2.location.x - aabb2.centeredAABB.size.x 

        && aabb1.location.y - aabb1.centeredAABB.size.y < aabb2.location.y + aabb2.centeredAABB.size.y 
        && aabb1.location.y + aabb1.centeredAABB.size.y > aabb2.location.y - aabb2.centeredAABB.size.y 

        && aabb1.location.z - aabb1.centeredAABB.size.z < aabb2.location.z + aabb2.centeredAABB.size.z 
        && aabb1.location.z + aabb1.centeredAABB.size.z > aabb2.location.z - aabb2.centeredAABB.size.z;
}

bool Collisions::boxBoxCollision(const Box& box1, const Box& box2)
{  
    if (aabbAabbCollision(box1, box2))
    {
        // Uses the SAT theorem

        // Get the axes we will project on.
        const Core::Maths::Vec3 axisArray[] = 
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

        for (const Core::Maths::Vec3& axis : axisArray)
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
    else 
        return false;
}

bool Collisions::centeredAABBMovingSphereCollision(CenteredAABB aabb, const Sphere& sphere, const Segment3D& seg, SegmentHit& hit)
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
    bool isInside = aabb.isPointInside(seg.p1);
    if (isInside)
    {
        aabb.size -= sphere.radius;
    }
    else 
    {
        aabb.size += sphere.radius;
    }

    if (centeredAABBSegmentCollision(aabb, seg, testHit))
    {
        if (isInside)
        {
            aabb.size += sphere.radius;
        }
        else 
        {
            aabb.size -= sphere.radius;
        }

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
        {
            aabb.size += sphere.radius;
        }
        else 
        {
            aabb.size -= sphere.radius;
        }

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

    // will never be called
    return false;
}

bool Collisions::boxMovingShereCollision(const Box& box, const Sphere& sphere, const Segment3D& seg, SegmentHit& hit)
{
    Sphere s;
    // s.center = // We don't have to set the center, 
    // because we won't use it later (we'll use seg.p1 instead)
    s.radius = sphere.radius;

    // We will have to "add" the sphere radius and the scale of the box later,
    // for the Minkowski addition.
    // That is why we will send an aabb of size box.scale.
    // However, if we didn't remove the scale from the matrix,
    // That would mean that the sphere would also be "scaled",
    // and since we have only a point, we could not scale it correctly.
    Core::Maths::Matrix4x4 m = box.transform;
    m.normalizeScale();

    Core::Maths::Matrix4x4 mInverse = m.getInverse();
    // The new Segment, relative to the box
    Segment3D seg2;
    seg2.p1 = mInverse * seg.p1;
    seg2.p2 = mInverse * seg.p2;

    if (centeredAABBMovingSphereCollision(box.transform.getScale(), s, seg2, hit))
    {
        // Pu the collision point and normal back to the previous referential.
        hit.collisionPoint = m * Core::Maths::Vec4(hit.collisionPoint, 1);
        hit.normal         = m * Core::Maths::Vec4(hit.normal, 0);
        hit.normal         = hit.normal.unitVector();
        return true;
    }
    else 
        return false;
}


