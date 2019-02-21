//
// Created by Egor Orachyov on 21.02.2019.
//

#include "Math/Sphere.h"

namespace Berserk
{

    Sphere::Sphere() : mCenter(0,0,0) , mRadius(0)
    {

    }

    Sphere::Sphere(const Sphere &source) : mCenter(source.mCenter) , mRadius(source.mRadius)
    {

    }

    Sphere::Sphere(const Vec3f &center, float32 radius) : mCenter(center) , mRadius(radius)
    {

    }

    Sphere& Sphere::operator=(const Sphere &source)
    {
        mCenter = source.mCenter;
        mRadius = source.mRadius;
    }

    void Sphere::operator+=(const Vec3f &t)
    {
        mCenter = mCenter + t;
    }

    void Sphere::operator-=(const Vec3f &t)
    {
        mCenter = mCenter - t;
    }

    bool Sphere::contact(const Sphere &a) const
    {
        float32 sq_dist = (mCenter - a.mCenter).norm();
        float32 sum_rad = mRadius + a.mRadius;

        return (sq_dist < sum_rad * sum_rad);
    }

    bool Sphere::contains(const Sphere &small) const
    {
        float32 dist = (mCenter - small.mCenter).length();

        return (dist + small.mRadius < mRadius);
    }

    bool Sphere::contains(const Vec3f &p) const
    {
        float32 dist = (mCenter - p).length();

        return (dist < mRadius);
    }

    Sphere Sphere::embrace(const Sphere &a) const
    {
        Vec3f c = (mCenter + a.mCenter) * 0.5f;
        float32 d = (mCenter - c).length();
        float32 r = d + Math::max(mRadius, a.mRadius);

        return Sphere(c, r);
    }

    Sphere Sphere::intersect(const Sphere &a) const
    {
        Vec3f c = (mCenter + a.mCenter) * 0.5f;
        float32 r = Math::abs(mRadius - a.mRadius);

        return Sphere(c, r);
    }

    bool Sphere::empty() const
    {
        return (mRadius <= 0);
    }

    const Vec3f& Sphere::center() const
    {
        return mCenter;
    }

    float32 Sphere::radius() const
    {
        return mRadius;
    }

    CName Sphere::toString() const
    {
        char buffer[Buffers::SIZE_64];
        sprintf(buffer, "[c=(X=%3.2f,Y=%3.2f,Z=%3.2f),r=%3.2f]", mCenter.x, mCenter.y, mCenter.z, mRadius);
        return CName(buffer);
    }

    bool Sphere::contact(const Sphere &a, const Sphere &b)
    {
        return a.contact(b);
    }

    bool Sphere::contains(const Sphere &big, const Sphere &small)
    {
        return big.contains(small);
    }

    Sphere Sphere::null()
    {
        return Sphere();
    }

    Sphere Sphere::embrace(const Sphere &a, const Sphere &b)
    {
        return a.embrace(b);
    }

    Sphere Sphere::intersect(const Sphere &a, const Sphere &b)
    {
        return a.intersect(b);
    }

}