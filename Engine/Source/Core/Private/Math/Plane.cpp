//
// Created by Egor Orachyov on 21.02.2019.
//

#include "Math/Plane.h"

namespace Berserk
{

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

}