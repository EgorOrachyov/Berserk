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

    /**
     * Structure for three dimensional planes.
     *
     * Stores the coefficients as [n*p + W=0], where n - is normalized plane normal vector
     * and value W = dot(-p,n), where p - is point on the plane
     */
    class CORE_EXPORT Plane
    {
    public:

        /**  Plane with point (0,0,0) and norm (0,1,0) */
        Plane();

        Plane(const Plane& source) = default;

        /** From plane point and normal */
        Plane(const Vec3f& point, const Vec3f& normal);

        ~Plane() = default;

    public:

        /** Moves plane in the direction of vector t */
        void operator += (const Vec3f& t);

        /** Moves plane in the direction of vector -t */
        void operator -= (const Vec3f& t);

        /** @return True if point belong plane with some threshold */
        bool belongs(const Vec3f& p) const;

        /** @return True if box on positive plane side or intersects that */
        bool positive(const AABB& a) const;

        /** @return True if sphere  on positive plane side or intersects that */
        bool positive(const Sphere& a) const;

        /** @return Signed distance from plane to the point  */
        float32 distance(const Vec3f &p) const;

    public:

        /** @return Plane norm */
        inline const Vec3f& norm() const;

        /** @return Plane w component */
        inline float32 w() const;

        /** @return Positive (the nearest) vertex to the plane in the direction of its normal vector */
        inline Vec3f positiveVertex(const AABB& a) const
        {
            Vec3f p = a.min();
            Vec3f m = a.max();

            if (mNorm.x >= 0.0f) p.x = m.x;
            if (mNorm.y >= 0.0f) p.y = m.y;
            if (mNorm.z >= 0.0f) p.z = m.z;

            return p;
        }

    private:

        friend class Frustum;

        float32  mW;
        Vec3f mNorm;

    };

    Plane::Plane() : mW(0) , mNorm(0,1,0)
    {

    }

    Plane::Plane(const Vec3f &point, const Vec3f &normal) : mNorm(normal.getNormalized())
    {
        mW = Vec3f::dot(-point, mNorm);
    }

    void Plane::operator+=(const Vec3f &t)
    {
        mW += Vec3f::dot(-t, mNorm);
    }

    void Plane::operator-=(const Vec3f &t)
    {
        mW += Vec3f::dot(t, mNorm);
    }

    bool Plane::belongs(const Vec3f &p) const
    {
        auto dist = Vec3f::dot(p, mNorm) + mW;
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

    float32 Plane::distance(const Vec3f &p) const
    {
        return Vec3f::dot(p, mNorm) + mW;
    }

    const Vec3f& Plane::norm() const
    {
        return mNorm;
    }

    float32 Plane::w() const
    {
        return mW;
    }

} // namespace Berserk

#endif //BERSERK_PLANE_H