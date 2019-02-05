//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_AABB_H
#define BERSERK_AABB_H

#include "Public/Math/TVector3.h"
#include "Public/Strings/StaticString.h"
#include "Public/Misc/UsageDescriptors.h"

namespace Berserk
{

    /**
     * Axis aligned bounding box
     */
    class CORE_EXPORT AABB
    {
    public:

        /** Creates empty box (0,0,0) (0,0,0) */
        AABB();

        AABB(const AABB& source);

        /** Create box from center point and size of the side */
        AABB(const Vector3f& center, float32 size);

        /** Min and max points of the box */
        AABB(const Vector3f& min, const Vector3f& max);

        ~AABB() = default;

    public:

        AABB& operator = (const AABB& source);

        /** Translate box on vector t */
        void operator += (const Vector3f& t);

        /** Translate box on vector t */
        void operator -= (const Vector3f& t);

        /** @return true if boxes in contact */
        bool contact(const AABB& a) const;

        /** @return true if this box contains small box */
        bool contains(const AABB& small) const;

        /** @return Box which contains this and a box */
        AABB embrace(const AABB& a) const;

        /** @return Intersection box of this and a boxex */
        AABB intersect(const AABB& a) const;

    public:

        /** @return true if box is empty */
        bool empty() const;

        /** @return Sizes ot this box  */
        const Vector3f& size() const;

        /** @return Center point of this box */
        const Vector3f& center() const;

        /** @return Min bound */
        const Vector3f& min() const;

        /** @return Max bound */
        const Vector3f& max() const;

        /** @return String representation of the box */
        CName toString() const;

    public:

        /** @return true if a and b in contacts */
        static bool contact(const AABB& a, const AABB& b);

        /** @return true if big contains smaller box */
        static bool contains(const AABB& big, const AABB& small);

        /** @return Null box */
        static AABB null();

        /** @return Box which contains a and b boxes */
        static AABB embrace(const AABB& a, const AABB& b);

        /** @return Intersection of boxes a and b */
        static AABB intersect(const AABB& a, const AABB& b);

    private:

        TVector3<float32> mMin;
        TVector3<float32> mMax;

    };

    AABB::AABB() : mMin() , mMax()
    {

    }

    AABB::AABB(const AABB &source) : mMin(source.mMin) , mMax(source.mMax)
    {

    }

    AABB::AABB(const Vector3f &center, float32 size)
    {
        float32 h = size / 2.0f;

        mMin = center - Vector3f(h);
        mMax = center + Vector3f(h);
    }

    AABB::AABB(const Vector3f &min, const Vector3f &max) : mMin(min) , mMax(max)
    {

    }

    AABB& AABB::operator=(const AABB &source)
    {
        mMin = source.mMin;
        mMax = source.mMax;

        return *this;
    }

    void AABB::operator+=(const Vector3f &t)
    {
        mMin = mMin + t;
        mMax = mMax + t;
    }

    void AABB::operator-=(const Vector3f &t)
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

    const Vector3f& AABB::size() const
    {
        return mMax - mMin;
    }

    const Vector3f& AABB::center() const
    {
        return (mMax + mMin) * 0.5f;
    }

    const Vector3f& AABB::min() const
    {
        return mMin;
    }

    const Vector3f& AABB::max() const
    {
        return mMax;
    }

    CName AABB::toString() const
    {
        char buffer[Buffers::SIZE_64];
        sprintf(buffer, "[min=(X=%3.1f,Y=%3.1f,Z=%3.1f),max=(X=%3.1f,Y=%3.1f,Z=%3.1f)]"
                , mMin.x, mMin.y, mMin.z, mMax.x, mMax.y, mMax.z);
        return CName(buffer);
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

} // namespace Berserk

#endif //BERSERK_AABB_H