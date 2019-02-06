//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_PLANE_H
#define BERSERK_PLANE_H

#include "Public/Math/MathUtility.h"
#include "Public/Math/AABB.h"
#include "Public/Math/Sphere.h"
#include "Public/Math/TVector3.h"
#include "Public/Strings/StaticString.h"
#include "Public/Misc/UsageDescriptors.h"

namespace Berserk
{

    class CORE_EXPORT Plane
    {
    public:

        /**  Plane with point (0,0,0) and norm (0,1,0) */
        Plane();

        Plane(const Plane& source) = default;

        /** From plane point and normal */
        Plane(const Vector3f& point, const Vector3f& normal);

        ~Plane() = default;

    public:

        /** @return True if point belong plane with some threshold */
        bool belongs(const Vector3f& p) const;

        /** @return True if box on positive plane side or intersects that */
        bool positive(const AABB& a) const;

        /** @return True if sphere  on positive plane side or intersects that */
        bool positive(const Sphere& a) const;

        /** @return Signed distance from plane to the point  */
        float32 distance(const Vector3f &p) const;

    public:

        /** @return Plane norm */
        const Vector3f& norm() const;

        /** @return Plane w component */
        float32 w() const;

    private:

        float32  mW;
        Vector3f mNorm;

    };

    Plane::Plane() : mW(0) , mNorm(0,1,0)
    {

    }

    Plane::Plane(const Vector3f &point, const Vector3f &normal) : mNorm(normal.getNormalized())
    {
        mW = Vector3f::dot(-point, mNorm);
    }

    bool Plane::belongs(const Vector3f &p) const
    {
        auto dist = Vector3f::dot(p, mNorm) + mW;
        return (dist <= Math::THRESH_POINT_ON_PLANE);
    }

    bool Plane::positive(const AABB &a) const
    {
        for (uint32 i = 0; i < 8; i++)
        {
            if (distance(a.vertex(i)) > 0.0f) return true;
        }

        return false;
    }

    bool Plane::positive(const Sphere &a) const
    {
        auto c = a.center();
        auto r = a.radius();
        auto d = distance(c);

        return (d > -r);
    }

    float32 Plane::distance(const Vector3f &p) const
    {
        return Vector3f::dot(p, mNorm) + mW;
    }

    const Vector3f& Plane::norm() const
    {
        return mNorm;
    }

    float32 Plane::w() const
    {
        return mW;
    }

} // namespace Berserk

#endif //BERSERK_PLANE_H