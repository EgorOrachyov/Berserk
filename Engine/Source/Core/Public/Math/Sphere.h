//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_SPHERE_H
#define BERSERK_SPHERE_H

#include "Public/Math/TVector3.h"
#include "Public/Strings/StaticString.h"
#include "Public/Misc/UsageDescriptors.h"

namespace Berserk
{

    /**
     * Sphere for objects bounding purposes
     */
    class CORE_EXPORT Sphere
    {
    public:

        /** With center (0,0,0) and radius (0) */
        Sphere();

        /** Copy */
        Sphere(const Sphere& source);

        /** From center point and radius */
        Sphere(const Vector3f& center, float32 radius);

        ~Sphere() = default;

    public:

        Sphere& operator = (const Sphere& source);

        /** Translate box on vector t */
        void operator += (const Vector3f& t);

        /** Translate box on vector t */
        void operator -= (const Vector3f& t);

        /** @return true if spheres in contact */
        bool contact(const Sphere& a) const;

        /** @return true if this sphere contains small one */
        bool contains(const Sphere& small) const;

        /** @return true if this sphere contains point */
        bool contains(const Vector3f& p) const;

        /** @return Sphere which contains this and a sphere */
        Sphere embrace(const Sphere& a) const;

        /** @return Intersection sphere of this and a spheres */
        Sphere intersect(const Sphere& a) const;

    public:

        /** @return true if sphere is empty */
        bool empty() const;

        /** @return Center point of this sphere */
        const Vector3f& center() const;

        /** @return Radius this sphere */
        float32 radius() const;

        /** @return String representation of the sphere */
        CName toString() const;

    public:

        /** @return true if a and b in contacts */
        static bool contact(const Sphere& a, const Sphere& b);

        /** @return true if big contains smaller sphere */
        static bool contains(const Sphere& big, const Sphere& small);

        /** @return Null sphere */
        static Sphere null();

        /** @return Sphere which contains a and b spheres */
        static Sphere embrace(const Sphere& a, const Sphere& b);

        /** @return Intersection of spheres a and b */
        static Sphere intersect(const Sphere& a, const Sphere& b);

    private:

        Vector3f mCenter;
        float32  mRadius;

    };

    Sphere::Sphere() : mCenter(0,0,0) , mRadius(0)
    {

    }

    Sphere::Sphere(const Sphere &source) : mCenter(source.mCenter) , mRadius(source.mRadius)
    {

    }

    Sphere::Sphere(const Vector3f &center, float32 radius) : mCenter(center) , mRadius(radius)
    {

    }

    Sphere& Sphere::operator=(const Sphere &source)
    {
        mCenter = source.mCenter;
        mRadius = source.mRadius;
    }

    void Sphere::operator+=(const Vector3f &t)
    {
        mCenter = mCenter + t;
    }

    void Sphere::operator-=(const Vector3f &t)
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

    bool Sphere::contains(const Vector3f &p) const
    {
        float32 dist = (mCenter - p).length();

        return (dist < mRadius);
    }

    Sphere Sphere::embrace(const Sphere &a) const
    {
        Vector3f c = (mCenter + a.mCenter) * 0.5f;
        float32 d = (mCenter - c).length();
        float32 r = d + Math::max(mRadius, a.mRadius);

        return Sphere(c, r);
    }

    Sphere Sphere::intersect(const Sphere &a) const
    {
        Vector3f c = (mCenter + a.mCenter) * 0.5f;
        float32 r = Math::abs(mRadius - a.mRadius);

        return Sphere(c, r);
    }

    bool Sphere::empty() const
    {
        return (mRadius <= 0);
    }

    const Vector3f& Sphere::center() const
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

} // namespace Berserk

#endif //BERSERK_SPHERE_H