//
// Created by Egor Orachyov on 21.02.2019.
//

#include "Math/AABB.h"

namespace Berserk
{

    AABB::AABB() : mMin() , mMax()
    {

    }

    AABB::AABB(const AABB &source) : mMin(source.mMin) , mMax(source.mMax)
    {

    }

    AABB::AABB(const Vec3f &center, float32 size)
    {
        float32 h = size / 2.0f;

        mMin = center - Vec3f(h);
        mMax = center + Vec3f(h);
    }

    AABB::AABB(const Vec3f &min, const Vec3f &max) : mMin(min) , mMax(max)
    {

    }

    AABB& AABB::operator=(const AABB &source)
    {
        mMin = source.mMin;
        mMax = source.mMax;

        return *this;
    }

    void AABB::operator+=(const Vec3f &t)
    {
        mMin = mMin + t;
        mMax = mMax + t;
    }

    void AABB::operator-=(const Vec3f &t)
    {
        mMin = mMin - t;
        mMax = mMax - t;
    }

    bool AABB::contact(const AABB &a) const
    {
        if (mMax.x < a.mMin.x || a.mMax.x < mMin.x) return false;
        if (mMax.y < a.mMin.y || a.mMax.y < mMin.y) return false;
        if (mMax.z < a.mMin.z || a.mMax.z < mMin.z) return false;

        return true;
    }

    bool AABB::contains(const AABB &small) const
    {
        if (small.mMax.x < mMin.x || mMax.x < small.mMin.x) return false;
        if (small.mMax.y < mMin.y || mMax.y < small.mMin.y) return false;
        if (small.mMax.z < mMin.z || mMax.z < small.mMin.z) return false;

        return true;
    }

    bool AABB::contains(const Vec3f &p) const
    {
        if (p.x < mMin.x || mMax.x < p.x) return false;
        if (p.y < mMin.y || mMax.y < p.y) return false;
        if (p.z < mMin.z || mMax.z < p.z) return false;

        return true;
    }

    AABB AABB::embrace(const AABB &a) const
    {
        AABB r;

        r.mMin.x= Math::min(mMin.x, a.mMin.x);
        r.mMax.x= Math::max(mMax.x, a.mMax.x);

        r.mMin.y= Math::min(mMin.y, a.mMin.y);
        r.mMax.y= Math::max(mMax.y, a.mMax.y);

        r.mMin.z= Math::min(mMin.z, a.mMin.z);
        r.mMax.z= Math::max(mMax.z, a.mMax.z);

        return r;
    }

    AABB AABB::intersect(const AABB &a) const
    {
        AABB r;

        r.mMin.x= Math::max(mMin.x, a.mMin.x);
        r.mMax.x= Math::min(mMax.x, a.mMax.x);

        r.mMin.y= Math::max(mMin.y, a.mMin.y);
        r.mMax.y= Math::min(mMax.y, a.mMax.y);

        r.mMin.z= Math::max(mMin.z, a.mMin.z);
        r.mMax.z= Math::min(mMax.z, a.mMax.z);

        if (r.empty()) return null();
        else return r;
    }

    bool AABB::empty() const
    {
        if (mMin.x >= mMax.x) return true;
        if (mMin.y >= mMax.y) return true;
        if (mMin.z >= mMax.z) return true;

        return false;
    }

    const Vec3f& AABB::size() const
    {
        return mMax - mMin;
    }

    const Vec3f& AABB::center() const
    {
        return (mMax + mMin) * 0.5f;
    }

    const Vec3f& AABB::min() const
    {
        return mMin;
    }

    const Vec3f& AABB::max() const
    {
        return mMax;
    }

    Name AABB::toString() const
    {
        char buffer[Buffers::SIZE_64];
        sprintf(buffer, "[min=(X=%3.1f,Y=%3.1f,Z=%3.1f),max=(X=%3.1f,Y=%3.1f,Z=%3.1f)]"
                , mMin.x, mMin.y, mMin.z, mMax.x, mMax.y, mMax.z);
        return Name(buffer);
    }

    bool AABB::contact(const AABB &a, const AABB &b)
    {
        return a.contact(b);
    }

    bool AABB::contains(const AABB &big, const AABB &small)
    {
        return big.contains(small);
    }

    AABB AABB::null()
    {
        return AABB();
    }

    AABB AABB::embrace(const AABB &a, const AABB &b)
    {
        return a.embrace(b);
    }

    AABB AABB::intersect(const AABB &a, const AABB &b)
    {
        return a.intersect(b);
    }

}